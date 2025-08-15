# Build the final ROM
all: laba.gba

CXX      := arm-none-eabi-g++
OBJCOPY  := arm-none-eabi-objcopy
TITLE    := Laba

CXXFLAGS := -mthumb -mthumb-interwork -O2 -Wall -fomit-frame-pointer \
            -std=gnu++17 -I. -I$(DEVKITPRO)/libgba/include \
            -fno-exceptions -fno-rtti
LDFLAGS  := -mthumb -mthumb-interwork -specs=gba.specs -Wl,-Map,game.map
LIBS     := -L$(DEVKITPRO)/libgba/lib -lgba

# Objects
OBJS := game.o

# Compile only
game.o: game.cpp game.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link to ELF, then objcopy to raw GBA, then fix header
game.elf: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

laba.gba: game.elf
	$(OBJCOPY) -O binary $< $@
	gbafix $@ -p -t$(TITLE)

clean:
	rm -rf $(OBJS) *.gba *.elf *.map *.sav
