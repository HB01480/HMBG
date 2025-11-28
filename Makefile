SHELL = pwsh.exe

CC := clang
CF := -O1 -m64 -std=c17 -Wall

SRC_DIR := src
OUT_DIR := out
OBJ_DIR := $(OUT_DIR)/obj

INC := -Iinclude
LIB := -Llib -lSDL3 -lSDL3_image -lSDL3_ttf
BIN := $(OUT_DIR)/HMBG.exe

SRC := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c $(SRC_DIR)/*/*/*.c)
HDR := $(wildcard $(SRC_DIR)/*.h $(SRC_DIR)/*/*.h $(SRC_DIR)/*/*/*.h)
OBJ := $(subst $(SRC_DIR),$(OBJ_DIR),$(SRC:%.c=%.o))

CH := src/common.h
PCH := $(OBJ_DIR)/common.pch

ARGS := debug


run: $(BIN)
	$(info Running the binary)
	@$(BIN) $(ARGS)

$(BIN): $(OBJ)
	$(info Linking object files into the binary)
	@$(CC) $(CF) -o $(BIN) $(OBJ) $(LIB)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDR) $(PCH)
	$(info Compiling $<)
	@pwsh.exe -Command "New-Item -ItemType Directory -Force -Path '$(dir $@)' | Out-Null"
	@$(CC) $(CF) -c $< $(INC) -o $@

$(PCH): $(CH)
	$(CC) $(CF) -o $(PCH) $(CH) $(INC)


.PHONY: clear

clear:
	rm -r $(OBJ_DIR)/**
	rm -r $(OUT_DIR)/*.exe
