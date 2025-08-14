# Build the final ROM
all: game.gba

# The target "game.o" has two dependencies: "game.cpp" and "game.hpp".
# The -c flag compiles only (no linking). We create the build/ directory first.
game.o: game.cpp game.hpp
	arm-none-eabi-g++ -mthumb -mthumb-interwork -O2 -Wall -fomit-frame-pointer \
	-std=gnu++17 -I. -I$(DEVKITPRO)/libgba/include \
	-c game.cpp -o game.o

# The target "game.gba" depends on "game.o".
# This links the object file into a GBA ROM.
game.gba: game.o
	arm-none-eabi-g++ -mthumb -mthumb-interwork -specs=gba.specs \
	-Wl,-Map,game.map game.o \
	-L$(DEVKITPRO)/libgba/lib -lgba -o game.gba
	@echo "✔ Built game.gba"

# Clean removes build artifacts and the ROM.
clean:
	rm -rf build *.o *.gba *.elf *.map *.sav
