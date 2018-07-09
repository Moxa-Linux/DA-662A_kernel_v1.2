all: gen_patch

gen_patch:
	mkdir -p Cross
	( cd Cross; \
	diff -Bbur ../Makefile.SH ../Makefile.SH.new > ../../Makefile.SH.patch; \
	diff -Bbur ../installperl ../installperl.new > ../../installperl.patch; \
	cd ..; )
	echo `pwd`
	rm -rf Cross
