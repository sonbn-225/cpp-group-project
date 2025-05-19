#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include <chrono>
#include "TransactionRecord.h" // Includes json.hpp indirectly
// #include "../json.hpp" // nlohmann/json - already included by TransactionRecord.h

// using json = nlohmann::json; // Already declared in TransactionRecord.h

class Wallet {
public:
    long long walletId;
    std::string ownerUsername; // Username of the wallet owner
    double balance;
    std::vector<TransactionRecord> transactionHistory;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point updatedAt;


    Wallet(); // Default constructor for JSON
    Wallet(long long id, std::string owner, double initialBalance = 0.0);

    void addTransaction(const TransactionRecord& tr);

    friend void to_json(json& j, const Wallet& w);
    friend void from_json(const json& j, Wallet& w);
};

#endif // WALLET_H