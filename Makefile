# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minjungk <minjungk@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/29 06:04:57 by minjungk          #+#    #+#              #
#    Updated: 2023/05/05 03:48:59 by minjungk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DELETE_ON_ERROR:
.DEFAULT_GOAL := all

CFLAGS = -Wall -Wextra -Werror -MMD -MP
CPPFLAGS = -I./libft -I./src
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

COMMON_SRCS = \
	replace_binary.c \
	init_pipex.c \
	pipex.c \

MANDATORY_SRCS = \
	mandatory.c \

BONUS_SRCS = \
	heredoc.c \
	bonus.c \

ifdef WITH_BONUS
PIPEX_SRCS := $(COMMON_SRCS) $(BONUS_SRCS)
PIPEX_OBJS := $(PIPEX_SRCS:.c=.o)
PIPEX_DEPS := $(PIPEX_SRCS:.c=.d)
-include $(PIPEX_DEPS)

$(PIPEX_OBJS): $(LIBFT)
$(PIPEX): $(PIPEX_OBJS)
	$(info $(PIPEX_SRCS))
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
else
PIPEX_SRCS := $(COMMON_SRCS) $(MANDATORY_SRCS)
PIPEX_OBJS := $(PIPEX_SRCS:.c=.o)
PIPEX_DEPS := $(PIPEX_SRCS:.c=.d)
-include $(PIPEX_DEPS)

$(PIPEX_OBJS): $(LIBFT)
$(PIPEX): $(PIPEX_OBJS)
	$(info $(PIPEX_SRCS))
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
endif

# **************************************************************************** #
# main
# **************************************************************************** #

all:
	$(MAKE) $(PIPEX)

bonus:
	$(MAKE) WITH_BONUS=1 $(PIPEX)

clean:
	$(MAKE) -C $(dir $(LIBFT)) clean
	$(RM) $(wildcard *.o) $(wildcard *.d)

fclean: clean
	$(RM) $(LIBFT) $(PIPEX)

re: fclean
	$(MAKE)

.PHONY: all clean fclean re bonus $(dir $(LIBFT))
