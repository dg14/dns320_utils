//test debug
#include "log.h"

int main(int argc, char *argv[]) {

	log_set_level(LOG_ALL);
	log_log(LOG_INFO, "test");
	log_log(LOG_INFO, "prova %d", 1);
	log_log(LOG_INFO, "prova %d %d", 1, 2);
	log_log(LOG_INFO, "prova %d %d %d", 1, 2, 3);

	log_string(LOG_INFO, "awpoepawoe awioe aowei oawei aowei aowei aowei aowei aowei aowei aowei aoweiaowei aowei awoeioaweiaowei aowei aowei aowei aowei aowei oawei ", -1, 30);
}


