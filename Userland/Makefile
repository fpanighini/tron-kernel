include Makefile.inc

U_DATA=0001-uDataModule.bin

all: uCodeModule uDataModule idleCodeModule

uCodeModule:
	cd uCodeModule; make

idleCodeModule:
	cd idleCodeModule; make

uDataModule:
	printf "This is u data." >> $(U_DATA) && dd if=/dev/zero bs=1 count=1 >> $(U_DATA)

clean:
	cd uCodeModule; make clean
	cd idleCodeModule; make clean
	rm -rf *.bin
	rm -rf *.elf


.PHONY: uCodeModule idleCodeModule all clean
