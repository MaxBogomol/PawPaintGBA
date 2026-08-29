.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/gba_rules

TARGET		:=	$(shell basename $(CURDIR))
BUILD		:=	build
SOURCES		:=	source
DATA		:=	data
INCLUDES	:=	include
SPRITES		:=  sprites

ARCH	:=	-mthumb -mthumb-interwork

CFLAGS	:=	-g -Wall -O3\
			-mcpu=arm7tdmi -mtune=arm7tdmi\
 			-fomit-frame-pointer\
			-ffast-math \
		$(ARCH)

CFLAGS		+=	$(INCLUDE)
CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:=	$(ARCH)
LDFLAGS	=	-g $(ARCH) -Wl,-Map,$(notdir $@).map

LIBS	:= -lgba
LIBDIRS	:= ../libs $(LIBGBA)

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(SPRITES),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES		:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
SPRITE_FILES	:=  $(foreach dir,$(SPRITES),$(notdir $(wildcard $(dir)/*.png)))


ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(SPRITE_FILES:.png=.o) \
				$(addsuffix .o,$(BINFILES)) \
				$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

all	: $(BUILD)

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).gba

else

DEPENDS	:=	$(OFILES:.o=.d)

$(OUTPUT).gba	:	$(OUTPUT).elf
$(OUTPUT).elf	:	$(OFILES)

%.s %.h : %.png
	grit $< -ff../sprites/sprite.grit -o$*

%.o	: %.pcx
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPENDS)

endif