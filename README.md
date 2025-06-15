# 🔐 Data Compressor
**Data Compressor** là một hệ thống nén dữ liệu chuyên sâu, tập trung vào **lưu trữ siêu tối ưu** cho cloud, backup, long-term archive hoặc hệ thống AI yêu cầu tiết kiệm không gian lưu trữ tối đa.
## 🚀 Tính năng 

- 📦 Nén bằng các thuật toán mạnh nhất thế giới: **ZSTD, PAQ8PX, CMIX, ZPAQ**
- 📈 Hiển thị tiến độ (progress) khi nén/giải nén
- 🧠 Dành cho người làm AI, lưu trữ logs, backup, xử lý dữ liệu lớn
- ☁️ Tối ưu cho cloud storage hoặc archival systems

## 📊 Benchmark thực tế (sample.txt, 10MB)

| Thuật toán | Kích thước sau nén | Tốc độ nén | Tốc độ giải nén | Ghi chú |
|------------|--------------------|------------|------------------|--------|
| ZSTD -19   | ~3.2 MB            | ⚡ Rất nhanh | ⚡ Rất nhanh     | Tốt cho backup/Cloud |
| PAQ8PX     | ~2.4 MB            | 🐢 Chậm     | 🐢 Chậm          | Tốt nhất cho văn bản |
| CMIX       | ~2.2 MB            | 🐌 Rất chậm | 🐌 Rất chậm      | Tỷ lệ cao nhất, cần RAM lớn |
| ZPAQ -m5   | ~2.6 MB            | 🐌 Chậm     | 🐢 Trung bình    | Hỗ trợ incremental archive |

> 📝 *Test trên CPU Intel i7 12th gen, RAM 16GB, Ubuntu 22.04*

## 🐳 Cài đặt & Build Docker Image
```bash
git clone https://github.com/ai-maxmind/data-compressor.git
cd data-compressor
docker build -t data_compressor .
```
🧠 Yêu cầu: Docker đã được cài đặt (trên Linux, macOS, WSL hoặc Windows)
## ▶️ Cách sử dụng với Docker
### 📦 Nén một file
```bash
# Nén bằng zstd
docker run --rm -v $(pwd):/data data_compressor compress zstd /data/input.txt /data/input.zst

# Nén bằng paq8px
docker run --rm -v $(pwd):/data data_compressor compress paq8px /data/data.txt /data/data.paq8px

# Nén bằng cmix (siêu mạnh nhưng chậm)
docker run --rm -v $(pwd):/data data_compressor compress cmix /data/data.txt /data/data.cmix

# Nén bằng zpaq
docker run --rm -v $(pwd):/data data_compressor compress zpaq /data/input.txt /data/archive.zpaq
```

Nếu thư mục chứa file **input.txt**, chạy câu lệnh trên sẽ:
+ Chạy container **data_compressor**
+ Nén file **input.txt** bằng thuật toán đã chọn
+ Tạo ra file nén trong cùng thư mục

Lưu ý: Các thuật toán như **zstd**, **paq8px**, **cmix**, **zpaq** chỉ hỗ trợ nén 1 file, không hỗ trợ nén nhiều file cùng lúc hoặc thư mục. Vì vậy, gộp nhiều file thành một .tar file rồi nén là cách hiệu quả để tận dụng các thuật toán nén mạnh.

```bash
# ZSTD
docker run --rm -v $(pwd):/data data_compressor compress zstd /data/data_bundle.tar /data/data_bundle.tar.zst

# PAQ8PX
docker run --rm -v $(pwd):/data data_compressor compress paq8px /data/data_bundle.tar /data/data_bundle.tar.paq8px

# CMIX
docker run --rm -v $(pwd):/data data_compressor compress cmix /data/data_bundle.tar /data/data_bundle.tar.cmix

# ZPAQ
docker run --rm -v $(pwd):/data data_compressor compress zpaq /data/data_bundle.tar /data/data_bundle.tar.zpaq

```
### 📦 Giải nén một file
Cú pháp chung: 
```bash
docker run --rm -v $(pwd):/data data_compressor decompress <thuật_toán> /data/<file_nén>
```

## 📌 Ghi chú
+ Các thuật toán như **PAQ8PX**, **CMIX**, **ZPAQ** yêu cầu RAM lớn, đặc biệt CMIX có thể cần > 8GB RAM.
+ **ZSTD** là tốt nhất nếu bạn cần nén mạnh và tốc độ tốt.


