include ../Makefile.inc

MODULE=0000-idleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:asm/%.asm=object/%.asm.o)

all: $(MODULE)

$(MODULE): $(SOURCES) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -I./include -T idleCodeModule.ld _loader.c $(OBJECTS_ASM) $(SOURCES) -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -I./include -T idleCodeModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(OBJECTS_ASM) $(SOURCES) -o ../0000-idleCodeModule.elf

object/%.asm.o : asm/%.asm
	mkdir -p object
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o
	rm -rf object

.PHONY: all clean print
