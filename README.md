# Hệ thống Đăng nhập và Quản lý Ví Điểm Thưởng

## 1. Giới thiệu dự án

Dự án này là một ứng dụng dòng lệnh (CLI) được viết bằng C++, mô phỏng một hệ thống đăng nhập người dùng cơ bản kết hợp với chức năng quản lý ví điểm thưởng. Người dùng có thể đăng ký, đăng nhập, quản lý thông tin cá nhân và thực hiện các giao dịch chuyển điểm giữa các ví. Hệ thống cũng bao gồm vai trò quản trị viên với các quyền hạn được mở rộng để quản lý người dùng và hệ thống điểm.

Dự án được thiết kế với mục đích học tập và minh họa các khái niệm cơ bản trong phát triển phần mềm như quản lý người dùng, bảo mật (ở mức độ cơ bản), quản lý giao dịch và lưu trữ dữ liệu.

## 2. Thành viên tham gia dự án

* *Bùi Ngọc Sơn - Phân tích yêu cầu, Thiết kế chính, Review code*
* *Nguyễn Đức Bình - hoàn thiện 2 hàm để tạo dựng dự án
## 3. Phân tích và Đặc tả Chức năng

Hệ thống bao gồm các chức năng chính sau:

* **Quản lý Tài khoản Người dùng:**
    * Đăng ký tài khoản mới (người dùng tự đăng ký hoặc admin tạo hộ).
    * Đăng nhập vào hệ thống bằng tên đăng nhập và mật khẩu.
    * Thay đổi mật khẩu cá nhân.
    * Cập nhật thông tin cá nhân (họ tên, email, số điện thoại) với xác thực OTP (phiên bản demo).
    * Yêu cầu đổi mật khẩu ngay lần đăng nhập đầu tiên nếu mật khẩu được quản trị viên tạo tự động.
* **Bảo mật:**
    * Lưu trữ mật khẩu đã được băm (sử dụng salt, phiên bản demo, **không an toàn cho sản xuất**).
    * Cơ chế OTP (One Time Password - phiên bản demo) để xác thực các thao tác quan trọng như cập nhật thông tin, giao dịch.
    * Khóa tài khoản sau một số lần đăng nhập thất bại.
* **Quản lý Ví điểm thưởng:**
    * Mỗi người dùng có một ví điểm duy nhất được liên kết với tài khoản.
    * Xem số dư điểm hiện tại trong ví.
    * Thực hiện giao dịch chuyển điểm sang ví của người dùng khác, có xác thực OTP (phiên bản demo).
    * Xem lịch sử các giao dịch đã thực hiện (gửi và nhận điểm).
    * Ví Tổng (Master Wallet) là nguồn cung cấp điểm cho hệ thống.
* **Phân quyền Người dùng và Chức năng Quản trị viên:**
    * **Người dùng thường:** Quản lý thông tin cá nhân và ví điểm của mình.
    * **Quản trị viên (Admin):**
        * Thực hiện tất cả các chức năng của người dùng thường đối với tài khoản của mình.
        * Xem danh sách tất cả người dùng.
        * Tạo tài khoản mới cho người dùng.
        * Chỉnh sửa thông tin tài khoản của người dùng khác (cần OTP xác nhận từ chủ tài khoản).
        * Khóa hoặc mở khóa tài khoản người dùng.
        * Nạp điểm vào ví của người dùng từ Ví Tổng.
* **Lưu trữ Dữ liệu:**
    * Thông tin tài khoản người dùng được lưu trong tệp `data/users.json`.
    * Thông tin ví và lịch sử giao dịch cơ bản của ví được lưu trong tệp `data/wallets.json`.
    * Chương trình sẽ tự động tạo thư mục `data/` và các tệp dữ liệu nếu chúng chưa tồn tại.

Chi tiết đầy đủ các chức năng được mô tả trong yêu cầu ban đầu của dự án và được thể hiện qua cấu trúc mã nguồn.

## 4. Tải, Dịch và Cài đặt

### 4.1. Mã nguồn
Mã nguồn của dự án được tổ chức trong thư mục `src/`.

### 4.2. Cấu trúc tệp tin chính
* `src/main.cpp`: Điểm vào chính của chương trình, chứa vòng lặp menu và điều hướng chính.
* `src/UserAccount.h`, `src/UserAccount.cpp`: Định nghĩa lớp `UserAccount` quản lý thông tin người dùng.
* `src/Wallet.h`, `src/Wallet.cpp`: Định nghĩa lớp `Wallet` (ví điểm) và cấu trúc `TransactionRecord` (bản ghi giao dịch).
* `src/AuthService.h`, `src/AuthService.cpp`: Chứa logic nghiệp vụ liên quan đến xác thực, đăng ký, quản lý mật khẩu và OTP.
* `src/WalletService.h`, `src/WalletService.cpp`: Chứa logic nghiệp vụ liên quan đến quản lý ví, xem số dư, chuyển điểm.
* `src/DataStorage.h`, `src/DataStorage.cpp`: Xử lý việc đọc và ghi dữ liệu người dùng, ví từ các tệp JSON.
* `src/Utils.h`, `src/Utils.cpp`: Chứa các hàm tiện ích chung (ví dụ: tạo ID, hash mật khẩu demo, OTP demo).
* `src/Constants.h`: Định nghĩa các hằng số sử dụng trong dự án (tên tệp, ngưỡng đăng nhập sai).
* `json.hpp`: Tệp header duy nhất của thư viện nlohmann/json.
* `Makefile`: Tệp kịch bản giúp tự động hóa quá trình biên dịch.
* `data/` (thư mục được tạo tự động):
    * `users.json`: Lưu trữ danh sách tài khoản người dùng.
    * `wallets.json`: Lưu trữ danh sách các ví và lịch sử giao dịch của chúng.
