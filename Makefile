SHELL = pwsh.exe

CC := clang++
CF := -O1 -m64 -std=c++17 -Wall

SRC_DIR := src
OUT_DIR := out
OBJ_DIR := $(OUT_DIR)/obj

INC := -Iinclude
LIB := -Llib -lSDL3 -lSDL3_image -lSDL3_ttf
BIN := $(OUT_DIR)/TerrorAndEnemy.exe

SRC := $(wildcard $(SRC_DIR)/*.cc $(SRC_DIR)/*/*.cc $(SRC_DIR)/*/*/*.cc)
OBJ := $(subst $(SRC_DIR),$(OBJ_DIR),$(SRC:%.cc=%.o))

CH := src/common.hh
PCH := $(OBJ_DIR)/common.pch


run: $(BIN)
	$(info Running the binary)
	@$(BIN)

$(BIN): $(OBJ)
	$(info Linking object files into the binary)
	@$(CC) $(CF) -o $(BIN) $(OBJ) $(LIB)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(PCH)
	$(info Compiling $<)
	@pwsh.exe -Command "New-Item -ItemType Directory -Force -Path '$(dir $@)' | Out-Null"
	@$(CC) $(CF) -c $< $(INC) -o $@

$(PCH): $(CH)
	$(CC) $(CF) -o $(PCH) $(CH) $(INC)


.PHONY: clear

clear:
	rm -r $(OBJ_DIR)/**
	rm -r $(OUT_DIR)/*.exe
