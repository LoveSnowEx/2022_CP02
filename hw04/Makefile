CC=gcc
OBJDIR:=$(shell [ -d obj ] || mkdir obj && echo "obj")
CFLAGS=-Wall -Wextra -std=gnu11
LDFLAGS=-lm -lpopt

TARGETS=hw0401 hw0403 hw0404 # hw0403 #showdiff # pb5 #hw0401 hw0402 hw0403 hw0404 hw0405
hw0401_OBJ=basic.o bmp.o hw0401.o
# hw0402_OBJ=basic.o hw0302.o
hw0403_OBJ=basic.o mmap.o codehighlighter.o hw0403.o
hw0404_OBJ=basic.o mmap.o hw0404.o
# showdiff_OBJ=basic.o mmap.o showdiff.o
# pb5_OBJ=basic.o pb5.o
# test_OBJ=mmap.o test.o
# test2_OBJ=basic.o test2.o

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
