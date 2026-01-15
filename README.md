# 🗺️ Traveling Salesman Problem (TSP) Solver

[![C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![SDL2](https://img.shields.io/badge/Graphics-SDL2-green.svg)](https://www.libsdl.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## 📋 Giới thiệu

Dự án **Traveling Salesman Problem (TSP)** là bài tập lớn môn Lập trình cơ sở 1 (PBL1) tại **Đại học Bách Khoa - Đại học Đà Nẵng**. Đây là một ứng dụng giải quyết bài toán người du lịch (TSP) với giao diện đồ họa sử dụng thư viện SDL2.

### 🎯 Mục tiêu dự án
- Giải quyết bài toán tối ưu hóa TSP (tìm đường đi ngắn nhất qua tất cả các thành phố)
- Trực quan hóa thuật toán và kết quả bằng giao diện đồ họa
- Áp dụng các thuật toán nhánh cận (Branch and Bound) và quy hoạch động (Dynamic Programming)

## 👥 Thành viên nhóm

**Team Leader:** Nguyễn Tiến  
**Thành viên:** [Thêm tên các thành viên khác]

## ✨ Tính năng

- ✅ Đọc dữ liệu ma trận chi phí từ file input
- ✅ Giải bài toán TSP bằng các thuật toán:
  - Branch and Bound (Nhánh cận)
  - Dynamic Programming with Bitmask
- ✅ Giao diện đồ họa SDL2 hiển thị:
  - Ma trận chi phí
  - Đường đi tối ưu
  - Chi phí nhỏ nhất
- ✅ Xuất kết quả ra file output
- ✅ Hỗ trợ tối đa 20 thành phố

## 🛠️ Công nghệ sử dụng

- **Ngôn ngữ:** C
- **Thư viện đồ họa:** SDL2, SDL2_ttf, SDL2_image
- **Build tool:** GCC, Make
- **IDE:** Visual Studio Code

## 📦 Cài đặt

### Yêu cầu hệ thống
- Windows OS
- GCC compiler (MinGW)
- SDL2 development libraries

### Các bước cài đặt

1. **Clone repository:**
```bash
git clone https://github.com/Tienes2810/Traveling-Salesman-Problem.git
cd Traveling-Salesman-Problem
```

2. **Cài đặt SDL2:**
   - Thư viện SDL2 đã được bao gồm trong thư mục `src/lib`
   - Đảm bảo các file `.dll` (SDL2.dll, SDL2_ttf.dll, SDL2_image.dll) có trong thư mục gốc

3. **Biên dịch chương trình:**
```bash
make build
```

4. **Chạy chương trình:**
```bash
make run
```

hoặc:
```bash
./main.exe
```

## 📖 Hướng dẫn sử dụng

### Format file input
Tạo file `in_put.txt` với định dạng:
```
n
c[0][0] c[0][1] ... c[0][n-1]
c[1][0] c[1][1] ... c[1][n-1]
...
c[n-1][0] c[n-1][1] ... c[n-1][n-1]
```

Trong đó:
- `n`: Số lượng thành phố (3 ≤ n ≤ 20)
- `c[i][j]`: Chi phí di chuyển từ thành phố i đến thành phố j
- `c[i][i] = 0`: Chi phí đi từ một thành phố đến chính nó = 0
- `c[i][j] = 0`: Không có đường đi trực tiếp giữa i và j

### Ví dụ input (4 thành phố):
```
4
0 10 15 20
10 0 35 25
15 35 0 30
20 25 30 0
```

### Chạy chương trình:
1. Đặt file input trong thư mục gốc
2. Chạy `./main.exe`
3. Sử dụng giao diện để chọn file input và xem kết quả
4. Kết quả sẽ được lưu vào file output

## 📊 Thuật toán

### 1. Branch and Bound (Nhánh cận)
- Tìm kiếm theo chiều sâu với chiến lược cắt tỉa
- Sử dụng giới hạn dưới (lower bound) để loại bỏ các nhánh không tối ưu
- Độ phức tạp: O(n!)

### 2. Dynamic Programming with Bitmask
- Sử dụng quy hoạch động với bitmask để lưu trạng thái đã thăm
- Công thức truy hồi: `dp[mask][i]` = chi phí nhỏ nhất khi đã thăm các thành phố trong mask và kết thúc tại i
- Độ phức tạp: O(n² × 2ⁿ)

## 📁 Cấu trúc thư mục

```
PBL1/
├── main.c                 # File chính chứa code
├── pbl1nowing.c          # Code phiên bản không GUI
├── Makefile              # Build script
├── README.md             # Documentation
├── in_put*.txt           # File input mẫu
├── out_put*.txt          # File output kết quả
├── Arial.TTF             # Font cho SDL2_ttf
├── logo.jpeg             # Logo dự án
├── SDL2*.dll             # SDL2 runtime libraries
└── src/
    ├── include/SDL2/     # SDL2 header files
    └── lib/              # SDL2 library files
```

## 🎮 Demo

Chương trình cung cấp giao diện đồ họa để:
- Chọn file input
- Hiển thị ma trận chi phí
- Visualize đường đi tối ưu
- Hiển thị tổng chi phí nhỏ nhất

## 📝 Tài liệu tham khảo

- [Traveling Salesman Problem - Wikipedia](https://en.wikipedia.org/wiki/Travelling_salesman_problem)
- [SDL2 Documentation](https://wiki.libsdl.org/)
- Giáo trình Lập trình cơ sở 1 - ĐH Bách Khoa Đà Nẵng

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Lời cảm ơn

- Giảng viên hướng dẫn: [Tên giảng viên]
- Khoa Công nghệ Thông tin - ĐH Bách Khoa Đà Nẵng
- Cộng đồng SDL2 developers

## 📧 Liên hệ

- **Email:** nguyentien281006@gmail.com
- **GitHub:** [@Tienes2810](https://github.com/Tienes2810)

---

⭐ Nếu bạn thấy dự án hữu ích, hãy cho chúng tôi một star nhé!
