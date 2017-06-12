PROJECT=helium-arduino
GIT_VERSION ?= $(shell git describe --abbrev=4 --dirty --always --tags)
RELEASE_DIR ?= .
RELEASE_PACKAGE ?= ${PROJECT}-${GIT_VERSION}
BOARD ?= uno

all: ci

ci:
	for e in examples/*; do \
	  platformio ci --board=${BOARD} --lib=src $$e/*; \
	done

release:
	rm -rf ${RELEASE_DIR}/Helium
	mkdir ${RELEASE_DIR}/Helium
	cp -R src examples ${RELEASE_DIR}/Helium
	find Helium -type d -name ".*" -and -not -name '.' -print0 | xargs -0 rm -rf
	sed 's/<version>/${GIT_VERSION}/g' library.properties > ${RELEASE_DIR}/Helium/library.properties
	zip -urq ${RELEASE_PACKAGE}.zip ${RELEASE_DIR}/Helium
