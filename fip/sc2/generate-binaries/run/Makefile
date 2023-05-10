
#
# Rules
#

DEVICE_SCS_KEY_TOP ?= $(CURDIR)/../../sc2/device
DEVICE_ROOTRSA_INDEX ?= 0
PROJECT ?= s905x4

ARGS += "DEVICE_SCS_KEY_TOP=$(DEVICE_SCS_KEY_TOP)"
ARGS += "DEVICE_ROOTRSA_INDEX=$(DEVICE_ROOTRSA_INDEX)"
ARGS += "PROJECT=$(PROJECT)"

all: clean build

build:
	make -C rootrsa-0/boot-blobs $(ARGS)
	make -C rootrsa-0/device-fip $(ARGS)

build-boot-blobs:
	make -C rootrsa-0/boot-blobs $(ARGS)

build-ddr-fip:
	make -C rootrsa-0/ddr-fip $(ARGS)

build-device-fip:
	make -C rootrsa-0/device-fip $(ARGS)

clean:
	make clean -C rootrsa-0/boot-blobs
	make clean -C rootrsa-0/device-fip
