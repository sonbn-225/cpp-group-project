#ifndef UTILS_H
#define UTILS_H

#include "Constants.h" // For OTP_LENGTH

#include <string>
#include <vector>
#include <chrono> // Required for time_point_to_string and string_to_time_point
#include <iomanip> // Required for put_time and get_time
#include <sstream> // Required for stringstream

// Forward declaration for nlohmann::json if needed, or include json.hpp
// #include "json.hpp" // If you use json in utils signatures (not strictly needed here)


namespace Utils {
    std::string generateUUID();
    std::string generateSalt(size_t length = 16);
    std::string mockHashPassword(const std::string& password, const std::string& salt); // NOT SECURE!
    std::string generateRandomPassword(size_t length = 12);
    std::string generateOTPSecret(); // Generates a mock secret
    std::string generateNumericOTP(const std::string& secret, size_t length = OTP_LENGTH); // Mock OTP generation
    bool validateOTP(const std::string& secret, const std::string& otpToValidate); // Mock OTP validation
    long long generateUniqueId(); // Simple ID generator

    // Time conversion utilities
    std::string time_point_to_string(const std::chrono::system_clock::time_point& tp);
    std::chrono::system_clock::time_point string_to_time_point(const std::string& s);

    void clearScreen();
    void pauseExecution();

} // namespace Utils

#endif // UTILS_H