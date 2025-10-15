# **************************************************************************** # # ::::::::              #
#    Makefile                                          :+:    :+:              #
#                                                     +:+                      #
#    By: avaliull <avaliull@student.codam.nl>        +#+                       #
#                                                   +#+                        #
#    Created: 2025/05/21 19:45:55 by avaliull     #+#    #+#                   #
#    Updated: 2025/10/07 16:19:21 by avaliull     ########   odam.nl           #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all
NAME	=	philo
CFILES	=	philo.c\
			setup_variables.c\
			philo_threads.c\
			philo_states.c\
			fork_management.c\
			time_tracking.c\
			panopticon.c\
			logger_loop.c\
			message_buffer.c\
			utils.c\
			exit.c\
			test_funcs.c

OFILES	= $(addprefix $(BUILDDIR),$(CFILES:.c=.o))
DEPFILES	= $(addprefix $(BUILDDIR),$(CFILES:.c=.d))

VPATH	= $(INCLUDE) $(SRCDIRS)
BUILDDIR = build/
SRCDIR = src/
INCDIR = inc/
SRCDIRS = $(SRCDIR)

$(SRCDIR):
	mkdir -p $@
$(INCDIR):
	mkdir -p $@
$(BUILDDIR):
	mkdir -p $@

INCLUDE = $(INCDIR)

RM	= rm -rf
CC	= cc

CPPFLAGS	= $(INCFLAGS) -MMD -MP

INCFLAGS	= $(addprefix -I,$(INCLUDE))
CFLAGS	= -Wall -Wextra -Werror -fsanitize=undefined
INPUT	= 256 400 100 100 10


-include $(OFILES:.o=.d)

# builds .d files, then builds .o files based on .d.
# skips files that weren't changed (see CPPFLAGS)
$(BUILDDIR)%.o: %.c $(INCLUDE) | $(BUILDDIR)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OFILES)
	$(CC) $(CFLAGS) -o $@ $(OFILES) $(INCFLAGS)

#Base/project requirements
all: $(NAME)
clean:
	$(RM) $(OFILES)
fclean:	clean
	$(RM) $(NAME) $(DEPFILES)
re:	fclean all

#LSP connection for neovim
clangd:
	$(MAKE) fclean
	intercept-build-14 $(MAKE)
bear:
	$(MAKE) fclean
	bear -- $(MAKE)

#debugging
debug: CPPFLAGS += -g
debug: clean $(NAME)
gdb: fclean debug
	gdb -tui ./$(NAME)
test: $(NAME) run
run:
	./$(NAME) $(INPUT)
leak:
	$(MAKE) -s debug
	valgrind --track-fds=yes --track-origins=yes \
	-s --leak-check=full --show-leak-kinds=all ./$(NAME) $(INPUT)
val:
	valgrind --track-fds=yes --track-origins=yes \
	-s --leak-check=full --show-leak-kinds=all ./$(NAME) $(INPUT)
hel:
	$(MAKE) -s debug
	valgrind --tool=helgrind --track-lockorders=yes ./$(NAME) $(INPUT)

.PHONY:	clangd all clean fclean re test run leak debug gdb
