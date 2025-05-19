#ifndef TRANSACTIONRECORD_H
#define TRANSACTIONRECORD_H

#include <string>
#include <chrono>
#include "../json.hpp" // nlohmann/json
#include "Utils.h"     // For time conversion utilities

using json = nlohmann::json;

enum class TransactionStatus {
    PENDING,
    COMPLETED,
    FAILED_INSUFFICIENT_FUNDS,
    FAILED_OTP,
    CANCELLED_BY_USER,
    UNKNOWN_ERROR
};

// For JSON serialization of TransactionStatus
NLOHMANN_JSON_SERIALIZE_ENUM(TransactionStatus, {
    {TransactionStatus::PENDING, "PENDING"},
    {TransactionStatus::COMPLETED, "COMPLETED"},
    {TransactionStatus::FAILED_INSUFFICIENT_FUNDS, "FAILED_INSUFFICIENT_FUNDS"},
    {TransactionStatus::FAILED_OTP, "FAILED_OTP"},
    {TransactionStatus::CANCELLED_BY_USER, "CANCELLED_BY_USER"},
    {TransactionStatus::UNKNOWN_ERROR, "UNKNOWN_ERROR"}
})

struct TransactionRecord {
    std::string transactionId;
    std::chrono::system_clock::time_point timestamp;
    long long fromWalletId;
    long long toWalletId;
    double amount;
    TransactionStatus status;
    std::string description;

    TransactionRecord(); // Default constructor for JSON
    TransactionRecord(std::string id, long long from, long long to, double amt,
                      TransactionStatus stat, std::string desc = "");

    friend void to_json(json& j, const TransactionRecord& tr);
    friend void from_json(const json& j, TransactionRecord& tr);
};

// Implementation for TransactionRecord constructors and JSON (can be in .cpp or here for simplicity)
inline TransactionRecord::TransactionRecord()
    : fromWalletId(0), toWalletId(0), amount(0.0), status(TransactionStatus::PENDING) {
    timestamp = std::chrono::system_clock::now();
    transactionId = Utils::generateUUID();
}

inline TransactionRecord::TransactionRecord(std::string id, long long from, long long to, double amt,
                                     TransactionStatus stat, std::string desc)
    : transactionId(std::move(id)), fromWalletId(from), toWalletId(to), amount(amt),
      status(stat), description(std::move(desc)) {
    timestamp = std::chrono::system_clock::now();
}

inline void to_json(json& j, const TransactionRecord& tr) {
    j = json{
        {"transactionId", tr.transactionId},
        {"timestamp", Utils::time_point_to_string(tr.timestamp)},
        {"fromWalletId", tr.fromWalletId},
        {"toWalletId", tr.toWalletId},
        {"amount", tr.amount},
        {"status", tr.status},
        {"description", tr.description}
    };
}

inline void from_json(const json& j, TransactionRecord& tr) {
    j.at("transactionId").get_to(tr.transactionId);
    tr.timestamp = Utils::string_to_time_point(j.at("timestamp").get<std::string>());
    j.at("fromWalletId").get_to(tr.fromWalletId);
    j.at("toWalletId").get_to(tr.toWalletId);
    j.at("amount").get_to(tr.amount);
    j.at("status").get_to(tr.status);
    j.at("description").get_to(tr.description);
}

#endif // TRANSACTIONRECORD_H