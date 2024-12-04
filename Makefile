# Define the compiler and compilation flags
CC = gcc
CFLAGS = -ggdb3 -Wall -Wextra

# Define the target executable and source files
TARGET = chess
SRC = game_logic.c bitboard.c random_bot.c helper_functions.c print_functions.c main_loop.c display.c file_handler.c move_evaluator.c

# Default rule to build the executable
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) -lSDL2 -lSDL2_image

clean:
	rm -f $(TARGET)