# 定义编译器
CC = gcc

# 定以编译器标志
CFLAGS = -O0

# 通过wildcard函数回去当前目录下的所有.c文件
SRC = $(wildcard *.c)
$(info 源文件: $(SRC))

# 根据.c文件生成对应的可执行文件名字

EXEC = $(SRC:.c=)
$(info 可执行文件: $(EXEC))

# 默认目标
all: $(EXEC)

# 生成每个可执行文件的规则
# 使用%进行模式匹配
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

# 清理生成的文件
clean:
	rm -rf $(EXEC)