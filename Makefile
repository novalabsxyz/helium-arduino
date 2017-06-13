PROJECT=helium-arduino
RELEASE_DIR ?= .
RELEASE_ZIP ?= Helium.zip
BOARD ?= uno
ifeq ($(strip $(shell git status --porcelain 2>/dev/null)),)
	GIT_TREE_STATE=clean
else
	GIT_TREE_STATE=dirty
endif

all: ci

.PHONY: docs
docs:
	@doxygen Doxyfile

.PHONY: clean
ci:
	for e in examples/*; do \
	  platformio ci --board=${BOARD} --lib=src $$e/*; \
	done

.PHONY: version
version: check_dirty check_version
	sed -i '' 's/version=.*/version=${VERSION}/' library.properties
	git commit -am "Making library version: ${VERSION}"
	git tag -as -m "Version ${VERSION}" ${VERSION}
	git push --tags origin master


.PHONY: release
release:
	rm -rf ${RELEASE_DIR}/Helium
	mkdir ${RELEASE_DIR}/Helium
	cp -R library.properties src examples ${RELEASE_DIR}/Helium
	find ${RELEASE_DIR}/Helium -type d -name ".*" -and -not -name '.' -print0 | xargs -0 rm -rf
	cd ${RELEASE_DIR}; zip -urq ${RELEASE_ZIP} Helium


.PHONY: gh-pages
gh-pages: docs
	git fetch origin gh-pages
	rm -rf gh-pages
	git worktree prune
	git worktree add gh-pages -B gh-pages --checkout
	cd gh-pages; git pull origin gh-pages
	rm -rf gh-pages/*
	mv docs/html/* gh-pages
	cd gh-pages; git add .; git commit -m "Generate docs"; git push origin gh-pages
	rm -rf gh-pages
	git worktree prune


.PHONY: clean
clean:
	rm -rf docs ${RELEASE_DIR}/Helium ${RELEASE_DIR}/${RELEASE_ZIP}


.PHONY: check_version
check_version:
	ifeq ($(VERSION),)
		$(error VERSION is not set)
	endif


.PHONY: check_dirty
check_dirty:
ifeq ($(GIT_TREE_STATE),dirty)
	$(error git state is not clean)
endif
