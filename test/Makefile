CC?=gcc
#CFLAGS:=-std=c99 -Wall -Wextra -g $(CFLAGS)
CFLAGS:=-std=c99 -g $(CFLAGS)

BDIR?=../build_test
SRCDIR?=../src

CLYDE_TYPE?=clyde_32bit
#CLYDE_TYPE?=clyde_64bit

SHADOW_TYPE?=shadow_32bit
#SHADOW_TYPE?=shadow_128bit
#SHADOW_TYPE?=shadow_256bit
#SHADOW_TYPE?=shadow_512bit

#CFLAGS+=-DDBOX=32 # dbox over 32 bit
#CFLAGS+=-DDBOX=8 # dbox over 8 bit
CFLAGS+=-DDBOX=1 # dbox over 1 bit

CFLAGS+=-DSHCST=1 # old cst addition
#CFLAGS+=-DSHCST=8 # 8 bit per bundle (on 1 word)
#CFLAGS+=-DSHCST=32 # 32 bits per bundle (on 1 word)

ifeq "$(CLYDE_TYPE)" "clyde_64bit"
    CFLAGS+=-mbmi2
endif
ifeq "$(SHADOW_TYPE)" "shadow_256bit"
    CFLAGS+=-mavx2
endif
ifeq "$(SHADOW_TYPE)" "shadow_512bit"
    CFLAGS+=-mavx512f
endif

objects := s1p.o genkat_aead.o encrypt.o primitives.o $(CLYDE_TYPE).o $(SHADOW_TYPE).o

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
