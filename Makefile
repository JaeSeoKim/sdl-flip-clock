TARGET_NAME ?= flipClock
CONFIG_FILE ?= config.json
VERSION ?= $(shell sed -n 's/.*"version"[[:space:]]*:[[:space:]]*"\([^"]*\)".*/\1/p' $(CONFIG_FILE))
ifeq ($(strip $(VERSION)),)
VERSION := 0.0.0
endif

CC=$(CROSS_COMPILE)gcc
# CFLAGS+=-I$(INC_DIR) -Werror
CFLAGS+=-I$(INC_DIR)
CFLAGS+=-MMD -MP
CFLAGS+=$(WARNFLAGS)

SDL2_CONFIG ?= sdl2-config
SDL2_CFLAGS ?= $(shell $(SDL2_CONFIG) --cflags)
SDL2_LIBS ?= $(shell $(SDL2_CONFIG) --libs) -lSDL2_ttf
CFLAGS+=$(SDL2_CFLAGS)
LDLIBS=$(SDL2_LIBS) -lpthread -lm

RM=rm
RMFLAGS=-rf

CP=cp
CPFLAGS=-R

MKDIR=mkdir
MKDIRFLAGS=-p

OUTPUT_DIR ?= output
BUILD_PROFILE ?= local
BUILD_DIR=$(OUTPUT_DIR)/build/$(BUILD_PROFILE)
BINARY_DIR=$(BUILD_DIR)/binary
INC_DIR=include
SRC_DIR=src
OBJ_DIR=$(BUILD_DIR)/objects
TARGET=$(BINARY_DIR)/$(TARGET_NAME)

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))
DEPS=$(OBJS:.o=.d)

LEGACY_OUTPUTS=out build objs $(TARGET_NAME) FlipClock-v*.zip

all: $(TARGET)

re: clean all

clean:
	$(RM) $(RMFLAGS) $(OUTPUT_DIR) $(LEGACY_OUTPUTS)

clean-build:
	$(RM) $(RMFLAGS) $(BUILD_DIR)

$(TARGET): ${OBJS}
	@$(MKDIR) $(MKDIRFLAGS) $(BINARY_DIR)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@$(MKDIR) $(MKDIRFLAGS) $(OBJ_DIR)

-include $(DEPS)

onionos:
	$(MAKE) -C distributions/onionos package


.PHONY: all clean clean-build re onionos
