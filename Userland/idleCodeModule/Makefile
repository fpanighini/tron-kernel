include ../Makefile.inc

MODULE=0000-idleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:asm/%.asm=object/%.asm.o)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=_loader.c

LIB_SOURCES=$(wildcard ../lib/*.c)
LIB_ASM=$(wildcard ../lib/asm/*.o)

LOADEROBJECT=$(LOADERSRC:.c=.o)

all: $(MODULE)

$(MODULE): $(SOURCES) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -I./include -I../lib/include -T idleCodeModule.ld _loader.c $(OBJECTS_ASM) $(SOURCES) $(LIB_ASM) $(LIB_SOURCES) -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -I./include -I../lib/include -T idleCodeModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(OBJECTS_ASM) $(SOURCES) $(LIB_ASM) $(LIB_SOURCES) -o ../0000-idleCodeModule.elf

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -I../lib/include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(GCC) $(GCCFLAGS) $(LOADERSRC) -c -o $(LOADEROBJECT)

object/%.asm.o : asm/%.asm
	mkdir -p object
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o
	rm -rf object

.PHONY: all clean print
