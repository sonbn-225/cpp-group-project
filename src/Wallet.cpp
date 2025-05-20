#include "Wallet.h"
#include "Utils.h" // For time_point_to_string

Wallet::Wallet() : walletId(-1), balance(0.0) {
    createdAt = std::chrono::system_clock::now();
    updatedAt = createdAt;
}

Wallet::Wallet(long long id, std::string owner, double initialBalance)
    : walletId(id), ownerUsername(std::move(owner)), balance(initialBalance) {
    createdAt = std::chrono::system_clock::now();
    updatedAt = createdAt;
}

void Wallet::addTransaction(const TransactionRecord& tr) {
    transactionHistory.push_back(tr);
    updatedAt = std::chrono::system_clock::now();
}

void to_json(json& j, const Wallet& w) {
    j = json{
        {"walletId", w.walletId},
        {"ownerUsername", w.ownerUsername},
        {"balance", w.balance},
        {"transactionHistory", w.transactionHistory}, // TransactionRecord needs to_json
        {"createdAt", Utils::time_point_to_string(w.createdAt)},
        {"updatedAt", Utils::time_point_to_string(w.updatedAt)}
    };
}

void from_json(const json& j, Wallet& w) {
    j.at("walletId").get_to(w.walletId);
    j.at("ownerUsername").get_to(w.ownerUsername);
    j.at("balance").get_to(w.balance);
    j.at("transactionHistory").get_to(w.transactionHistory); // TransactionRecord needs from_json
    w.createdAt = Utils::string_to_time_point(j.at("createdAt").get<std::string>());
    w.updatedAt = Utils::string_to_time_point(j.at("updatedAt").get<std::string>());
}