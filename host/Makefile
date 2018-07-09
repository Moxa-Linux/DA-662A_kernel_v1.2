
#DIRS := mkfrm mkrom mkencryp ucfinder php-target openswan-target
DIRS := mkhfm mkrom mkrconfig #mkencryp ucfinder moxalib

clean:
	for i in $(DIRS); \
	do	\
		cd $$i && make clean && cd .. ; \
	done

release:
	for i in $(DIRS); \
	do	\
		cd $$i && make release && cd .. ; \
	done

debug:
	for i in $(DIRS); \
	do	\
		cd $$i && make debug && cd .. ; \
	done
