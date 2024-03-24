TARGET=flipClock

CC=gcc
CFLAGS+=-I$(INC_DIR)
LDLIBS=-D_THREAD_SAFE -lSDL2 -lSDL2_ttf


PLATFORM ?= $(UNION_PLATFORM)

ifeq ($(PLATFORM),miyoomini)
LIB = /mnt/SDCARD/.tmp_update/lib
CFLAGS := $(CFLAGS) -marm -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7ve -Wl,-rpath=$(LIB)
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
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@$(MKDIR) $(MKDIRFLAGS) $(OBJ_DIR)

.PHONY: all clean re
