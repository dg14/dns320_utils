//log
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "str.h"
#include "log.h"

static int log_level = LOG_NONE;

char *log_labels[] = { "INFO", "ERROR", "DEBUG" };
#define LABEL_INFO 0
#define LABEL_ERROR 1
#define LABEL_DEBUG 2



void log_set_level(int level) {
	log_level = level;
}
void log_init() {
	log_level = LOG_NONE;
}

void log_log(int level, const char *str, ...) {
	if (level <= log_level && level != LOG_NONE) {
		struct tm tm = *localtime(&(time_t) {time(NULL)});
		char timestr[2048];
		strftime(timestr, 2048, "%Y-%m-%d %H:%M:%S ", &tm);
		fputs(timestr, stdout);

		switch (level) {
		case LOG_INFO:
			fputs(log_labels[LABEL_INFO], stdout);
			break;
		case LOG_DEBUG:
			fputs(log_labels[LABEL_DEBUG], stdout);
			break;
		case LOG_ERROR:
			fputs(log_labels[LABEL_ERROR], stdout);
			break;
		}
		fputs(" ", stdout);

		va_list argptr;
		va_start(argptr, str);
		vfprintf(stdout, str, argptr);
		va_end(argptr);
		fputs("\n", stdout);
		fflush(stdout);
	}

}


void log_string(int level, char *buffer, int bufferSize, int cn) {
	if (level <= log_level) {
		if (bufferSize < 0) bufferSize = str_len(buffer);
		int pos = 0, offset = 0, row, lineLength;
		int numRows = (bufferSize / cn) + 1;
		for (row = 0; row < numRows; row++) {
			offset = (row * cn);
			int lineLength = row < numRows - 1 ? cn : (bufferSize % cn);

			for (pos = 0; pos < lineLength; pos++) {
				unsigned char ch = buffer[offset + pos];
				fprintf(stdout, "%02x ", ch);
			}
			if (lineLength < cn) {
				for (pos = lineLength + 1; pos <= cn; pos++) {
					fprintf(stdout, "   ");
				}
			}

			for (pos = 0; pos < lineLength; pos++) {
				char ch = buffer[offset + pos];
				if (isprint(ch) && ch > 0) {
					fprintf(stdout, "%c", ch);
				} else {
					fprintf(stdout, " ");
				}
			}
			if (lineLength < cn) {
				for (pos = lineLength + 1; pos <= cn; pos++) {
					fprintf(stdout, " ");
				}
			}

			fprintf(stdout, "\n");
		}
		fflush(stdout);
	}
}
