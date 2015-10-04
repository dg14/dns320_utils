//test_readserial

// main.c

#include "str.h"
#include "ini.h"
#include "serial.h"
#include "socket.h"
#include <stdio.h>
#include <stdlib.h>

#define DATA_BIT 5

typedef struct {
    int serial;
} data;

int test_readvalue(int serial, char *sequence) {
    char *buffer = (char *)malloc(256);
    printf("WRITE:    ");
    int i;
    for (i = 0; i < 7; i++) {
        printf("%02x ", sequence[i]);
    }
    printf("\n");

//    sleep(1);
    usleep(10000);
    serial_write(serial, sequence, 7);
    usleep(10000);
    serial_read(serial, buffer, 7);
    int k = 1;
    while (buffer[2] != sequence[2]) {
        usleep(10000);
        serial_write(serial, sequence, 7);
        usleep(10000);
        serial_read(serial, buffer, 7);
        k++;

    }
    printf("READ [%02d] ", k);
    for (i = 0; i < 20; i++) {
        printf("%02x ", buffer[i]);
    }
    int ret = buffer[DATA_BIT];
    free(buffer);
    return ret;
}

int main(int argc, char *argv[]) {
    int serial = serial_open("/dev/ttyS1", 115200);
    /*
        char WOLStatusEnableCmd[] =    {0xfa, 0x03, 0x0a, 0x01, 0x00, 0x00, 0xfb};
        char WOLStatusDisableCmd[] =   {0xfa, 0x03, 0x0a, 0x00, 0x00, 0x00, 0xfb};
        char WOLStatusGetCmd[] =       {0xfa, 0x03, 0x0a, 0x02, 0x00, 0x00, 0xfb};

        serial_write(serial, WOLStatusGetCmd, 7);
        sleep(1);
        char *buffer = (char *)malloc(256);
        serial_read(serial, buffer, 256);
        int i = 0;
        for (i = 0; i < 20; i++) {
            printf("%02x ", buffer[i]);
        }
        printf("\n");
        //printf("%d",str_len(buffer));

        fprintf(stdout, "Done\n");
        fflush(stdout);
        char ReadMonthCmd[] = { 0xfa, 0x01, 0x0a, 0x01, 0x01, 0x00, 0xfb };
        serial_write(serial, ReadMonthCmd , 7);
        sleep(1);
        serial_read(serial, buffer, 7);
        for (i = 0; i < 20; i++) {
            printf("%02x ", buffer[i]);
        }
        printf("\n");
    */
    char ReadMonthCmd[] = { 0xfa, 0x01, 0x0a, 0x01, 0x01, 0x00, 0xfb };
    char ReadDateCmd[] = { 0xfa, 0x01, 0x0b, 0x01, 0x01, 0x00, 0xfb };
    char ReadHourCmd[] = { 0xfa, 0x01, 0x0c, 0x01, 0x01, 0x00, 0xfb };
    char ReadMinuteCmd[] = { 0xfa, 0x01, 0x0d, 0x01, 0x01, 0x00, 0xfb };

    printf("%d\n", to_int(test_readvalue(serial, ReadMonthCmd)));
    printf("%d\n", to_int(test_readvalue(serial, ReadHourCmd)));
    printf("%d\n", to_int(test_readvalue(serial, ReadMinuteCmd)));
    printf("%d\n", to_int(test_readvalue(serial, ReadDateCmd)));
    /*
    Results: 1 cycle for first call, 2 for second, 4 for third, and 8 for the forth.
    [1] fa 01 0a 01 01 10 fb 00 00 00 00 00 00 00 00 00 00 00 00 00 10
    [2] fa 01 0c 01 01 07 fb 00 00 00 00 00 00 00 00 00 00 00 00 00 7
    [4] fa 01 0d 01 01 00 fb 00 00 00 00 00 00 00 00 00 00 00 00 00 0
    [8] fa 01 0b 01 01 17 fb 00 00 00 00 00 00 00 00 00 00 00 00 00 17
    */
    serial_close(serial);

}
