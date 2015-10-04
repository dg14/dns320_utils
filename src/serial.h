#include <termios.h>    // POSIX terminal control definitions

int serial_open(char *dev, speed_t velocity);
int serial_write ( int device, const char *ptr, int nchars);
int serial_read ( int device, char *ptr, int size);
void serial_close (int device);

