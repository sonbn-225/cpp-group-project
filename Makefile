CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I../ # -I../ de tim json.hpp neu ban dat json.hpp cung cap voi thu muc src
LDFLAGS =
RM = rm -f

# Thư mục chứa mã nguồn
SRCDIR = src
# Thư mục chứa file object
OBJDIR = obj
# Thư mục chứa file thực thi
BINDIR = bin

# Tìm tất cả các file .cpp trong SRCDIR
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# Tạo danh sách file object tương ứng trong OBJDIR
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
# Tên file thực thi
EXECUTABLE = $(BINDIR)/login_wallet_system

# Target mặc định
all: $(EXECUTABLE)

# Rule để tạo file thực thi
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Bien dich hoan tat! File thuc thi: $(EXECUTABLE)"

# Rule để tạo file object từ file .cpp
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dọn dẹp
clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
	$(RM) -r $(OBJDIR) $(BINDIR) data/*
	@echo "Da don dep!"

# Rule để chạy (tùy chọn)
run: all
	./$(EXECUTABLE)

.PHONY: all clean run