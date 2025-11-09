# MinGW/Win32 Makefile for run_pwsh_hidden
# Ensures Unicode startup (-municode) and GUI subsystem (-mwindows)

CC      := gcc
RC      := windres
CFLAGS  := -O2 -Wall -Wextra -DUNICODE -D_UNICODE
LDFLAGS := -municode -mwindows
OUT     := run_pwsh_hidden.exe

SRC     := src/run_pwsh_hidden.c
RES     := res/run_pwsh_hidden.rc
OBJ     := $(SRC:.c=.o)
RESOBJ  := $(RES:.rc=.o)

all: $(OUT)

$(OUT): $(OBJ) $(RESOBJ)
	$(CC) $(OBJ) $(RESOBJ) $(LDFLAGS) -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

res/%.o: res/%.rc
	$(RC) $< $@

clean:
	-del $(OBJ) $(RESOBJ) $(OUT) 2>NUL || rm -f $(OBJ) $(RESOBJ) $(OUT)

.PHONY: all clean
