NAME = cub3D
MY_NAME = cub3D Project

SRCS = $(wildcard srcs/*.c)
OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Werror -Wextra -I includes/ -g

LIB = libft/libft.a
DEPS = $(addprefix lib/, $(LIB))

# Colors
RESET   = \033[0m
BOLD    = \033[1m
DIM     = \033[2m
RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
BLUE    = \033[34m
MAGENTA = \033[35m
CYAN    = \033[36m
WHITE   = \033[37m
BG_GREEN = \033[42m
BG_BLUE  = \033[44m

# Symbols
CHECK = ✓
ARROW = ➜
GEAR  = ⚙
ROCKET = 🚀
CLEAN = 🧹
LINK  = 🔗

# Count files for progress
TOTAL_FILES = $(words $(C_FILES))
CURRENT = 0

all: header $(NAME)
	@echo ""
	@echo "  $(BOLD)$(GREEN)$(CHECK) Build complete!$(RESET)"
	@echo "  $(DIM)Run with: ./$(NAME)$(RESET)"
	@echo ""

header:
	@echo ""
	@echo "  $(BOLD)$(MAGENTA)╔════════════════════════════════════════════════════════════════════════════╗$(RESET)"
	@echo "  $(BOLD)$(MAGENTA)║$(RESET)    $(BOLD)$(CYAN)$(ROCKET) $(MY_NAME) $(RESET)  $(DIM)by roazouan && Agallot$(RESET)     $(BOLD)$(MAGENTA)                             ║$(RESET)"
	@echo "  $(BOLD)$(MAGENTA)╚════════════════════════════════════════════════════════════════════════════╝$(RESET)"
	@echo ""

$(NAME): libs $(OBJS)
	@echo ""
	@echo "  $(BOLD)$(YELLOW)$(LINK) Linking...$(RESET)"
	@cc $(CFLAGS) $(OBJS) -o $(NAME) $(DEPS) -lreadline
	@echo "  $(GREEN)$(CHECK)$(RESET) $(BOLD)$(NAME)$(RESET) created"

libs:
	@echo "  $(BOLD)$(BLUE)$(GEAR) Building libraries...$(RESET)"
	@$(MAKE) --no-print-directory -C lib/libft > /dev/null 
	@echo "  $(GREEN)$(CHECK)$(RESET) libft"
	@echo ""
	@echo "  $(BOLD)$(BLUE)$(GEAR) Compiling source files...$(RESET)"

srcs/%.o: srcs/%.c
	@cc $(CFLAGS) -c $< -o $@
	@printf "  $(GREEN)$(CHECK)$(RESET) %s\n" $(notdir $<)

clean:
	@echo ""
	@echo "  $(BOLD)$(YELLOW)$(CLEAN) Cleaning...$(RESET)"
	@rm -f $(OBJS)
	@$(MAKE) --no-print-directory -C lib/libft fclean > /dev/null 
	@echo ""

fclean: clean
	@rm -rf $(NAME)
	@echo "  $(GREEN)$(CHECK)$(RESET) $(NAME) removed"
	@echo ""

re: fclean all

# Debug mode - shows all compilation commands
debug: header debug_libs debug_srcs
	@echo ""
	@echo "  $(BOLD)$(YELLOW)$(LINK) Linking...$(RESET)"
	cc $(CFLAGS) $(OBJS) -o $(NAME) $(DEPS) -lreadline
	@echo ""
	@echo "  $(BOLD)$(GREEN)$(CHECK) Debug build complete!$(RESET)"
	@echo ""

debug_libs:
	@echo "  $(BOLD)$(RED)🔧 DEBUG MODE - Verbose output$(RESET)"
	@echo ""
	@echo "  $(BOLD)$(BLUE)$(GEAR) Building libft...$(RESET)"
	$(MAKE) -C lib/libft re
	@echo ""

debug_srcs: $(OBJS)
	@echo ""
	@echo "  $(BOLD)$(GREEN)$(CHECK) All object files created:$(RESET)"
	@ls -la srcs/*.o
	@echo ""

.PHONY: all clean fclean re header libs debug debug_libs debug_srcs