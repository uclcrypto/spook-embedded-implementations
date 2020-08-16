CC?=gcc
#CFLAGS:=-std=c99 -Wall -Wextra -g $(CFLAGS)
CFLAGS:=-std=c99 -g $(CFLAGS)

BDIR?=../build_test
SRCDIR?=../src

objects := s1p.o genkat_aead.o encrypt.o primitives.o

all: $(BDIR)/test

bdir:
	mkdir -p $(BDIR)

$(BDIR)/%.o: $(SRCDIR)/%.c bdir
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/%.o: %.c bdir
	$(CC) -c -o $@ $< $(CFLAGS) -I $(SRCDIR)

$(BDIR)/test: $(patsubst %,$(BDIR)/%,$(objects))
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(BDIR)


.PHONY: clean bdir
