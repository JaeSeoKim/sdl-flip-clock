PROJECT_ROOT ?= $(abspath $(CURDIR)/../..)
APP_NAME ?= FlipClock
CONFIG_FILE ?= $(PROJECT_ROOT)/config.json
VERSION ?= $(shell sed -n 's/.*"version"[[:space:]]*:[[:space:]]*"\([^"]*\)".*/\1/p' $(CONFIG_FILE))
ifeq ($(strip $(VERSION)),)
VERSION := 0.0.0
endif

OUTPUT_DIR ?= $(PROJECT_ROOT)/output
BUILD_PROFILE ?= onionos
BUILD_DIR := $(OUTPUT_DIR)/build/$(BUILD_PROFILE)
BINARY_DIR := $(BUILD_DIR)/binary
PACKAGE_WORK_DIR := $(OUTPUT_DIR)/package/onionos
PACKAGE_ROOT := $(PACKAGE_WORK_DIR)/$(APP_NAME)
DISTRIBUTION_DIR := $(OUTPUT_DIR)/distributions/onionos
PACKAGE_ZIP := $(DISTRIBUTION_DIR)/$(APP_NAME)-v$(VERSION).zip

package:
	./scripts/build-docker.sh

stage:
	rm -rf $(PACKAGE_ROOT)
	mkdir -p $(PACKAGE_ROOT)
	mkdir -p $(DISTRIBUTION_DIR)
	cp -R $(PROJECT_ROOT)/fonts $(PROJECT_ROOT)/icon $(PROJECT_ROOT)/config.json $(PACKAGE_ROOT)/
	cp $(BINARY_DIR)/flipClock $(PACKAGE_ROOT)/flipClock
	cp $(CURDIR)/package/launch.sh $(PACKAGE_ROOT)/launch.sh
	cp $(CURDIR)/package/set-time.sh $(PACKAGE_ROOT)/set-time.sh
	chmod +x $(PACKAGE_ROOT)/launch.sh $(PACKAGE_ROOT)/set-time.sh

zip:
	cd $(PACKAGE_WORK_DIR) && zip -r ../../distributions/onionos/$(notdir $(PACKAGE_ZIP)) $(APP_NAME)

clean:
	rm -rf $(PACKAGE_ROOT) $(DISTRIBUTION_DIR)/$(APP_NAME)-v*.zip

.PHONY: package stage zip clean
