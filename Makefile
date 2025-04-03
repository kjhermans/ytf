all: preproc
	cd src && make

debug:
	make all DEBUG="-D_DEBUG=1"

.PHONY: test
test:
	cd test && make

.PHONY: doc
doc:
	cd doc && make

preproc:
	cat release | xxd -i | sed -e 's/0x0a//g' \
	  > ./src/include/ytf/release.h
	@MFS=`find src/ doc/ -name Makefile | xargs grep -l 'preproc:'`; \
	  for MF in $$MFS; do \
	    DIR=`dirname $$MF`; \
	    make -C $$DIR preproc; \
	  done

clean:
	@MFS=`find src/ doc/ test/ -name Makefile | xargs grep -l 'clean:'`; \
	  for MF in $$MFS; do \
	    DIR=`dirname $$MF`; \
	    make -C $$DIR clean; \
	  done

superclean: clean
	@MFS=`find src/ doc/ -name Makefile | xargs grep -l 'superclean:'`; \
	  for MF in $$MFS; do \
	    DIR=`dirname $$MF`; \
	    make -C $$DIR superclean; \
	  done

archive: clean
	RELEASE=$$(cat release); \
	/bin/echo "  [TAR] ~/ytf-src-$$RELEASE.tar.gz"; \
	cd .. && \
	  tar czf archive/ytf-src-$$RELEASE.tar.gz \
	  --exclude=\.git ytf/

