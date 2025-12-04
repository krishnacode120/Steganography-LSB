# Compiler
CC = gcc

# Flags
CFLAGS = -Wall -Wextra

# Target executable
TARGET = lsb_steg

# Source files
SRCS = lsb_steg.c encode.c decode.c common.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default build
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compilation rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild from scratch
rebuild: clean all
