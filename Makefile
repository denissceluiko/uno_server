CC = gcc
CFLAGS = -Wall -Werror 
LDFLAGS = -pthread
TARGET = uno_server

BIN_DIR = bin
INCLUDE_DIR = include
SOURCE_DIR = src

SRC_FILES = $(wildcard $(SOURCE_DIR)/*.c) main.c
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
EXEC = $(BIN_DIR)/uno_server

all: $(EXEC)

$(BIN_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(EXEC)
