// debug.c
#include "debug.h"



static int debug_level=DEBUG_OFF;

void debug_set(int debug) {
	debug_level=debug;
}
void debug_init() {
	debug_level=DEBUG_OFF;
}
int debug_get() {
	return debug_level;
}