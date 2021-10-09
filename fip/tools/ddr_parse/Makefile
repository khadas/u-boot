# Make host test programs

SFX =

CC = cc

CFLAGS  += -Wall -static -lrt -ldl

OBJS = parse.o

BINFILES = parse$(SFX)

.DEFAULT: all
.PHONY: all clean

all:  $(BINFILES)
	@echo $(BINFILES) build done

parse$(SFX): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@
	@rm -f $(OBJS)

%.o: %.c
	@$(CC) $(CFLAGS) $(OFLAGS) -c $< -o  $@

clean:
	@rm -f $(OBJS) parse$(SFX)
