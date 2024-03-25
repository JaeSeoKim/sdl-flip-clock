TARGET=flipClock

CC=$(CROSS_COMPILE)gcc
CFLAGS+=-I$(INC_DIR)

ifeq ($(MIYOOMINI),1)
SDL=1
LDLIBS=-lSDL -lpthread -lSDL_ttf
else
LDLIBS=-lSDL2 -lpthread -lSDL2_ttf
endif

ifeq ($(MIYOOMINI),1)
CROSS_COMPILE?=~/buildroot/output/host/bin/arm-linux-gnueabihf-
SYSROOT	?= $(shell ${CC} --print-sysroot)
ARCH=-marm -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7ve+simd
CFLAGS+=-Os $(ARCH) -I${SYSROOT}/usr/include -ffunction-sections -fdata-sections -Wall
LDFLAGS+=-I${SYSROOT}/usr/lib $(ARCH) -Bstatic,-lutil,-Bdynamic
endif

RM=rm
RMFLAGS=-rf

MKDIR=mkdir
MKDIRFLAGS=-p

OUTPUT_DIR=build
INC_DIR=include
SRC_DIR=src
OBJ_DIR=objs

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

all: $(TARGET)

re: clean all

clean:
	$(RM) $(RMFLAGS) $(OBJS) $(TARGET)

$(TARGET): ${OBJS}
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@$(MKDIR) $(MKDIRFLAGS) $(OBJ_DIR)

.PHONY: all clean re
