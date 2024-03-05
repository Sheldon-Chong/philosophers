CC := gcc									# Compiler
CFLAGS := -pthread -Wall -Werror -Wextra -fsanitize=thread 	# Compiler flags
SRCS :=  $(wildcard *.c)					# Source files
OBJS := $(SRCS:.c=.o)						# Object files
TARGET := philo_bonus						# Executable name

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean:
	rm -f $(OBJS) $(TARGET)

re: fclean all

.PHONY: clean fclean re all