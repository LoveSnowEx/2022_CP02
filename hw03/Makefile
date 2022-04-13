CC=gcc
OBJDIR:=$(shell [ -d obj ] || mkdir obj && echo "obj")
CFLAGS=-Wall -Wextra -std=gnu11
LDFLAGS=-lm

TARGETS=hw0301 hw0302 hw0303 hw0304 hw0305
hw0301_OBJ=basic.o hw0301.o
hw0302_OBJ=basic.o hw0302.o
hw0303_OBJ=basic.o bmp.o hw0303.o
hw0304_OBJ=basic.o bmp.o hw0304.o
hw0305_OBJ=basic.o bmp.o hw0305.o

.PHONY: all

all: CFLAGS:=$(CFLAGS) -O2
all: $(TARGETS)

debug: CFLAGS:=$(CFLAGS) -g -DDEBUG -fsanitize=leak -fsanitize=undefined
debug: LDFLAGS:=$(LDFLAGS) -fsanitize=address -lubsan -lasan
debug: $(TARGETS)

dev: CFLAGS:=$(CFLAGS) -g -DDEBUG
dev: LDFLAGS:=$(LDFLAGS)
dev: $(TARGETS)


.SECONDEXPANSION:
$(TARGETS): $$(patsubst %, $(OBJDIR)/%, $$($$@_OBJ))
	$(CC) $(filter %.o, $^) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(TARGETS) obj
