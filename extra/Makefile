#-------------------------------------------------------------
# Makefile
# $Author:$
# $Date:$
#-------------------------------------------------------------

CC = avr-gcc
LD = avr-gcc
AS = avr-gcc
ENCRYPT_TOOL = encryptor.exe

TARGET = target.hex
#TARGET = target.a
#TARGET = target.enc

MCU    = atmega32
F_CPU  = 16000000
RELEASE_URL = "http://192.168.4.202/svn/hard/firmware"
SRCDIR = .
BLDDIR = ./build
EXEDIR = $(BLDDIR)/exe
OBJDIR = $(BLDDIR)/obj
DEPDIR = $(BLDDIR)/dep
RELEASE_DIR = $(BLDDIR)/release
INCDIR = .
LIBDIR = .

HFUSE = 0x03
LFUSE = 0x0a

CDEFS  = GCC
CDEFS += F_CPU=$(F_CPU)UL
CDEFS += ENABLE_BIT_DEFINITIONS
#CDEFS += DEBUGUART				# вывод отладочной информации по UART. 
#CDEFS += EE_SAVE_STATE			# сохранять состояние выходов в EEPROM.

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

# Debugging format.
#     Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#     AVR Studio 4.10 requires dwarf-2.
#     AVR [Extended] COFF format requires stabs, plus an avr-objcopy run.
DEBUG = stabs

SRC    = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.S)
SRC += ../AvrLib/timer32A.c ../AvrLib/extint32A.c

OBJ = $(patsubst %,$(OBJDIR)/%.o,$(notdir $(basename $(SRC))))
DEP = $(patsubst %,$(DEPDIR)/%.d,$(notdir $(basename $(SRC))))
#OBJ    = $(patsubst $(SRCDIR)/%, $(OBJDIR)/%.o, $(basename $(SRC)))
#DEP    = $(patsubst $(SRCDIR)/%, $(DEPDIR)/%.d, $(basename $(SRC)))
LIB    =

vpath %.c   $(SRCDIR) ../AvrLib
vpath %.S   $(SRCDIR)
vpath %.h   $(SRCDIR)
vpath %.h   $(INCDIR)
vpath %.o   $(OBJDIR)
vpath %.d   $(DEPDIR)
vpath %.a   $(LIBDIR)
vpath %.elf $(EXEDIR)
vpath %.lst $(EXEDIR)

CFLAGS  = -mmcu=$(MCU)
CFLAGS += $(addprefix -D,$(CDEFS))
CFLAGS += -O$(OPT)
#CFLAGS += -g$(DEBUG)
CFLAGS += -std=gnu99
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
CFLAGS += -fno-exceptions
#CFLAGS += -Wfloat-equal
#CFLAGS += -Wsign-compare
#CFLAGS += -Wpointer-arith
#CFLAGS += -Wmissing-prototypes
#CFLAGS += -Wunreachable-code
#CFLAGS += -Winline
CFLAGS += -Wundef
CFLAGS += $(addprefix -I, $(INCDIR))
#CFLAGS += -Wa,-adhlns=$(<:%.c=$(OBJDIR)/%.lst)
CFLAGS += -Wa,-adhlns=$(patsubst %,$(OBJDIR)/%.lst,$(notdir $(basename $(<))))

# Compiler flags to generate dependency files.
GENDEPFLAGS := -MMD -MP -MF

#---------------- Linker Options ----------------
#  -Wl,...:   tell GCC to pass this to linker.
#     -Map:   create map file
#   --cref:   add cross reference to  map file
LDFLAGS += -Wl,-Map=$(EXEDIR)/$(TARGET).map,--cref
#LDFLAGS += -lc
#LDFLAGS += -lgcc
#LDFLAGS += -nostartfiles
#LDFLAGS += -T$(SRCDIR)/link_rom.ld
LDFLAGS += $(addprefix -L, $(LIBDIR))
#LDFLAGS += $(addprefix -l, $(LIB))

# Чтобы make не удалял промежуточные файлы.
.SECONDARY: $(OBJ) \
            $(basename $(EXEDIR)/$(TARGET)).bin \
            $(basename $(EXEDIR)/$(TARGET)).elf

# Default target.
all: begin tree build end

begin:
	@echo $(TARGET)
	@echo $(DEP)
	@echo $(SRC)
	@echo $(OBJ)
	@echo $(EXEDIR)/$(TARGET)
	@echo Building targets
	@echo

end: objdump size
	@echo Done building targets
	@echo

tree: $(EXEDIR)	$(OBJDIR) $(DEPDIR) $(RELEASE_DIR)

# Создание каталогов для .o .elf и .d файлов.
$(EXEDIR) $(OBJDIR) $(DEPDIR) $(RELEASE_DIR):
	@echo Create $(@) folder
	$(shell mkdir -p $(@))
	
build: $(EXEDIR)/$(TARGET)

# Шифрование бинарника
$(EXEDIR)/%.enc: $(EXEDIR)/%.bin
	@echo Encryption:
	$(ENCRYPT_TOOL) $<
	@echo

# Создание eep из elf.
$(EXEDIR)/%.eep: $(EXEDIR)/%.elf

# Создание bin из elf.
$(EXEDIR)/%.bin: $(EXEDIR)/%.elf
	@echo Extract binary:
	avr-objcopy -O binary -R .eeprom -R .fuse -R .lock $< $@
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" \
--change-section-lma .eeprom=0 --no-change-warnings -O binary $< $(basename $@).eep
	@echo

# Создание hex из elf
$(EXEDIR)/%.hex: $(EXEDIR)/%.elf
	@echo Extract hex:
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock $< $@
	@echo Extract eeprom:
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" \
--change-section-lma .eeprom=0 --no-change-warnings -O ihex $< $(basename $@).eep
	@echo

# Сборка выходного файла в Executable and Linkable Format.
$(EXEDIR)/%.elf: $(OBJ)
	@echo Linking...
	$(LD) $(CFLAGS) $^ $(addprefix -l, $(LIB)) -o $@ $(LDFLAGS)
	@echo
	@echo Generating list file:
	avr-objdump -h -S $@ > $(basename $@).lst
	@echo
	@echo Size:
	avr-size $@
	@echo

$(EXEDIR)/%.a: $(OBJ)
	@echo Make archive...
	$(AR) rc $@ $^

# Создание из исходников C.
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPDIR)/%.d
	$(CC) -c $(CFLAGS) $(GENDEPFLAGS) $(DEPDIR)/$(@F:.o=.d) -o $@ $<
	@echo

# Чтобы цели выполнялись в случае отсутствия файла зависимостей.
$(DEP):

clean:
	-rm $(EXEDIR)/*
	-rm $(OBJ)
	-rm $(DEP)

write: write-fuse write-flash
	@echo

write-fuse:
	avrdude -p$(MCU) -cusbasp -B15.0 -u -Uhfuse:w:$(HFUSE):m -Ulfuse:w:$(LFUSE):m

write-flash:
	avrdude -p$(MCU) -cusbasp -y -Uflash:w:$(EXEDIR)/$(TARGET):i

avrosp-write:
	AVROSP -d$(MCU) -if$(EXEDIR)/$(TARGET) -e -pf -vf -cCOM1 -B9600

release:
	svn checkout $(RELEASE_URL) $(RELEASE_DIR)
	cp $(EXEDIR)/$(TARGET) $(RELEASE_DIR)/
	-svn add $(RELEASE_DIR)/$(TARGET)
	svn commit $(RELEASE_DIR) -m "Release autocommit"

# Include the dependency files.
-include $(wildcard $(DEPDIR)/*)
 
# Listing of phony targets.
.PHONY : all begin build depend clean end objdump size tree bin release
