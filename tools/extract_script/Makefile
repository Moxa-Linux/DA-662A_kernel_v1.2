#Notice : version only x.x
# Create by jimmy_chen@moxa.com
#
ifndef VERSION
VERSION=1.0
endif

ifndef PNAME
PNAME=noname
endif

ifeq ("$(PNAME)","UC-1260-LX-HITZ")
INSTALL_HEADER=install.header.uc1260
else
INSTALL_HEADER=install.header
endif

all: $(INSTALL_HEADER)
	cp $(INSTALL_HEADER) $(INSTALL_HEADER).tmp;
	sed -i -e 's/V.\../V$(VERSION)/' $(INSTALL_HEADER).tmp;
	sed -i -e 's/PNAME/$(PNAME)/' $(INSTALL_HEADER).tmp;
	[ -d "installer" ] || mkdir "installer"
	cat $(INSTALL_HEADER).tmp install.tgz > installer/$(PNAME)-$(VERSION).sh

clean:
	rm -rf installer
