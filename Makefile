NAME		= ircserv
	
CC			= c++
FLAGS		= -Wall -Wextra -Werror -std=c++98
RM			= rm -rf

OBJDIR =	 .objFiles

FILES		= 	main \
				server channel command client signal \
				parseLoginData clientChannelManagement \
				Commands/KICK Commands/MODE Commands/NICK Commands/PART Commands/PASS \
				Commands/QUIT Commands/TOPIC Commands/USER Commands/PRIVMSG Commands/INVITE Commands/JOIN

HFILES		= 	Headers/channel Headers/client Headers/command Headers/ft_irc Headers/RPL Headers/server

HEADER		= $(HFILES:=.hpp)
SRCS		= $(FILES:=.cpp)
OBJS		= $(addprefix $(OBJDIR)/, $(FILES:=.o))

PURPLE		= 	\e[33;1;35m
GREEN		= 	\e[33;1;32m
BLUE		= 	\e[33;1;34m
RED			=	\e[33;1;31m
WHITEBOLD	=	\e[33;1;37m
RESET		=	\e[0m

LAST_COMPILE_MARK := .last_compile

#check si fichier a deja ete compile et si il a ete compile avant ou apres le file reference
define file_changed_since_last_compile
	[ $1 -nt $(LAST_COMPILE_MARK) ]
endef

define update_last_compile_timestamp
	touch $(LAST_COMPILE_MARK)
endef

all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	@$(CC) $(OBJS) $(FLAGS) -o $(NAME)
	@$(foreach header, $(HEADER), \
		if [ -f $(header) ]; then \
			printf "$(PURPLE)-> ./$(header)$(RESET)\n"; \
		fi; \
	)
	@$(foreach obj, $(OBJS), \
		if [ -f $(obj) ]; then \
			printf "$(GREEN)+ $(obj)$(RESET)\n"; \
		fi; \
	)
	@printf "$(BLUE)+ ./$(NAME)$(RESET)\n"
	@printf "$(WHITEBOLD)The $(RED)\"$(NAME)\" $(WHITEBOLD)program is ready to be executed.$(RESET)\n"
	@$(call update_last_compile_timestamp)

silent: $(OBJS) $(HEADER)
		@$(CC) $(OBJS) $(FLAGS) -o $(NAME)
		@$(foreach src_file, $(SRCS), \
		if $(call file_changed_since_last_compile, $(src_file)); then \
			printf "$(GREEN)+ $(src_file)$(RESET)\n"; \
		fi; \
	)
	@$(call update_last_compile_timestamp)
	@printf "$(WHITEBOLD)Program compiled$(RESET).\n"


$(OBJDIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@$(foreach obj, $(OBJS), \
		if [ -f $(obj) ]; then \
			$(RM) $(obj); \
			printf "$(RED)- $(obj)$(RESET)\n"; \
		fi; \
	)
	@$(RM) -rf $(OBJDIR) $(LAST_COMPILE_MARK)
	@printf "$(WHITEBOLD)All object files have been removed.$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(RED)- ./$(NAME) $(RESET)\n"
	@printf "$(WHITEBOLD)Executable has been removed$(RESET).\n"


run: silent
	
	./$(NAME) 6667 1

valgrind: silent
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) 6667 1

re: fclean all

.PHONY: all clean fclean re valgrind silent