
.EXPORT_ALL_VARIABLES:

DIRS = ftp

all:
	for i in $(DIRS) ; do make -C $$i $@ ; done

romfs: all
	for i in $(DIRS) ; do make -C $$i $@ ; done

package:

clean:
	-for i in $(DIRS) ; do make -C $$i clean ; done


