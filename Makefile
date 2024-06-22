# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minjungk <minjungk@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/29 06:04:57 by minjungk          #+#    #+#              #
#    Updated: 2024/06/22 23:50:20 by minjungk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DELETE_ON_ERROR:
.DEFAULT_GOAL := all

CPPFLAGS	+= -MMD -MP
CFLAGS		+= -Wall -Wextra -Werror -O2

ifdef DEBUG
CFLAGS		+= -g -fsanitize=address,undefined
LDFLAGS		+= -fsanitize=address,undefined
endif

# **************************************************************************** #
# dependency
# **************************************************************************** #

LIBS = \
	external/libexternal.a \

CPPFLAGS	+= $(foreach dir, $(dir $(LIBS)), -I$(dir))
LDFLAGS 	+= $(foreach dir, $(dir $(LIBS)), -L$(dir))
LDLIBS  	+= $(foreach lib, $(notdir $(LIBS)), -l$(patsubst lib%.a,%,$(lib)))

$(LIBS):
	$(MAKE) -C $(@D)

# **************************************************************************** #
# push_swap
# **************************************************************************** #


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

$(PIPEX_OBJS): $(LIBS)
$(PIPEX): $(PIPEX_OBJS)
	$(info $(PIPEX_SRCS))
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
else
PIPEX_SRCS := $(COMMON_SRCS) $(MANDATORY_SRCS)
PIPEX_OBJS := $(PIPEX_SRCS:.c=.o)
PIPEX_DEPS := $(PIPEX_SRCS:.c=.d)
-include $(PIPEX_DEPS)

$(PIPEX_OBJS): $(LIBS)
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
	$(MAKE) -C $(dir $(LIBS)) clean
	$(RM) $(wildcard *.o) $(wildcard *.d)

fclean: clean
	$(RM) $(LIBS) $(PIPEX)

re: fclean
	$(MAKE)

.PHONY: all clean fclean re bonus $(dir $(LIBS))
