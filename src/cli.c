//cli.c

// main.c

//#include "proto.h"
#include "funs.h"
#include <stdlib.h>
#include <signal.h>
#include "log.h"
#include "debug.h"
#include "str.h"
//#include "handlers.h"
/*
int main(int argc, char *argv[]) {
	debug_set(DEBUG_OFF);
	log_set_level(LOG_INFO);
	if (getenv("DEBUG") != NULL) {
		log_set_level(LOG_DEBUG);
	}
	if (argc < 3) {
		log_log(LOG_INFO	, "ERROR: Args check");
		return 0;
	}
	char *serial = argv[1];
	log_log(LOG_INFO, "Serial:%s", serial);

	dns320_connection con;
	if (!dns320_init ( &con, serial , 115200)) {

		log_log(LOG_INFO, "Error opening serial");
		return 0;
	}
	add_handler_functions(&con);

	char *cmd = str_cat(argv, 2, argc - 1 , 255, " ");

	dns320_return ret = dns320_manage_cmd(&con, cmd);
	log_log(LOG_INFO, "Return: %s", ret.data);
	dns320_close ( &con );
}
*/


int main(int argc, char *argv[]) {
	debug_set(DEBUG_OFF);
	log_set_level(LOG_INFO);
	if (getenv("DEBUG") != NULL) {
		log_set_level(LOG_DEBUG);
	}
	if (argc < 3) {
		log_log(LOG_INFO	, "ERROR: Args check");
		return 0;
	}
	char *serial = argv[1];
	log_log(LOG_INFO, "Serial:%s", serial);
	int serial_port = serial_open(serial, 115200);
	char *cmd = str_cat(argv, 2, argc - 1 , 255, " ");
	funs_manage(serial_port, cmd);
	serial_close(serial_port);


}

