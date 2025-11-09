# Makefile for run_pwsh_hidden (Windows-only)

NAME      := run_pwsh_hidden
SRC       := src/$(NAME).c
RES       := res/$(NAME).rc
OUTDIR    := build

CC        := gcc
WINDRES   := windres
CFLAGS    := -O2 -Wall -Wextra -municode -mwindows
LDFLAGS   := -s
RESOBJ    := $(OUTDIR)/$(NAME).res

all: $(OUTDIR)/$(NAME).exe

$(OUTDIR):
	@mkdir -p $(OUTDIR)

$(RESOBJ): $(RES) | $(OUTDIR)
	$(WINDRES) $< -O coff -o $@

$(OUTDIR)/$(NAME).exe: $(SRC) $(RESOBJ) | $(OUTDIR)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(RESOBJ) $(LDFLAGS)

clean:
	@rm -rf $(OUTDIR)

.PHONY: all clean
