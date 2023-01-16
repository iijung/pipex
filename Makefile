# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minjungk <minjungk@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/29 06:04:57 by minjungk          #+#    #+#              #
#    Updated: 2023/01/16 17:28:37 by minjungk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_ON_ERROR:
.DEFAULT_GOAL := all

CFLAGS = -Wall -Wextra -Werror -MMD -MP -Weverything
CPPFLAGS = -I./libft
LDFLAGS = -L./libft
LDLIBS = -lft

ifdef DEBUG
	CFLAGS += -O0 -g -fsanitize=address,undefined
	LDFLAGS += -fsanitize=address,undefined
endif

# **************************************************************************** #
# dependency
# **************************************************************************** #

LIBFT = libft/libft.a

$(LIBFT):
	$(MAKE) -C $(@D)

# **************************************************************************** #
# pipex
# **************************************************************************** #

PIPEX = pipex

PIPEX_SRCS = \
		pipex.c \

PIPEX_OBJS = $(PIPEX_SRCS:.c=.o)
PIPEX_DEPS = $(PIPEX_SRCS:.c=.d)
-include $(PIPEX_DEPS)

$(PIPEX): $(PIPEX_OBJS)
$(PIPEX_OBJS): $(LIBFT)

# **************************************************************************** #
# main
# **************************************************************************** #

all bonus:
	$(MAKE) -C $(dir $(LIBFT))
	$(MAKE) $(PIPEX)

clean:
	$(MAKE) -C $(dir $(LIBFT)) clean
	$(RM) $(wildcard *.o) $(wildcard *.d)

fclean: clean
	$(RM) $(LIBFT) $(PIPEX)

re: fclean
	$(MAKE)

.PHONY: all clean fclean re bonus $(dir $(LIBFT))
