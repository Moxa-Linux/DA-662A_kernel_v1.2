#!/usr/bin/perl -w

use Device::SerialPort;

my $port=Device::SerialPort->new("/dev/ttyM0");
my $STALL_DEFAULT=10; # how many seconds to wait for new input
my $timeout=$STALL_DEFAULT;

$port->baudrate(115200);     # baud
$port->databits(8);          # databits
$port->stopbits(1);          # stopbits

#$port->read_char_time(0);  # don't wait for each character
$port->read_const_time(1000); # 1 second per unfulfilled "read" call

my $chars=0;
my $buffer="";

$port->write("Test\n");

# Read from serial port in 1 second.
my ($count,$saw)=$port->read(1); # will read _up to_ 255 chars
print "$saw, $count";
