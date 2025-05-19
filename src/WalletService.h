#ifndef WALLETSERVICE_H
#define WALLETSERVICE_H

#include "DataStorage.h" // Includes Wallet.h, TransactionRecord.h
#include "AuthService.h" // For OTP verification

class WalletService {
private:
    DataStorage& storage;
    AuthService& authService; // For OTP

public:
    WalletService(DataStorage& ds, AuthService& as);

    bool transferPoints(UserAccount* senderUser, long long recipientWalletId, double amount, const std::string& description);
    void viewBalance(const UserAccount* user) const;
    void viewTransactionHistory(const UserAccount* user) const;

    // Admin functions
    bool fundWalletFromMaster(long long targetWalletId, double amount, const std::string& description, UserAccount* adminUser);
};

#endif // WALLETSERVICE_H