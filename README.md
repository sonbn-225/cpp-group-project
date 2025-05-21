# Hệ thống Đăng nhập và Quản lý Ví Điểm Thưởng

## 1. Giới thiệu dự án

Dự án này là một ứng dụng dòng lệnh (CLI) được viết bằng C++, mô phỏng một hệ thống đăng nhập người dùng cơ bản kết hợp với chức năng quản lý ví điểm thưởng. Người dùng có thể đăng ký, đăng nhập, quản lý thông tin cá nhân và thực hiện các giao dịch chuyển điểm giữa các ví. Hệ thống cũng bao gồm vai trò quản trị viên với các quyền hạn được mở rộng để quản lý người dùng và hệ thống điểm.

Mục tiêu chính của dự án là xây dựng một hệ thống dòng lệnh cho phép người dùng đăng ký, đăng nhập, quản lý tài khoản và thực hiện các giao dịch điểm thưởng. Hệ thống cần đảm bảo tính bảo mật cơ bản cho thông tin người dùng và tính toàn vẹn cho các giao dịch. Dự án được thiết kế với mục đích học tập và minh họa các khái niệm cơ bản trong phát triển phần mềm như quản lý người dùng, bảo mật (ở mức độ cơ bản với OTP stub), quản lý giao dịch và lưu trữ dữ liệu.

## 2. Thành viên tham gia dự án

* **Bùi Ngọc Sơn:** Phân tích yêu cầu, Thiết kế kiến trúc chính của hệ thống, Code chính, Review code, Hỗ trợ kỹ thuật chung.
* **Nguyễn Đức Bình:** hoàn thiện 2 hàm để tạo dựng dự án.
* **Ngô Thế Quang:** Kiểm thử hoặc Gỡ lỗi, hoàn thiện 2 hàm để tạo dựng dự án
* **Bùi Đức Tâm:** Thiết kế và triển khai cấu trúc dữ liệu cho `UserAccount` và `Wallet`

*(Đóng góp công việc chi tiết hơn có thể tham khảo trong các git log).*

## 3. Phân tích và Đặc tả Chức năng

Hệ thống được thiết kế để cung cấp các chức năng cốt lõi sau, chia theo vai trò người dùng:

### 3.1. Chức năng Chung (Cho mọi người dùng)
* **Đăng ký tài khoản:**
    * Input: Tên đăng nhập, mật khẩu, họ tên, email, số điện thoại (tùy chọn).
    * Output: Tạo tài khoản mới, tạo ví điểm liên kết, lưu thông tin vào CSDL (tệp JSON).
    * Xử lý: Kiểm tra tên đăng nhập tồn tại, kiểm tra độ mạnh mật khẩu cơ bản, băm mật khẩu (mock), tạo salt.
* **Đăng nhập:**
    * Input: Tên đăng nhập, mật khẩu.
    * Output: Cho phép truy cập vào các chức năng tương ứng với vai trò, thông báo lỗi nếu sai thông tin hoặc tài khoản bị khóa.
    * Xử lý: Tìm kiếm người dùng, xác thực mật khẩu (so sánh hash), kiểm tra trạng thái tài khoản (khóa/mở khóa), cập nhật số lần đăng nhập thất bại.

### 3.2. Chức năng Người dùng thường (Sau khi đăng nhập)
* **Xem Thông Tin Cá Nhân:**
    * Input: Không có.
    * Output: Hiển thị tên đăng nhập, họ tên, email, SĐT, trạng thái OTP, ID Ví.
* **Cập Nhật Thông Tin Cá Nhân:**
    * Input: Thông tin mới cho họ tên, email, SĐT. Mã OTP nếu đã kích hoạt.
    * Output: Cập nhật thông tin trong CSDL, thông báo thành công/thất bại.
    * Xử lý: Yêu cầu OTP (nếu cần), lưu thay đổi.
* **Đổi Mật Khẩu:**
    * Input: Mật khẩu cũ, mật khẩu mới (2 lần).
    * Output: Cập nhật mật khẩu mới (đã băm) vào CSDL, thông báo.
    * Xử lý: Xác thực mật khẩu cũ, kiểm tra độ mạnh mật khẩu mới.
* **Thiết Lập/Quản Lý OTP (Stub):**
    * Kích hoạt OTP:
        * Input: Mã OTP từ ứng dụng xác thực (sau khi quét/nhập khóa bí mật).
        * Output: Lưu khóa bí mật (Base32), kích hoạt trạng thái OTP cho người dùng. Hiển thị URI/khóa để người dùng thêm vào ứng dụng.
        * Xử lý: Tạo khóa bí mật (stub), xác minh mã OTP đầu tiên (stub).
    * Vô hiệu hóa OTP:
        * Input: Mật khẩu hiện tại, mã OTP hiện tại.
        * Output: Tắt trạng thái OTP, xóa khóa bí mật.
        * Xử lý: Xác thực mật khẩu, xác minh OTP (stub).
* **Xem Số Dư Ví:**
    * Input: Không có.
    * Output: Hiển thị số điểm hiện có.
* **Chuyển Điểm:**
    * Input: ID ví người nhận, số điểm, mô tả (tùy chọn). Mã OTP nếu đã kích hoạt.
    * Output: Thực hiện giao dịch, cập nhật số dư hai ví, ghi lịch sử giao dịch, thông báo.
    * Xử lý: Yêu cầu OTP (nếu cần), kiểm tra số dư đủ, thực hiện giao dịch (trừ ví gửi, cộng ví nhận), ghi log.
* **Xem Lịch Sử Giao Dịch:**
    * Input: Không có.
    * Output: Danh sách các giao dịch (gửi/nhận) của ví người dùng.
* **Đăng Xuất:**
    * Input: Không có.
    * Output: Kết thúc phiên làm việc, quay lại menu chính.

### 3.3. Chức năng Quản trị viên (Ngoài các chức năng của người dùng thường)
* **Liệt Kê Danh Sách Người Dùng:**
    * Input: Không có.
    * Output: Hiển thị danh sách người dùng (tên đăng nhập, họ tên, email, vai trò, trạng thái khóa).
* **Tạo Tài Khoản Người Dùng Mới (Làm hộ):**
    * Input: Thông tin người dùng (tên đăng nhập, họ tên, email, SĐT, vai trò), tùy chọn tự động sinh mật khẩu.
    * Output: Tạo tài khoản mới, ví liên kết, thông báo mật khẩu (nếu tự sinh).
    * Xử lý: Tương tự đăng ký thông thường, nhưng do admin thực hiện.
* **Chỉnh Sửa Thông Tin Người Dùng (Làm hộ):**
    * Input: Tên đăng nhập người dùng cần sửa, thông tin mới. Mã OTP của người dùng đó (nếu họ đã bật OTP và admin cần lấy từ họ).
    * Output: Cập nhật thông tin, thông báo.
    * Xử lý: Yêu cầu OTP (nếu cần), lưu thay đổi.
* **Khóa/Mở Khóa Tài Khoản Người Dùng:**
    * Input: Tên đăng nhập người dùng.
    * Output: Thay đổi trạng thái khóa của tài khoản, thông báo.
* **Nạp Điểm Vào Ví Người Dùng (Từ Ví Tổng):**
    * Input: ID ví người nhận, số điểm, mô tả. Mã OTP của admin nếu đã bật.
    * Output: Chuyển điểm từ Ví Tổng sang ví người dùng, ghi lịch sử, thông báo.
    * Xử lý: Yêu cầu OTP admin (nếu cần), kiểm tra số dư Ví Tổng, thực hiện giao dịch.
* **Xóa Tất Cả Tài Khoản Người Dùng Thường:**
    * Input: Xác nhận (y/n). Mã OTP của admin nếu đã bật.
    * Output: Xóa tất cả người dùng có vai trò "REGULAR_USER" và ví liên quan của họ khỏi CSDL. Giữ lại tài khoản admin và Ví Tổng.
    * Xử lý: Yêu cầu xác nhận nhiều lần, yêu cầu OTP admin (nếu cần), lọc và xóa dữ liệu.

## 4. Cài đặt và Biên dịch Chương trình

### 4.1. Yêu cầu hệ thống và Thư viện
* **Mã nguồn:**
    * Toàn bộ mã nguồn của dự án nằm trong thư mục `src/`.
    * Tệp `CMakeLists.txt` (khuyến nghị cho CLion) và `Makefile` (tùy chọn) được cung cấp trong thư mục gốc.
* **Thư viện nlohmann/json (Xử lý JSON):**
    * Tải tệp `json.hpp` (phiên bản single-header) từ: [https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp](https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp)
    * **Quan trọng:** Đặt tệp `json.hpp` này vào thư mục gốc của dự án (cùng cấp với `src/`, `CMakeLists.txt`, `Makefile`).
* **Trình biên dịch C++:**
    * Cần trình biên dịch C++ hỗ trợ C++20 (theo cấu hình `CMakeLists.txt` hiện tại). Ví dụ: g++ phiên bản 10 trở lên, Clang 10+, MSVC 2019 (v16.10)+.
    * Đối với Windows, bạn có thể cài đặt MinGW-w64 (đảm bảo phiên bản hỗ trợ C++20) hoặc sử dụng MSYS2 để cài đặt toolchain.
* **Công cụ xây dựng:**
    * **CMake (Khuyến nghị cho CLion):** Phiên bản tối thiểu 3.15 (mặc dù `CMakeLists.txt` hiện tại đặt 3.31). CLion sử dụng CMake làm hệ thống xây dựng mặc định.
    * **GNU Make (Tùy chọn):** Nếu bạn muốn sử dụng `Makefile` được cung cấp.
* **Biến môi trường PATH:** Đảm bảo thư mục `bin` của trình biên dịch C++ (ví dụ: `g++`) và công cụ xây dựng (`cmake` hoặc `make`) đã được thêm vào biến môi trường `PATH` của hệ thống. Khởi động lại terminal/command prompt sau khi thay đổi PATH.

### 4.2. Cách biên dịch chương trình

**Cách 1: Sử dụng CMake với CLion (Khuyến nghị)**
![image](https://github.com/user-attachments/assets/9896a937-dcd1-48ca-a85c-3405312cfc44)

1.  **Chuẩn bị thư viện:** Đảm bảo bạn đã đặt `json.hpp` ở thư mục gốc.
2.  **Mở dự án:** Mở thư mục gốc của dự án (nơi chứa `CMakeLists.txt`) bằng CLion.
3.  **Tải dự án CMake:** CLion sẽ tự động phát hiện và tải `CMakeLists.txt`. Nếu có thông báo yêu cầu "Reload CMake Project", hãy chấp nhận.
4.  **Biên dịch:**
    * Chọn "Build" > "Build Project" từ menu.
    * Hoặc nhấn vào biểu tượng build (thường là hình cái búa) trên thanh công cụ.
    * Tệp thực thi (ví dụ: `wallet_cpp.exe` hoặc `wallet_cpp`) sẽ được tạo trong thư mục build của CMake (thường là `cmake-build-debug` hoặc `cmake-build-release` bên trong thư mục dự án).

**Cách 2: Sử dụng `Makefile` (Nếu không dùng CLion/CMake)**
1.  **Chuẩn bị thư viện:** Tương tự như trên.
2.  **Mở Terminal/Command Prompt:** Di chuyển đến thư mục gốc của dự án (nơi chứa `Makefile`).
3.  **Biên dịch:** Chạy lệnh:
    ```bash
    make
    ```
    Lệnh này sẽ sử dụng `g++` (hoặc trình biên dịch C++ mặc định của bạn) để biên dịch các tệp nguồn. Tệp thực thi (ví dụ: `login_wallet_system`) sẽ được tạo trong thư mục `bin/` bên trong thư mục gốc của dự án.
4.  **Dọn dẹp (Clean):** Để xóa các tệp object (`.o`) và tệp thực thi đã tạo bằng `make`, chạy:
    ```bash
    make clean
    ```

## 5. Cách Chạy Chương trình

1.  **Sau khi biên dịch thành công:**
    * **Nếu dùng CLion/CMake:** Tệp thực thi thường nằm trong thư mục `cmake-build-debug/` (ví dụ: `cmake-build-debug/wallet_cpp.exe`).
    * **Nếu dùng `Makefile`:** Tệp thực thi nằm trong thư mục `bin/` (ví dụ: `bin/login_wallet_system`).
2.  **Chạy từ CLion:**
    * Chọn cấu hình chạy (thường CLion sẽ tự tạo một cấu hình cho target `wallet_cpp`).
    * Nhấn nút Run (biểu tượng tam giác màu xanh).
    * **Thư mục làm việc:** Chương trình tạo thư mục `data/` ở thư mục gốc của dự án. Để đảm bảo điều này khi chạy từ CLion, bạn có thể cần cấu hình "Working directory" trong "Run/Debug Configurations" thành `$CMakeProjectRoot$` hoặc `${CMAKE_SOURCE_DIR}`.
3.  **Chạy từ Terminal/Command Prompt:**
    * Di chuyển vào thư mục chứa tệp thực thi.
    * Chạy tệp thực thi:
        * Linux/macOS: `./wallet_cpp` (hoặc `./login_wallet_system` nếu dùng Makefile)
        * Windows: `wallet_cpp.exe` (hoặc `login_wallet_system.exe` nếu dùng Makefile)

### Lần chạy đầu tiên:
* Chương trình sẽ kiểm tra và nếu không tìm thấy tệp dữ liệu, nó sẽ tự động tạo:
    * Tài khoản quản trị viên mặc định: Tên đăng nhập `admin`, Mật khẩu `admin123`.
    * Ví Tổng (Master Wallet) với một lượng điểm khởi tạo.
* Bạn nên đăng nhập bằng tài khoản admin này và **đổi mật khẩu ngay lập tức**.

## 6. Hướng dẫn Sử dụng

Chương trình hoạt động dựa trên menu lựa chọn bằng số. Sau mỗi hành động, chương trình sẽ hiển thị kết quả và đợi bạn nhấn "Enter" để tiếp tục, sau đó sẽ hiển thị lại menu hiện tại.

*(Nội dung chi tiết của Mục 6.1, 6.2, 6.3, 6.4 giữ nguyên như trong README hiện tại của bạn, vì nó đã mô tả khá rõ các thao tác thực hiện).*

### 6.1. Menu Chính (Khi chưa đăng nhập)
* **1. Dang Nhap:** Cho phép người dùng đã có tài khoản đăng nhập vào hệ thống.
* **2. Dang Ky:** Cho phép người dùng mới tạo tài khoản.
* **0. Thoat:** Lưu dữ liệu và thoát khỏi chương trình.

### 6.2. Menu Người dùng thường (Sau khi đăng nhập thành công)
* **--- Tai Khoan ---**
    * **1. Xem Thong Tin Ca Nhan:** Hiển thị thông tin cá nhân của người dùng đang đăng nhập (tên đăng nhập, họ tên, email, SĐT, trạng thái OTP, ID Ví).
    * **2. Cap Nhat Thong Tin Ca Nhan:** Cho phép thay đổi họ tên, email, số điện thoại. Nếu OTP đã được kích hoạt, hệ thống sẽ yêu cầu mã OTP để xác nhận.
    * **3. Doi Mat Khau:** Cho phép thay đổi mật khẩu hiện tại. Cần nhập mật khẩu cũ và mật khẩu mới (2 lần).
    * **4. Thiet Lap/Quan Ly OTP:**
        * **1. Kich hoat OTP:** Tạo khóa bí mật mới, hiển thị thông tin (khóa Base32, URI) để người dùng thêm vào ứng dụng xác thực. Yêu cầu nhập mã OTP từ ứng dụng để hoàn tất kích hoạt.
        * **2. Vo hieu hoa OTP:** Yêu cầu mật khẩu hiện tại và mã OTP hiện tại để tắt chức năng OTP.
        * **0. Quay lai Menu Nguoi Dung.**
* **--- Vi Diem ---**
    * **5. Xem So Du Vi:** Hiển thị số điểm hiện có trong ví của người dùng.
    * **6. Chuyen Diem:** Cho phép chuyển điểm sang ví của người dùng khác. Cần nhập ID ví người nhận và số điểm. Nếu OTP đã kích hoạt, sẽ yêu cầu OTP để xác nhận giao dịch.
    * **7. Xem Lich Su Giao Dich:** Hiển thị danh sách các giao dịch đã thực hiện (gửi và nhận điểm), bao gồm ID giao dịch, thời gian, ví nguồn/đích, số tiền, trạng thái và mô tả.
* **0. Dang Xuat:** Đăng xuất khỏi hệ thống, quay lại Menu Chính.

### 6.3. Menu Quản trị viên (Sau khi admin đăng nhập)
Quản trị viên có tất cả các chức năng của người dùng thường đối với tài khoản của mình, cộng thêm:
* **--- Quan Ly Nguoi Dung ---**
    * **10. Liet Ke Danh Sach Nguoi Dung:** Hiển thị danh sách tất cả người dùng trong hệ thống (tên đăng nhập, họ tên, email, vai trò, trạng thái khóa).
    * **11. Tao Tai Khoan Nguoi Dung Moi:** Cho phép admin tạo tài khoản mới cho người dùng, có tùy chọn tự động sinh mật khẩu.
    * **12. Chinh Sua Thong Tin Nguoi Dung (lam ho):** Cho phép admin thay đổi thông tin (họ tên, email, SĐT) của một người dùng cụ thể. Nếu người dùng đó đã bật OTP, admin sẽ cần lấy mã OTP từ người dùng đó để xác nhận.
    * **13. Khoa/Mo Khoa Tai Khoan Nguoi Dung:** Cho phép admin khóa hoặc mở khóa một tài khoản người dùng (trừ tài khoản admin chính).
    * **14. XOA TAT CA TAI KHOAN NGUOI DUNG THUONG (CAN THUC OTP):** Chức năng nguy hiểm, xóa tất cả người dùng thường và ví của họ (giữ lại tài khoản admin và ví tổng). Yêu cầu xác nhận nhiều lần và OTP của admin nếu đã bật.
* **--- Quan Ly Vi Diem ---**
    * **23. Nap Diem Vao Vi Nguoi Dung (tu Vi Tong):** Cho phép admin chuyển điểm từ Ví Tổng vào ví của một người dùng cụ thể. Yêu cầu OTP của admin nếu đã bật.
* **0. Dang Xuat:** Đăng xuất khỏi hệ thống.

### 6.4. Lưu ý quan trọng về OTP
* Chức năng OTP trong phiên bản hiện tại sử dụng các **hàm giả mạo (stub)** cho phần mã hóa cốt lõi. Điều này có nghĩa là mặc dù quy trình thiết lập và xác minh có vẻ hoạt động, nó **KHÔNG AN TOÀN VỀ MẶT MẬT MÃ** và không nên được sử dụng trong môi trường sản xuất.
* Để có một hệ thống OTP an toàn thực sự, cần phải tích hợp đầy đủ một thư viện crypto (ví dụ: OpenSSL) và triển khai đúng các thuật toán HMAC (ví dụ: HMAC-SHA1) theo chuẩn RFC.

## 7. Tổng quan về Mã nguồn và Luồng xử lý

### 7.1. Cấu trúc Mã nguồn
Mã nguồn được tổ chức thành các module chính:
* **`main.cpp`**: Điểm vào của chương trình, điều khiển luồng chính và hiển thị menu.
* **`UserAccount` (.h, .cpp)**: Định nghĩa cấu trúc dữ liệu và các hàm liên quan đến tài khoản người dùng.
* **`Wallet` (.h, .cpp)**: Định nghĩa cấu trúc dữ liệu cho ví điểm và bản ghi giao dịch (`TransactionRecord`).
* **`DataStorage` (.h, .cpp)**: Quản lý việc đọc/ghi dữ liệu người dùng và ví từ/tới các tệp JSON.
* **`AuthService` (.h, .cpp)**: Xử lý logic nghiệp vụ liên quan đến xác thực (đăng ký, đăng nhập, đổi mật khẩu) và quản lý OTP (sử dụng stub của thư viện COTP).
* **`WalletService` (.h, .cpp)**: Xử lý logic nghiệp vụ liên quan đến ví (xem số dư, chuyển điểm, xem lịch sử, nạp điểm).
* **`Utils` (.h, .cpp)**: Chứa các hàm tiện ích chung (tạo ID, hash mật khẩu demo, xóa màn hình, tạm dừng).
* **`Constants.h`**: Định nghĩa các hằng số (đường dẫn tệp, ngưỡng đăng nhập sai, v.v.).

### 7.2. Luồng Input/Output và Xử lý chính
* **Input:**
    * Người dùng tương tác với chương trình thông qua giao diện dòng lệnh (CLI) bằng cách nhập các lựa chọn số từ menu và cung cấp dữ liệu (tên đăng nhập, mật khẩu, thông tin cá nhân, số điểm, v.v.) khi được yêu cầu.
    * Dữ liệu người dùng và ví được đọc từ các tệp JSON (`data/users.json`, `data/wallets.json`) khi chương trình khởi động.
* **Output:**
    * Các menu, thông báo, kết quả xử lý và thông tin người dùng/ví được hiển thị ra màn hình console.
    * Dữ liệu người dùng và ví sau khi thay đổi (đăng ký, cập nhật, giao dịch) được lưu lại vào các tệp JSON tương ứng.
* **Thủ tục xử lý chính:**
    1.  **Khởi tạo:** Chương trình khởi tạo `DataStorage` để tải dữ liệu từ file. Nếu là lần chạy đầu hoặc không có dữ liệu admin/ví tổng, chúng sẽ được tạo mặc định.
    2.  **Vòng lặp Menu chính:** Hiển thị menu chính (Đăng nhập, Đăng ký, Thoát).
    3.  **Xác thực:**
        * Nếu đăng ký: Thu thập thông tin, tạo `UserAccount` và `Wallet` mới, lưu vào `DataStorage`.
        * Nếu đăng nhập: Xác thực thông tin, nếu thành công, chuyển đến menu người dùng tương ứng (User hoặc Admin).
    4.  **Vòng lặp Menu người dùng/admin:**
        * Hiển thị các lựa chọn chức năng.
        * Nhận lựa chọn từ người dùng.
        * Gọi các hàm xử lý tương ứng trong `AuthService` hoặc `WalletService`.
        * Các "Service" này sẽ tương tác với `DataStorage` để đọc/ghi dữ liệu.
        * Đối với các thao tác nhạy cảm (cập nhật thông tin, chuyển điểm, xóa dữ liệu), yêu cầu xác thực OTP (sử dụng stub).
        * Hiển thị kết quả, tạm dừng, sau đó hiển thị lại menu.
    5.  **Đăng xuất:** Xóa thông tin người dùng hiện tại, quay lại Menu chính.
    6.  **Thoát:** Lưu tất cả dữ liệu hiện tại trong `DataStorage` xuống file trước khi kết thúc chương trình.

### 7.3. Chú thích trong Mã nguồn
Mã nguồn trong các tệp `.cpp` và `.h` được cung cấp với các chú thích giải thích mục đích của các lớp, hàm và các đoạn mã quan trọng để người đọc có thể hiểu rõ hơn về hoạt động của chương trình.

## 8. Tài liệu tham khảo

* **Thư viện nlohmann/json:** [https://github.com/nlohmann/json](https://github.com/nlohmann/json)
* **Tính chất ACID cho giao dịch:** [https://200lab.io/blog/acid-la-gi/](https://200lab.io/blog/acid-la-gi/)

