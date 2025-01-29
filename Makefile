
INCLUDES = -I./include
TARGET = mt19937_full_example
CFLAGS = -Wall -Wextra -O2 -Wpedantic

ifeq ($(OS),Windows_NT)
    EXT = exe
    RM = del /Q /F 2>NUL
else
    EXT = out
    RM = rm -f
endif

all: mt19937_full_example.$(EXT) mt19937_small_example.$(EXT)

mt19937_full_example.$(EXT): examples/mt19937_full_example.c src/random.c src/mt19937.c
	gcc -o $@ $(CFLAGS) $(INCLUDES) $^

mt19937_small_example.$(EXT): examples/mt19937_small_example.c src/random.c src/mt19937.c
	gcc -o $@ $(CFLAGS) $(INCLUDES) $^

.PHONY: clean
clean:
	$(RM) *.exe *.out