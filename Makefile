# **************************************************************************** #
#                                                                              #
#                                                        ::::::::              #
#    Makefile                                          :+:    :+:              #
#                                                     +:+                      #
#    By: avaliull <avaliull@student.codam.nl>        +#+                       #
#                                                   +#+                        #
#    Created: 2025/05/21 19:45:55 by avaliull     #+#    #+#                   #
#    Updated: 2025/05/30 15:43:02 by avaliull     ########   odam.nl           #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all
MAKEFLAGS =

NAME	=	philo
CFILES	=	philo.c\
			utils.c\
			exit.c\
			test_funcs.c

OFILES	= $(addprefix $(OBJDIR),$(CFILES:.c=.o))

VPATH	= $(INCLUDE) $(SRCDIRS)
OBJDIR = obj/
SRCDIR = src/
INCDIR = inc/
SRCDIRS = $(addprefix $(SRCDIR), rendering controls init_exit\
		  coordinate_manipulation map_manipulation) $(SRCDIR)

$(INCDIR):
	mkdir -p $@

INCLUDE = $(INCDIR)

RM	= rm -rf
CC	= cc
CFLAGS	= -Wall -Wextra -Werror
INPUT	= 4 100 100 100 -1

#chec kthis
#include $(OBJ:.o=.d)
#
#$(DEPDIR)/%.d	: %.c
#	$(CC) $(CPPFLAG) $(INCLUDE) -MM -MF $@ -MT $(OBJDIR)/$(addsuffix .o,$(basename $<)) $<
#
$(OBJDIR):
	mkdir -p $@
$(OBJDIR)%.o: %.c $(INCLUDE) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(addprefix -I,$(INCLUDE))

$(NAME): $(OFILES)
	$(CC) $(CFLAGS) -o $@\
		$(OFILES) $(addprefix -I,$(INCLUDE))

#Base/project requirements
all: $(NAME)
clean:
	$(RM) $(OFILES)
fclean:	clean
	$(RM) $(NAME)
re:	fclean all

#LSP connection for neovim
clangd:
	$(MAKE) fclean
	intercept-build-14 make all

#debugging
debug: CFLAGS += -g
debug: clean $(NAME)
gdb: fclean debug
	gdb ./$(NAME)
test: clean $(NAME) run
run:
	./$(NAME) $(INPUT)
leak:	debug
	valgrind  --suppressions=MLX42.supp -s --leak-check=full \
	--show-leak-kinds=all --track-fds=yes ./$(NAME) $(INPUT)
val:
	valgrind  --suppressions=MLX42.supp -s --leak-check=full \
	--show-leak-kinds=all --track-fds=yes ./$(NAME) $(INPUT)

.PHONY:	clangd all clean fclean re test run leak debug gdb
