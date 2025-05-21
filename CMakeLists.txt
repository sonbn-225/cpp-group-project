cmake_minimum_required(VERSION 3.31)
project(wallet_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(wallet_cpp
        src/main.cpp
        src/AuthService.cpp
        src/DataStorage.cpp
        src/UserAccount.cpp
        src/Utils.cpp
        src/Wallet.cpp
        src/WalletService.cpp
)

target_include_directories(wallet_cpp PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
