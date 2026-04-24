NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRCS = main.c \
       init.c \
       routine.c \
       queue.c \
       utils.c \
       monitor.c
OBJS = $(SRCS:.c=.o)
HEADERS = codexion.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re