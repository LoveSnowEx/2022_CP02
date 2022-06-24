CC=gcc
WSDIR:= $(CURDIR)/../wsServer/
INCLUDE:= -I $(WSDIR)/include
OBJDIR:=$(shell [ -d obj ] || mkdir obj && echo "obj")
CFLAGS= $(INCLUDE) -Wall -Wextra -std=gnu11 -pthread -pedantic
LDFLAGS=-lm -lpopt $(WSDIR)/libws.a

TARGETS=main
ws_client_OBJ=basic.o cJSON.o ws_client.o
main_OBJ=basic.o cJSON.o main.o


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
