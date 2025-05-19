#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <vector>
#include <map> // For wallets, mapping ID to Wallet
#include "UserAccount.h"
#include "Wallet.h"
// TransactionRecord is included via Wallet.h

class DataStorage {
public:
    std::vector<UserAccount> users;
    std::map<long long, Wallet> wallets; // Key is walletId
    // std::vector<TransactionRecord> globalTransactions; // Optional global log

    DataStorage();

    bool loadUsers();
    bool saveUsers() const;

    bool loadWallets();
    bool saveWallets() const;

    // bool loadGlobalTransactions(); // Optional
    // bool saveGlobalTransactions() const; // Optional

    UserAccount* findUser(const std::string& username);
    Wallet* findWallet(long long walletId);

    void addUser(const UserAccount& user);
    void addWallet(const Wallet& wallet);
    // void addGlobalTransaction(const TransactionRecord& tr); // Optional

    bool ensureDataDirectoryExists();

    void initializeMasterWalletAndAdmin(); // For first time setup
};

#endif // DATASTORAGE_H