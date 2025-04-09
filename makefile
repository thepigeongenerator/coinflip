CC      := x86_64-pc-linux-gnu-gcc
CFLAGS  := -Wall -Wextra -Wpedantic -O2 -std=gnu17
LDFLAGS :=

SRC = $(shell find src/ -name '*.c')
OBJ := $(patsubst src/%,obj/%,$(SRC:.c=.o))
BIN := bin/coinflip

compile: compile_commands.json $(BIN)
clean:
	rm -rf obj/ bin/ compile_commands.json

# link the .o files
$(BIN): $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $(OBJ) -o $(BIN)

# compile .o files (w/ .d files)
obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) -c -MD -MP $(CFLAGS) -o $@ $<

compile_commands.json: makefile
	$(MAKE) clean
	@touch compile_commands.json
	bear -- make compile

-include $(OBJ:.o=.d)
