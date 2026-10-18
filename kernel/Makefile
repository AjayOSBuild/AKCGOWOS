# Kernel Subsystem Makefile

.PHONY: all clean build

all: build

build:
	@../scripts/build_kernel.sh

clean:
	@rm -f ../build/bzImage
	@if [ -d "src/linux-6.6.60" ]; then \
		$(MAKE) -C src/linux-6.6.60 clean; \
	fi
