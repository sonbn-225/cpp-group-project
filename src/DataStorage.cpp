#include "DataStorage.h"
#include "Constants.h"
#include <fstream>
#include <iostream>
#include <filesystem> // For std::filesystem::create_directory
#include <algorithm> // For std::find_if

namespace fs = std::filesystem;

DataStorage::DataStorage() {
    ensureDataDirectoryExists();
    loadUsers();
    loadWallets();
    // loadGlobalTransactions(); // Optional
    initializeMasterWalletAndAdmin(); // Call this after loading to check if init is needed
}

bool DataStorage::ensureDataDirectoryExists() {
    try {
        if (!fs::exists(DATA_DIR)) {
            fs::create_directories(DATA_DIR);
            std::cout << "Thu muc data da duoc tao: " << DATA_DIR << std::endl;
        }
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Loi tao thu muc data: " << e.what() << std::endl;
        return false;
    }
}

void DataStorage::initializeMasterWalletAndAdmin() {
    bool adminExists = false;
    for(const auto& user : users) {
        if(user.username == "admin") {
            adminExists = true;
            break;
        }
    }

    if (!adminExists) {
        std::cout << "Khong tim thay tai khoan admin. Tao tai khoan admin mac dinh..." << std::endl;
        long long adminWalletId = Utils::generateUniqueId();
        UserAccount adminUser("admin", "admin123", "Default Admin", "admin@system.local", UserRole::ADMIN_USER, "", adminWalletId);
        addUser(adminUser); // This will also save users if implemented that way or needs explicit saveUsers()

        Wallet adminWallet(adminWalletId, "admin", 1000.0); // Admin gets some starting points
        addWallet(adminWallet);

        std::cout << "Tai khoan admin (admin/admin123) va vi cua admin da duoc tao." << std::endl;
    }

    if (wallets.find(MASTER_WALLET_ID) == wallets.end()) {
        std::cout << "Khong tim thay Vi Tong. Tao Vi Tong mac dinh..." << std::endl;
        Wallet masterWallet(MASTER_WALLET_ID, MASTER_WALLET_OWNER, INITIAL_MASTER_WALLET_BALANCE);
        addWallet(masterWallet);
        std::cout << "Vi Tong (ID: " << MASTER_WALLET_ID << ") da duoc tao." << std::endl;
    }
    saveUsers(); // Ensure changes are saved
    saveWallets();
}


bool DataStorage::loadUsers() {
    std::ifstream file(USERS_FILE);
    if (!file.is_open()) {
        // std::cerr << "Khong the mo file users: " << USERS_FILE << ". Se tao moi neu can." << std::endl;
        return false; // File might not exist yet, which is fine on first run
    }
    try {
        json j;
        file >> j;
        users = j.get<std::vector<UserAccount>>();
    } catch (const json::exception& e) {
        std::cerr << "Loi phan tich users.json: " << e.what() << std::endl;
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool DataStorage::saveUsers() const {
    std::ofstream file(USERS_FILE);
    if (!file.is_open()) {
        std::cerr << "Khong the mo file de ghi users: " << USERS_FILE << std::endl;
        return false;
    }
    try {
        json j = users;
        file << j.dump(4); // Pretty print with 4 spaces
    } catch (const json::exception& e) {
        std::cerr << "Loi JSON khi luu users: " << e.what() << std::endl;
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool DataStorage::loadWallets() {
    std::ifstream file(WALLETS_FILE);
    if (!file.is_open()) {
        // std::cerr << "Khong the mo file wallets: " << WALLETS_FILE << ". Se tao moi neu can." << std::endl;
        return false;
    }
    try {
        json j;
        file >> j;
        // Wallets are stored as a map in JSON like {"walletId_str": wallet_obj, ...}
        // Or as a list of wallet objects. Let's assume a list for simplicity of direct map loading
        // If it's a list in JSON:
        // std::vector<Wallet> walletList = j.get<std::vector<Wallet>>();
        // for(const auto& w : walletList) {
        //     wallets[w.walletId] = w;
        // }
        // If it's a map in JSON directly (requires walletId to be string in JSON key)
        // For now, we'll assume the JSON stores an array of wallet objects
        // And we convert it to map on load.
        if (j.is_array()) {
            std::vector<Wallet> wallet_list = j.get<std::vector<Wallet>>();
            for (const auto& wallet_obj : wallet_list) {
                wallets[wallet_obj.walletId] = wallet_obj;
            }
        } else { // Or if the JSON directly represents a map where keys are stringified wallet IDs
             wallets = j.get<std::map<long long, Wallet>>(); // This works if json keys are numbers
        }

    } catch (const json::exception& e) {
        std::cerr << "Loi phan tich wallets.json: " << e.what() << std::endl;
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool DataStorage::saveWallets() const {
    std::ofstream file(WALLETS_FILE);
    if (!file.is_open()) {
        std::cerr << "Khong the mo file de ghi wallets: " << WALLETS_FILE << std::endl;
        return false;
    }
    try {
        // To save map as a JSON array of wallet objects:
        json j = json::array();
        for(const auto& pair : wallets) {
            j.push_back(pair.second);
        }
        // Or to save as a JSON object with walletId as key (needs string keys for JSON usually)
        // json j = wallets; // This works if nlohmann/json handles long long keys correctly or if they are strings
        file << j.dump(4);
    } catch (const json::exception& e) {
        std::cerr << "Loi JSON khi luu wallets: " << e.what() << std::endl;
        file.close();
        return false;
    }
    file.close();
    return true;
}


UserAccount* DataStorage::findUser(const std::string& username) {
    auto it = std::find_if(users.begin(), users.end(),
                           [&](const UserAccount& u){ return u.username == username; });
    if (it != users.end()) {
        return &(*it);
    }
    return nullptr;
}

Wallet* DataStorage::findWallet(long long walletId) {
    auto it = wallets.find(walletId);
    if (it != wallets.end()) {
        return &(it->second);
    }
    return nullptr;
}

void DataStorage::addUser(const UserAccount& user) {
    // Check if user already exists to avoid duplicates if username is key
    UserAccount* existingUser = findUser(user.username);
    if (existingUser) {
        *existingUser = user; // Update existing user
    } else {
        users.push_back(user);
    }
    // saveUsers(); // Consider batching saves
}

void DataStorage::addWallet(const Wallet& wallet) {
    wallets[wallet.walletId] = wallet;
    // saveWallets(); // Consider batching saves
}