CC := gcc									# Compiler
CFLAGS := -Wall -Wextra -Wextra	-pthread	# Compiler flags
SRCS :=  $(wildcard *.c)									# Source files
OBJS := $(SRCS:.c=.o)
TARGET := philo

# BUILD RULES
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
