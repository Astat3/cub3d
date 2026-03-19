NAME = cub3D
MY_NAME = cub3D Project

SRCS =	srcs/main.c \
		srcs/init/data.c \
		srcs/init/player.c \
		srcs/init/textures.c \
		srcs/init/parse_color.c \
		srcs/parsing/init_texture.c \
		srcs/parsing/parse_line.c \
		srcs/parsing/parse_texture.c \
		srcs/parsing/init_map.c \
		srcs/parsing/check_parsing.c \
		srcs/parsing/check_map.c \
		srcs/parsing/utils_pars.c \
		srcs/handling/input_handling.c \
		srcs/handling/game_loop.c \
		srcs/render/raycasting.c \
		srcs/render/render.c \
		srcs/render/dda.c \
		srcs/exit/exit.c \
		srcs/exit/freeeee.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra -I includes/ -g
RM = rm -f

UNAME := $(shell uname -s)
ifeq ($(UNAME), Linux)
	MLX_DIR = lib/minilibx-linux
	MLX_FLAGS = -L $(MLX_DIR) -lmlx -lXext -lX11 -lm -lz
else
	MLX_DIR = lib/minilibx_opengl_20191021
	MLX_FLAGS = -L $(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
endif

MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_LOCAL = libmlx.a

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
	@echo "  $(BOLD)$(MAGENTA)║$(RESET)    $(BOLD)$(CYAN)$(ROCKET) $(MY_NAME) $(RESET)  $(DIM)by roazouan && Agallot$(RESET)     $(BOLD)$(MAGENTA)                          ║$(RESET)"
	@echo "  $(BOLD)$(MAGENTA)╚════════════════════════════════════════════════════════════════════════════╝$(RESET)"
	@echo ""

$(DEPS):
	@echo "  $(BOLD)$(BLUE)$(GEAR) Building libft...$(RESET)"
	@$(MAKE) --no-print-directory -C lib/libft > /dev/null 
	@echo "  $(GREEN)$(CHECK)$(RESET) libft"

$(MLX_LIB):
	@echo "  $(BOLD)$(BLUE)$(GEAR) Building mlx...$(RESET)"
	@if [ -d "$(MLX_DIR)" ]; then $(MAKE) --no-print-directory -C $(MLX_DIR) > /dev/null; else echo "  $(YELLOW)No mlx directory found: $(MLX_DIR)$(RESET)"; fi
	@echo "  $(GREEN)$(CHECK)$(RESET) mlx"
	@if [ -f "$(MLX_LIB)" ]; then cp -f $(MLX_LIB) $(MLX_LOCAL); fi

$(NAME): $(DEPS) $(MLX_LIB) $(OBJS)
	@echo ""
	@echo "  $(BOLD)$(YELLOW)$(LINK) Linking...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(DEPS) $(MLX_FLAGS) -lreadline
	@echo "  $(GREEN)$(CHECK)$(RESET) $(BOLD)$(NAME)$(RESET) created"



srcs/%.o: srcs/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "  $(GREEN)$(CHECK)$(RESET) %s\n" $(notdir $<)

clean:
	@echo ""
	@echo "  $(BOLD)$(YELLOW)$(CLEAN) Cleaning...$(RESET)"
	@$(RM) $(OBJS)
	@$(RM) $(MLX_LOCAL)
	@$(MAKE) --no-print-directory -C lib/libft fclean > /dev/null 
	@if [ -d "$(MLX_DIR)" ]; then $(MAKE) --no-print-directory -C $(MLX_DIR) clean > /dev/null; fi 
	@echo ""

fclean: clean
	@$(RM) $(NAME)
	@echo "  $(GREEN)$(CHECK)$(RESET) $(NAME) removed"
	@echo ""

re: fclean all

# Debug mode - shows all compilation commands
debug: header debug_libs debug_srcs
	@echo ""
	@echo "  $(BOLD)$(YELLOW)$(LINK) Linking...$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(DEPS) $(MLX_FLAGS) -lreadline
	@echo ""
	@echo "  $(BOLD)$(GREEN)$(CHECK) Debug build complete!$(RESET)"
	@echo ""

debug_libs:
	@echo "  $(BOLD)$(RED)🔧 DEBUG MODE - Verbose output$(RESET)"
	@echo ""
	@echo "  $(BOLD)$(BLUE)$(GEAR) Building libft...$(RESET)"
	$(MAKE) -C lib/libft re
	@echo ""
	@echo "  $(BOLD)$(BLUE)$(GEAR) Building mlx...$(RESET)"
	$(MAKE) -C $(MLX_DIR) re
	@cp -f $(MLX_LIB) $(MLX_LOCAL)
	@echo ""

debug_srcs: $(OBJS)
	@echo ""
	@echo "  $(BOLD)$(GREEN)$(CHECK) All object files created:$(RESET)"
	@ls -la $(OBJS)
	@echo ""

.PHONY: all clean fclean re header libs debug debug_libs debug_srcs
