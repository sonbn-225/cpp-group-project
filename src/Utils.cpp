#include "Utils.h"
#include "Constants.h" // For OTP_LENGTH
#include <random>
#include <algorithm>
#include <sstream>
#include <iomanip> // For std::put_time, std::get_time
#include <iostream> // For clearScreen, pauseExecution
#include <cstdlib>  // For system() in clearScreen, pauseExecution (platform dependent)

namespace Utils {

    std::string generateUUID() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15);
        std::uniform_int_distribution<> dis2(8, 11);

        std::stringstream ss;
        ss << std::hex;
        for (int i = 0; i < 8; i++) ss << dis(gen);
        ss << "-";
        for (int i = 0; i < 4; i++) ss << dis(gen);
        ss << "-4"; // version 4
        for (int i = 0; i < 3; i++) ss << dis(gen);
        ss << "-";
        ss << dis2(gen); // variant
        for (int i = 0; i < 3; i++) ss << dis(gen);
        ss << "-";
        for (int i = 0; i < 12; i++) ss << dis(gen);
        return ss.str();
    }

    std::string generateSalt(size_t length) {
        const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        std::random_device random_device;
        std::mt19937 generator(random_device());
        std::uniform_int_distribution<> distribution(0, CHARACTERS.length() - 1);
        std::string random_string;
        for (std::size_t i = 0; i < length; ++i) {
            random_string += CHARACTERS[distribution(generator)];
        }
        return random_string;
    }

    // WARNING: THIS IS A MOCK HASH FUNCTION AND IS NOT SECURE!
    // DO NOT USE IN PRODUCTION. Use bcrypt, scrypt, or Argon2.
    std::string mockHashPassword(const std::string& password, const std::string& salt) {
        std::string toHash = salt + password + salt; // Simple combination
        unsigned long hash = 5381; // djb2
        for (char c : toHash) {
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }
        return "MOCK_HASH_" + std::to_string(hash);
    }

    std::string generateRandomPassword(size_t length) {
        const std::string CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(0, CHARS.length() - 1);
        std::string password;
        for (size_t i = 0; i < length; ++i) {
            password += CHARS[distribution(generator)];
        }
        return password;
    }

    // Mock OTP generation - NOT SECURE
    std::string generateOTPSecret() {
        return "OTPSECRET_" + generateSalt(16); // Just a random string
    }

    std::string generateNumericOTP(const std::string& secret, size_t length) {
        // In a real system, this would use TOTP/HOTP algorithms.
        // This mock version just uses part of the "secret" or a fixed value.
        if (secret.empty()) return "";
        std::string base = "123456"; // Fallback OTP
        if (secret.length() >= length) {
           base = secret.substr(secret.length() - length); // Simplistic
        }
         // Ensure it's numeric for the mock
        std::string numericOtp = "";
        for(char c : base) {
            if(numericOtp.length() < length) {
                numericOtp += std::to_string(c % 10);
            }
        }
        while(numericOtp.length() < length) numericOtp += "0";

        return numericOtp.substr(0, length);
    }

    bool validateOTP(const std::string& secret, const std::string& otpToValidate) {
        // Mock validation: checks if the provided OTP matches the one we'd generate.
        // A real system would validate against a time window for TOTP.
        if (secret.empty() || otpToValidate.empty()) return false;
        return otpToValidate == generateNumericOTP(secret, otpToValidate.length());
    }

    long long generateUniqueId() {
        static long long idCounter = std::chrono::system_clock::now().time_since_epoch().count() / 100000; // Semi-unique based on time
        return ++idCounter;
    }

    std::string time_point_to_string(const std::chrono::system_clock::time_point& tp) {
        std::time_t t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm = *std::localtime(&t); // Using localtime for local timezone representation
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    std::chrono::system_clock::time_point string_to_time_point(const std::string& s) {
        std::tm tm = {};
        std::stringstream ss(s);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (ss.fail()) {
            // Handle error, perhaps return a default time_point or throw exception
            // For simplicity, returning epoch if parsing fails
            return std::chrono::system_clock::from_time_t(0);
        }
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    void clearScreen() {
        #if defined(_WIN32) || defined(_WIN64)
            std::system("cls");
        #else
            // Assume POSIX
            std::system("clear");
        #endif
    }

    void pauseExecution() {
        std::cout << "\nNhan Enter de tiep tuc...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear previous newline
        std::cin.get();
    }

} // namespace Utils