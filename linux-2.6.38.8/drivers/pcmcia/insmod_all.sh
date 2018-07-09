#!/bin/sh
insmod ./pcmcia_core.ko 
insmod ./pcmcia.ko
insmod ./pcmcia_rsrc.ko
insmod ./yenta_socket.ko
