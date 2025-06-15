CC = gcc
CFLAGS = -O2 -Wall -Iinclude
TARGET = data_compressor

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

PAQ8PX_REPO=https://github.com/hxim/paq8px.git
PAQ8PX_DIR=build/paq8px
CMIX_DIR=build/cmix
ZPAQ_DIR=build/zpaq

BINARIES = $(TARGET) paq8px cmix zpaq_bin

.PHONY: all clean deps build_tools check_zstd help

all: check_zstd deps $(TARGET)
	@echo "✅ Build hoàn tất!"

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

deps: build_paq8px build_cmix build_zpaq

build_paq8px:
	@if [ ! -f paq8px ]; then \
		echo "🔧 Cloning paq8px..."; \
		mkdir -p build; \
		git clone --depth 1 $(PAQ8PX_REPO) $(PAQ8PX_DIR); \
		echo "🔧 Building paq8px..."; \
		g++ -std=c++17 -O3 -march=native $$(find $(PAQ8PX_DIR) -name '*.cpp') -lz -o paq8px && strip paq8px; \
	else \
		echo "✅ paq8px đã tồn tại."; \
	fi

build_cmix:
	@if [ ! -f cmix ]; then \
		echo "🔧 Cloning and building cmix..."; \
		mkdir -p build && \
		git clone --depth=1 https://github.com/byronknoll/cmix.git $(CMIX_DIR) && \
		sed -i 's/clang++-17/g++/g' $(CMIX_DIR)/makefile && \
		$(MAKE) -C $(CMIX_DIR) || { echo '❌ Build cmix failed'; exit 1; }; \
		cp $(CMIX_DIR)/cmix ./ || { echo '❌ Copy cmix failed'; exit 1; }; \
	else \
		echo "✅ cmix đã tồn tại."; \
	fi

build_zpaq:
	@if [ ! -f zpaq_bin ]; then \
		echo "🔧 Cloning and building zpaq..."; \
		git clone --depth=1 https://github.com/zpaq/zpaq.git $(ZPAQ_DIR) || exit 1; \
		$(MAKE) -C $(ZPAQ_DIR) || exit 1; \
		cp $(ZPAQ_DIR)/zpaq ./zpaq_bin || exit 1; \
	else \
		echo "✅ zpaq đã tồn tại."; \
	fi

check_zstd:
	@command -v zstd >/dev/null 2>&1 || { echo "❌ Zstandard (zstd) chưa được cài. Vui lòng chạy: sudo apt install zstd"; exit 1; }

clean:
	rm -f $(BINARIES) src/*.o
	rm -rf $(PAQ8PX_DIR) $(CMIX_DIR) $(ZPAQ_DIR)
	@echo "🧹 Đã xóa toàn bộ file build và thư mục tạm."

help:
	@echo "🛠️ Các lệnh hỗ trợ:"
	@echo "  make             : Build toàn bộ project"
	@echo "  make clean       : Xóa tất cả file biên dịch và thư viện"
	@echo "  make deps        : Tải và build paq8px, cmix, zpaq"
	@echo "  make help        : Hiển thị trợ giúp"
