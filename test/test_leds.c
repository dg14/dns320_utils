// test_leds

#include "serial.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    int serial=serial_open("/dev/ttyS1",115200);
    char PwrLedOnCmd[] =           {0xfa, 0x03, 0x06, 0x01, 0x00, 0x01, 0xfb};
    char PwrLedOffCmd[] =          {0xfa, 0x03, 0x06, 0x00, 0x00, 0x01, 0xfb};

    fprintf(stdout,"Send off\n");
    serial_write(serial,PwrLedOffCmd, 7);

    fprintf(stdout,"Wait 5 secs\n");    
    fflush(stdout);
    sleep(5);

    fprintf(stdout,"Send on\n");    
    fflush(stdout);

    serial_write(serial,PwrLedOnCmd, 7);

    fprintf(stdout,"Done\n");    
    fflush(stdout);

    serial_close(serial);
}


