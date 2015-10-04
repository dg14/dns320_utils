#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

int serial_open(char *dev, speed_t velocity) {
    /* Open File Descriptor */
    int USB = open( dev, O_RDWR | O_NONBLOCK | O_NDELAY );

    /* Error Handling */
    if ( USB < 0 )
    {
        fprintf(stderr, "Error opening %s\n", dev);
        exit(0);
    }

    /* *** Configure Port *** */
    struct termios tty;
    memset (&tty, 0, sizeof tty);

    /* Error Handling */
    if ( tcgetattr ( USB, &tty ) != 0 )
    {
        close(USB);
        fprintf(stderr, "Error tcgetattr\n");
        exit(0);
    }

    /* Set Baud Rate */
    cfsetospeed (&tty, velocity);
    cfsetispeed (&tty, velocity);

    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;        // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;
    tty.c_cflag     &=  ~CRTSCTS;       // no flow control
    tty.c_lflag     =   0;          // no signaling chars, no echo, no canonical processing
    tty.c_oflag     =   0;                  // no remapping, no delays
    tty.c_cc[VTIME]     =   5;                  // 0.5 seconds read timeout

    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
    tty.c_iflag     &=  ~(IXON | IXOFF | IXANY);// turn off s/w flow ctrl
    //tty.c_iflag     &=  ~(INLCR | IGNCR | ISTRIP);// turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                     | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    tty.c_oflag     &=  ~OPOST;              // make raw
    cfmakeraw(&tty);

    /* Flush Port, then applies attributes */
    tcflush( USB, TCIFLUSH );

    if ( tcsetattr ( USB, TCSANOW, &tty ) != 0)
    {
        fprintf(stderr, "Error from tcsetattr\n");
        close(USB);
        exit(0);
    }
    return USB;
}

int serial_write ( int device, const char *ptr, int nchars) {
    return write(  device, ptr, nchars);
}

#define TIMEOUT 10000
int serial_read ( int device, char *ptr, int size) {
    struct timeval timeout;
    memset(ptr, '\0', size);
    fd_set set;
    FD_ZERO(&set); /* clear the set */
    FD_SET(device, &set); /* add our file descriptor to the set */

    timeout.tv_sec = 0;
    timeout.tv_usec = TIMEOUT;

    int rv = select(device + 1, &set, NULL, NULL, &timeout);
    if (rv == -1) {
        perror("select"); /* an error accured */
        return -1;
    } else if (rv == 0) {
        printf("timeout"); /* a timeout occured */
        return -1;
    } else {
        return read(device, ptr, size); /* there was data to read */
    }
}

void serial_close (int device) {
    close(device);
}
