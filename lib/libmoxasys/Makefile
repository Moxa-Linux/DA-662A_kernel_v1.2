all: src_dir
	make -C example

install:
	make -C src install
src_dir:
	make -C src

test_prog:
	make -C testing
clean:
	make -C src clean
	make -C testing clean
	make -C example clean
