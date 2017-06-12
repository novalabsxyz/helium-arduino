PROJECT=helium-arduino
GIT_VERSION ?= $(shell git describe --abbrev=4 --dirty --always --tags)
RELEASE_DIR ?= .
RELEASE_PACKAGE ?= ${PROJECT}-${GIT_VERSION}
RELEASE_ZIP ?= Helium.zip
BOARD ?= uno

all: ci

ci:
	for e in examples/*; do \
	  platformio ci --board=${BOARD} --lib=src $$e/*; \
	done

release:
	rm -rf ${RELEASE_DIR}/Helium
	mkdir ${RELEASE_DIR}/Helium
	sed -i 's/<version>/${GIT_VERSION}/g' library.properties
	cp -R src examples ${RELEASE_DIR}/Helium
	find Helium -type d -name ".*" -and -not -name '.' -print0 | xargs -0 rm -rf
	cd ${RELEASE_DIR}; zip -urq ${RELEASE_ZIP} Helium
