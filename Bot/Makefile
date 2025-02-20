NAME		= AwesomeBot
	
CC			= c++
FLAGS		= -Wall -Wextra -Werror -std=c++98
RM			= rm -rf

OBJDIR =	 .objs

FILES		= 	main			\
				parseInput		\
				serverUtils		\
				parsingUtils	\
				Joke			\
				funFact			\
				api				\
				BotCommands		

HFILES		= 	Bot

HEADER		= $(HFILES:=.hpp)
SRCS		= $(FILES:=.cpp)
OBJS		= $(addprefix $(OBJDIR)/, $(FILES:=.o))

PURPLE		= 	\e[33;1;35m
GREEN		= 	\e[33;1;32m
BLUE		= 	\e[33;1;34m
RED			=	\e[33;1;31m
WHITEBOLD	=	\e[33;1;37m
RESET		=	\e[0m

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
	@printf "$(WHITEBOLD)The $(PURPLE)\"$(NAME)\" $(WHITEBOLD)program is ready to be executed.$(RESET)\n"

silent: $(OBJS) $(HEADER)
		@$(CC) $(OBJS) $(FLAGS) -o $(NAME)
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
	@$(RM) $(OBJDIR)
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