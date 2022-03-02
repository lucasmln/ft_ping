# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/28 19:46:42 by lmoulin           #+#    #+#              #
#    Updated: 2022/02/28 19:51:18 by lmoulin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ping

SRC = ./main.c ./get_signal.c ./packet.c

CFLAGS = #-Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME) : $(OBJ)
	make -C libft
	gcc $(CFLAGS) $(OBJ) libft/libft.a -o $(NAME)
	#setcap cap_net_raw+ep ft_ping

clean :
	make -C libft clean
	rm -f *.o

fclean :
	make -C libft fclean
	rm -f *.o
	rm ft_ping

re :
	make fclean
	make
