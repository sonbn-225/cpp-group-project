#include "WalletService.h"
#include "Utils.h"
#include "Constants.h" // For MASTER_WALLET_ID
#include <iostream>
#include <iomanip> // For std::fixed, std::setprecision

WalletService::WalletService(DataStorage& ds, AuthService& as) : storage(ds), authService(as) {}

bool WalletService::transferPoints(UserAccount* senderUser, long long recipientWalletId, double amount, const std::string& description) {
    if (!senderUser) {
        std::cerr << "Nguoi dung gui khong hop le." << std::endl;
        return false;
    }
    if (amount <= 0) {
        std::cerr << "So diem chuyen phai lon hon 0." << std::endl;
        return false;
    }

    Wallet* senderWallet = storage.findWallet(senderUser->walletId);
    Wallet* recipientWallet = storage.findWallet(recipientWalletId);

    if (!senderWallet) {
        std::cerr << "Khong tim thay vi cua nguoi gui." << std::endl;
        return false;
    }
    if (!recipientWallet) {
        std::cerr << "Khong tim thay vi cua nguoi nhan (ID: " << recipientWalletId << ")." << std::endl;
        return false;
    }
    if (senderWallet->walletId == recipientWallet->walletId) {
        std::cerr << "Ban khong the chuyen diem cho chinh minh." << std::endl;
        return false;
    }

    // OTP Verification if enabled for sender
    if (senderUser->isOtpEnabled) {
        std::cout << "Vui long nhap ma OTP de xac nhan giao dich: ";
        std::string otpCode;
        std::cin >> otpCode;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!authService.verifyOTP(senderUser, otpCode)) {
            std::cerr << "Xac thuc OTP that bai. Giao dich bi huy." << std::endl;
            // Log failed transaction due to OTP
            TransactionRecord tr(Utils::generateUUID(), senderWallet->walletId, recipientWalletId, amount, TransactionStatus::FAILED_OTP, description);
            senderWallet->addTransaction(tr);
            // Don't add to recipient's history for failed OTP on sender's side
            storage.saveWallets(); // Save sender's log
            return false;
        }
    }

    if (senderWallet->balance < amount) {
        std::cerr << "So du khong du. Giao dich that bai." << std::endl;
        TransactionRecord tr(Utils::generateUUID(), senderWallet->walletId, recipientWalletId, amount, TransactionStatus::FAILED_INSUFFICIENT_FUNDS, description);
        senderWallet->addTransaction(tr);
        storage.saveWallets();
        return false;
    }

    // --- ATOMIC PART (simulated) ---
    // In a real DB, this would be a transaction. Here, we do it in memory then save.
    // If save fails, data might be inconsistent until next successful load/save or manual fix.
    double senderOldBalance = senderWallet->balance;
    double recipientOldBalance = recipientWallet->balance;

    senderWallet->balance -= amount;
    recipientWallet->balance += amount;

    TransactionRecord tr(Utils::generateUUID(), senderWallet->walletId, recipientWalletId, amount, TransactionStatus::COMPLETED, description);
    senderWallet->addTransaction(tr);
    recipientWallet->addTransaction(tr); // Both wallets get a record of the same transaction

    senderWallet->updatedAt = std::chrono::system_clock::now();
    recipientWallet->updatedAt = std::chrono::system_clock::now();

    if (storage.saveWallets()) {
        std::cout << "Chuyen diem thanh cong!" << std::endl;
        std::cout << amount << " diem da duoc chuyen tu vi " << senderWallet->walletId
                  << " den vi " << recipientWallet->walletId << "." << std::endl;
        return true;
    } else {
        std::cerr << "Loi nghiem trong: Khong the luu trang thai vi sau giao dich." << std::endl;
        // Attempt to rollback in-memory changes (though data on disk might be stale)
        senderWallet->balance = senderOldBalance;
        recipientWallet->balance = recipientOldBalance;
        // Remove the last transaction record if rollback (complex, needs careful handling)
        // For simplicity, we won't remove the transaction record from history here if save failed,
        // but mark it as potentially problematic or log an error.
        // The transaction status is already COMPLETED in memory. This is a limitation.
        std::cerr << "DA CO LOI KHI LUU FILE WALLETS. DU LIEU CO THE KHONG NHAT QUAN." << std::endl;
        return false;
    }
    // --- END ATOMIC PART ---
}

void WalletService::viewBalance(const UserAccount* user) const {
    if (!user) return;
    Wallet* wallet = storage.findWallet(user->walletId);
    if (wallet) {
        std::cout << "So du hien tai cua ban (Vi ID: " << wallet->walletId << "): "
                  << std::fixed << std::setprecision(2) << wallet->balance << " diem." << std::endl;
    } else {
        std::cerr << "Khong tim thay thong tin vi cho nguoi dung." << std::endl;
    }
}

void WalletService::viewTransactionHistory(const UserAccount* user) const {
    if (!user) return;
    Wallet* wallet = storage.findWallet(user->walletId);
    if (wallet) {
        std::cout << "\n--- Lich Su Giao Dich cho Vi ID: " << wallet->walletId << " ---" << std::endl;
        if (wallet->transactionHistory.empty()) {
            std::cout << "Khong co giao dich nao." << std::endl;
        } else {
            // Sort by timestamp descending (most recent first)
            std::vector<TransactionRecord> sortedHistory = wallet->transactionHistory;
            std::sort(sortedHistory.begin(), sortedHistory.end(), [](const TransactionRecord& a, const TransactionRecord& b){
                return a.timestamp > b.timestamp;
            });

            for (const auto& tr : sortedHistory) {
                std::cout << "ID: " << tr.transactionId << std::endl;
                std::cout << "  Thoi gian: " << Utils::time_point_to_string(tr.timestamp) << std::endl;
                std::cout << "  Tu Vi ID : " << tr.fromWalletId
                          << (tr.fromWalletId == wallet->walletId ? " (Ban)" : "") << std::endl;
                std::cout << "  Toi Vi ID: " << tr.toWalletId
                          << (tr.toWalletId == wallet->walletId ? " (Ban)" : "") << std::endl;
                std::cout << "  So diem  : " << std::fixed << std::setprecision(2) << tr.amount << std::endl;
                std::cout << "  Trang thai: ";
                switch(tr.status){
                    case TransactionStatus::COMPLETED: std::cout << "Hoan thanh"; break;
                    case TransactionStatus::PENDING: std::cout << "Dang xu ly"; break;
                    case TransactionStatus::FAILED_INSUFFICIENT_FUNDS: std::cout << "That bai (Khong du so du)"; break;
                    case TransactionStatus::FAILED_OTP: std::cout << "That bai (OTP sai)"; break;
                    case TransactionStatus::CANCELLED_BY_USER: std::cout << "Da huy"; break;
                    default: std::cout << "Khong xac dinh"; break;
                }
                std::cout << std::endl;
                if (!tr.description.empty()) {
                    std::cout << "  Mo ta: " << tr.description << std::endl;
                }
                std::cout << "------------------------------------" << std::endl;
            }
        }
    } else {
        std::cerr << "Khong tim thay thong tin vi cho nguoi dung." << std::endl;
    }
}

bool WalletService::fundWalletFromMaster(long long targetWalletId, double amount, const std::string& description, UserAccount* adminUser) {
    if (!adminUser || adminUser->role != UserRole::ADMIN_USER) {
        std::cerr << "Chuc nang nay chi danh cho Quan tri vien." << std::endl;
        return false;
    }
     if (amount <= 0) {
        std::cerr << "So diem nap phai lon hon 0." << std::endl;
        return false;
    }

    Wallet* masterWallet = storage.findWallet(MASTER_WALLET_ID);
    Wallet* targetWallet = storage.findWallet(targetWalletId);

    if (!masterWallet) {
        std::cerr << "Loi: Khong tim thay Vi Tong (Master Wallet)." << std::endl;
        return false;
    }
    if (!targetWallet) {
        std::cerr << "Khong tim thay vi dich (ID: " << targetWalletId << ")." << std::endl;
        return false;
    }
    if (targetWalletId == MASTER_WALLET_ID) {
        std::cerr << "Khong the nap diem vao chinh Vi Tong." << std::endl;
        return false;
    }

    // OTP for admin action if enabled for admin
    if (adminUser->isOtpEnabled) {
        std::cout << "Quan tri vien " << adminUser->username << ", vui long nhap ma OTP de xac nhan nap diem: ";
        std::string otpCode;
        std::cin >> otpCode;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!authService.verifyOTP(adminUser, otpCode)) {
            std::cerr << "Xac thuc OTP that bai. Giao dich bi huy." << std::endl;
            return false;
        }
    }


    if (masterWallet->balance < amount) {
        std::cerr << "So du Vi Tong (" << masterWallet->balance << ") khong du de nap " << amount << " diem." << std::endl;
        // Log this attempt?
        return false;
    }

    masterWallet->balance -= amount;
    targetWallet->balance += amount;

    TransactionRecord tr(Utils::generateUUID(), MASTER_WALLET_ID, targetWalletId, amount, TransactionStatus::COMPLETED, description + " (Admin: " + adminUser->username + ")");
    masterWallet->addTransaction(tr);
    targetWallet->addTransaction(tr);

    masterWallet->updatedAt = std::chrono::system_clock::now();
    targetWallet->updatedAt = std::chrono::system_clock::now();


    if (storage.saveWallets()) {
        std::cout << "Nap diem thanh cong!" << std::endl;
        std::cout << amount << " diem da duoc chuyen tu Vi Tong den vi " << targetWalletId << "." << std::endl;
        return true;
    } else {
        std::cerr << "Loi nghiem trong: Khong the luu trang thai vi sau khi nap diem." << std::endl;
        // Attempt rollback (difficult here without proper transaction management)
        masterWallet->balance += amount; // Rollback in-memory
        targetWallet->balance -= amount; // Rollback in-memory
        std::cerr << "DA CO LOI KHI LUU FILE WALLETS. DU LIEU CO THE KHONG NHAT QUAN." << std::endl;
        return false;
    }
}