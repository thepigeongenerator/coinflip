CC      ?= cc
CFLAGS  += -Wall -Wextra -Wpedantic -O3 -MD -MP -std=gnu99 -DNDEBUG
LDFLAGS +=

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
	$(CC) -c $(CFLAGS) -o $@ $<

ifneq ($(shell which bear),)
compile_commands.json: makefile
	$(MAKE) clean
	@touch compile_commands.json
	bear -- make compile
else
compile_commands.json:
endif

-include $(OBJ:.o=.d)
