# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kgale <kgale@student.21-school.ru>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/06/17 13:10:53 by lmartin           #+#    #+#              #
#    Updated: 2021/07/12 19:51:39 by kgale            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			=	gcc
CC_FLAGS	=	-Wall -Wextra -Werror
RM			=	rm -rf
DIR_HEADERS =	./includes/
DIR_SRCS	=	./srcs/
SRCS			=	philosophers.c \
					params_init.c \
					philosophers_init.c \
					simulation.c \
					simulation_helpers.c
OBJS 		=	$(SRCS:%.c=$(DIR_OBJS)%.o)
NAME 		=	philo

all:			$(NAME)

$(NAME):		$(OBJS)
				@$(CC) $(CC_FLAGS) -v -I $(DIR_HEADERS) $(OBJS) -o $(NAME) -lpthread

$(OBJS):		| $(DIR_OBJS)


$(DIR_OBJS)%.o: $(DIR_SRCS)%.c
				@$(CC) $(CC_FLAGS) -I $(DIR_HEADERS) -c $< -o $@

$(DIR_OBJS):
				@mkdir $(DIR_OBJS)

clean:
				@$(RM) $(DIR_OBJS)

fclean:			clean
				@$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re
