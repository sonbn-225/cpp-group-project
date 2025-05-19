#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// File paths
const std::string DATA_DIR = "data/";
const std::string USERS_FILE = DATA_DIR + "users.json";
const std::string WALLETS_FILE = DATA_DIR + "wallets.json";
const std::string TRANSACTIONS_FILE = DATA_DIR + "transactions.json"; // Optional: for global transaction log

// User roles (though UserAccount.h has an enum, this could be for string representation if needed)
const std::string ROLE_ADMIN = "ADMIN";
const std::string ROLE_USER = "USER";

// Wallet constants
const long long MASTER_WALLET_ID = 0;
const std::string MASTER_WALLET_OWNER = "SYSTEM";
const double INITIAL_MASTER_WALLET_BALANCE = 1000000.0; // Example total points in system

// Security (Placeholder values, use strong settings in production)
const int MIN_PASSWORD_LENGTH = 8;
const int OTP_LENGTH = 6; // Standard OTP length
const int MAX_FAILED_LOGIN_ATTEMPTS = 5;

#endif // CONSTANTS_H