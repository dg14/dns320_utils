//test_debug
#include <stdio.h>
#include "debug.h"

void main() {
	debug_set(DEBUG_OFF);
	if (debug_get()) {
		printf("ON DEBUG\n");
	} else {
		printf("NORMAL\n");
	}
	printf("SETTING DEBUG OON\n");
	debug_set(DEBUG_ON);
	if (debug_get()) {
		printf("ON DEBUG\n");
	} else {
		printf("NORMAL\n");
	}
}