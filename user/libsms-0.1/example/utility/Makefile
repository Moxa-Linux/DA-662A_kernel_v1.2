#CC = arm-linux-gcc
CFLAGS = -g -O2 -I../../include -I../../ -L../../src
LDFLAGS = -lsms
#CPP = xscale-linux-gcc -E
#CPPFLAGS = 
#CXX = arm-linux-g++
#CXXCPP = arm-linux-g++ -E
#RANLIB = arm-linux-ranlib
VERSION = 0.1
MODULES=main.o fn.o
EXE=egprscmd
EXAMPLE=gprs_diagnose sim_unlock gprs_disconnect sim_change_pin sms_read_text_msg gprs_reconnect sim_enable_pin sms_remove_msg gprs_connect sim_get_pin_status sms_send_text_msg gprs_connection_status sim_input_pin

all: ${MODULES}
	${CC} ${CFLAGS} ${LDFLAGS} -o ${EXE} ${MODULES} -lsms
	cp $(EXE) $(INSTALL_ROOT)/bin/$(EXE)
	cp $(EXAMPLE) $(INSTALL_ROOT)/bin/

main.o: main.c
	${CC} ${CFLAGS} -c $^

fn.o: fn.c
	${CC} ${CFLAGS} -c $^

clean:
	rm -rf ${MODULES}
	rm -rf ${EXE}

romfs:
	$(ROMFSINST) $(INSTALL_ROOT)/bin/$(EXE) /bin/$(EXE)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXE)
	$(ROMFSINST) $(INSTALL_ROOT)/bin/gprs_diagnose /bin/gprs_diagnose
	$(ROMFSINST) $(INSTALL_ROOT)/bin/gprs_disconnect /bin/gprs_disconnect
	$(ROMFSINST) $(INSTALL_ROOT)/bin/sim_unlock /bin/sim_unlock
	$(ROMFSINST) $(INSTALL_ROOT)/bin/sim_change_pin /bin/sim_change_pin
	$(ROMFSINST) $(INSTALL_ROOT)/bin/sms_read_text_msg /bin/sms_read_text_msg
	$(ROMFSINST) $(INSTALL_ROOT)/bin/gprs_reconnect /bin/gprs_reconnect
	$(ROMFSINST) $(INSTALL_ROOT)/bin/sim_enable_pin /bin/sim_enable_pin
	$(ROMFSINST) $(INSTALL_ROOT)/bin/sms_remove_msg /bin/sms_remove_msg
	$(ROMFSINST) $(INSTALL_ROOT)/bin/gprs_connect /bin/gprs_connect
	$(ROMFSINST) $(INSTALL_ROOT)/bin/sim_get_pin_status /bin/sim_get_pin_status
	$(ROMFSINST) $(INSTALL_ROOT)/bin/sms_send_text_msg /bin/sms_send_text_msg
	$(ROMFSINST) $(INSTALL_ROOT)/bin/gprs_connection_status /bin/gprs_connection_status
	$(ROMFSINST) $(INSTALL_ROOT)/bin/sim_input_pin /bin/sim_input_pin

