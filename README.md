


# 🕹️ Modern Tetris C++ (Windows Console Edition)

Một phiên bản trò chơi **Tetris cổ điển tích hợp đầy đủ tính năng hiện đại**, được phát triển bằng ngôn ngữ **C++**. Trò chơi vận hành mượt mượt mà trực tiếp trên **Windows Console**, sử dụng mã màu ANSI để mang lại giao diện sống động, trực quan mà không cần cài đặt các thư viện đồ họa bên thứ ba phức tạp.

---
## Thành viên
- Bùi Nguyễn Quốc Trọng 24521862
-  Đinh Văn Dũng 25520351
-  Võ Hoàng Gia 25520460
-  Trần Ngọc Hiếu 25520559
-  Phan Thanh Huy 25520730
---

## 📖 Giới Thiệu Trò Chơi

Dự án này tái hiện lại tựa game giải đố xếp gạch kinh điển Tetris. Người chơi sẽ điều khiển các khối gạch rơi từ đỉnh màn hình, xoay và sắp xếp chúng vào ma trận tiêu chuẩn để lấp đầy các hàng ngang. Khi hàng ngang được lấp đầy, chúng sẽ biến mất, mang lại điểm số và giúp người chơi tiếp tục sinh tồn trước khi các khối gạch bị chồng quá cao dẫn đến `Game Over`.

---

## ✨ Các Tính Năng Hiện Đại Tích Hợp

Không dừng lại ở việc xếp gạch thô sơ, phiên bản này sở hữu đầy đủ các cơ chế bổ trợ chuẩn của một tựa game Tetris hiện đại:

*   **Hệ thống 7 Khối gạch Chuẩn (Tetrominoes):** Đầy đủ các khối **I, O, T, S, Z, J, L** với màu sắc ANSI riêng biệt, rất dễ nhận diện.
*   **Bóng Mờ Dự Đoán (Ghost Piece):** Hiển thị vùng bóng mờ ở đáy màn hình, giúp người chơi biết chính xác vị trí khối gạch sẽ đáp xuống để ra quyết định lập tức ở tốc độ cao.
*   **Lưu Giữ Khối (Hold Piece):** Tính năng cho phép tạm cất một khối gạch rắc rối đi hoặc để dành một thanh dài (khối I) cho những pha ăn 4 hàng chiến thuật.
*   **Hàng Đợi Xem Trước (Next Queue):** Hiển thị liên tục 3 khối gạch tiếp theo chuẩn bị xuất hiện để người chơi chủ động lên kế hoạch sắp xếp.
*   **Tốc Độ Thách Thức (Dynamic Speed):** Càng ghi được nhiều điểm và dọn được nhiều hàng, cấp độ (`Level`) sẽ tăng lên, đồng thời tốc độ rơi tự động đẩy nhanh.

---

## 🕹️ Hệ Thống Điều Khiển (Controls)

Giao diện điều khiển bằng bàn phím máy tính cực kỳ nhạy và dễ thao tác:

*   `⬅️` / `➡️` (Mũi tên Trái / Phải): Di chuyển khối sang hai bên.
*   `⬆️` (Mũi tên Lên): Xoay khối 90 độ để tìm khớp nối hoàn hảo.
*   `⬇️` (Mũi tên Xuống): Thả nhanh (Soft Drop) có kiểm soát.
*   `Spacebar` (Phím cách): Thả lập tức (Hard Drop) – cắm thẳng khối xuống đáy và khóa cứng ngay.
*   `C`: Lưu giữ hoặc hoán đổi khối gạch hiện tại (Hold).
*   `P`: Tạm dừng trò chơi (`PAUSED`).
*   `Q`: Thoát trò chơi.

---
