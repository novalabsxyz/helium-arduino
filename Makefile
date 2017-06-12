PROJECT=helium-arduino
RELEASE_VERSION ?= $(shell git describe --abbrev=4 --dirty --always --tags)
RELEASE_DIR ?= .
RELEASE_ZIP ?= Helium.zip
BOARD ?= uno

all: ci

.PHONY: docs
docs:
	@doxygen Doxyfile

.PHONY: clean
ci:
	for e in examples/*; do \
	  platformio ci --board=${BOARD} --lib=src $$e/*; \
	done

.PHONY: release
release:
	rm -rf ${RELEASE_DIR}/Helium
	mkdir ${RELEASE_DIR}/Helium
	cp -R src examples ${RELEASE_DIR}/Helium
	find ${RELEASE_DIR}/Helium -type d -name ".*" -and -not -name '.' -print0 | xargs -0 rm -rf
	sed 's/<version>/${RELEASE_VERSION}/g' library.properties > ${RELEASE_DIR}/Helium/library.properties
	cd ${RELEASE_DIR}; zip -urq ${RELEASE_ZIP} Helium

.PHONY: clean
clean:
	rm -rf docs ${RELEASE_DIR}/Helium ${RELEASE_DIR}/${RELEASE_ZIP}
