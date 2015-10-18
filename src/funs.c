// funs.c
#include <stdlib.h>
#include "log.h"
#include <string.h>
#include "serial.h"
#include "str.h"
#include <time.h>
#include <stdio.h>

#define TYPE_READ 1
#define TYPE_READ_RAW 2
#define TYPE_WRITE 3
#define TYPE_QUIT 4
#define TYPE_CUSTOM 5
#define NULLP (void *)NULL

#define DATA_BIT 5

typedef void (* func_callback)(int serial, const char *) ;
typedef struct  {
	char *name;
	char sequence[7];
	int type; // 1=>read, 2=>write, 3=>custom
	int size;
	int size_read;
	func_callback handler;
} func;

/*
void ralarm(int serial, const char *cmd) {
	char *buf = malloc(7);
	func function = tabl_inte[i];
	memset(buf, '\0', function.size_read);
	funs_do_get(serial, function.sequence, function.size, buf, function.size_read);
	//log_string(LOG_INFO, buf, function.size_read, 30);
	log_log(LOG_INFO, "%d", to_int(buf[DATA_BIT]));

	log_log(LOG_INFO, "Hello");
}
*/
#define CMD_DeviceReadyCmd			0
#define CMD_AckFromSerial			1
#define CMD_ThermalStatusGetCmd		2
#define CMD_FanStopCmd				3
#define CMD_FanHalfCmd				4
#define CMD_FanFullCmd				5
#define CMD_DeviceShutdownCmd		6
#define CMD_APREnableCmd			7
#define CMD_APRDisableCmd			8
#define CMD_APRStatusCmd			9
#define CMD_PwrLedOnCmd				10
#define CMD_PwrLedOffCmd			11
#define CMD_PwrLedBlinkCmd			12
#define CMD_WOLStatusEnableCmd		13
#define CMD_WOLStatusDisableCmd		14
#define CMD_WOLStatusGetCmd			15
#define CMD_RDateAndTimeCmd			16
#define CMD_WAlarmEnableCmd			17
#define CMD_WAlarmDisableCmd		18
#define CMD_RAlarmMonthCmd			19
#define CMD_RAlarmDateCmd			20
#define CMD_RAlarmHourCmd			21
#define CMD_RAlarmMinuteCmd			22
#define CMD_RAlarmSecondCmd			23
#define CMD_RAlarmFlagCmd			24
#define CMD_WAlarmMonthCmd			25
#define CMD_WAlarmDateCmd			26
#define CMD_WAlarmHourCmd			27
#define CMD_WAlarmMinuteCmd			28
#define CMD_WAlarmSecondCmd			29
#define CMD_WDateAndTimeCmd			30

void hello(int serial, const char *cmd);
void ralarm(int serial, const char *cmd);
void walarm(int serial, const char *cmd);

static func table[] = {
	{ "DeviceReadyCmd",        { 0xfa, 0x30, 0x10, 0x00, 0x00, 0x00, 0xfb }, TYPE_READ , 7, 7, NULLP},
	{ "AckFromSerial",         { 0xfa, 0x30, 0x00, 0x00, 0x00, 0x00, 0xfb }, TYPE_READ , 7, 7, NULLP},
	{ "ThermalStatusGetCmd",   { 0xfa, 0x03, 0x08, 0x00, 0x00, 0x00, 0xfb }, TYPE_READ , 7, 7, NULLP},
	{ "FanStopCmd",            { 0xfa, 0x02, 0x00, 0x00, 0x00, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "FanHalfCmd",            { 0xfa, 0x02, 0x00, 0x01, 0x00, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "FanFullCmd",            { 0xfa, 0x02, 0x00, 0x02, 0x00, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "DeviceShutdownCmd",     { 0xfa, 0x03, 0x03, 0x01, 0x01, 0x14, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "APREnableCmd",          { 0xfa, 0x03, 0x02, 0x01, 0x00, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "APRDisableCmd",         { 0xfa, 0x03, 0x02, 0x00, 0x00, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "APRStatusCmd",          { 0xfa, 0x03, 0x02, 0x02, 0x00, 0x00, 0xfb }, TYPE_READ , 7, 7, NULLP},
	{ "PwrLedOnCmd",           { 0xfa, 0x03, 0x06, 0x01, 0x00, 0x01, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "PwrLedOffCmd",          { 0xfa, 0x03, 0x06, 0x00, 0x00, 0x01, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "PwrLedBlinkCmd",        { 0xfa, 0x03, 0x06, 0x02, 0x00, 0x01, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "WOLStatusEnableCmd",    { 0xfa, 0x03, 0x0a, 0x01, 0x00, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "WOLStatusDisableCmd",   { 0xfa, 0x03, 0x0a, 0x00, 0x00, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "WOLStatusGetCmd",       { 0xfa, 0x03, 0x0a, 0x02, 0x00, 0x00, 0xfb }, TYPE_READ , 7, 7, NULLP},
	{ "RDateAndTimeCmd",       { 0xfa, 0x01, 0x08, 0x01, 0x01, 0x00, 0xfb }, TYPE_READ_RAW , 7, 15, NULLP},
	{ "WAlarmEnableCmd",       { 0xfa, 0x01, 0x10, 0x02, 0x01, 0x01, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "WAlarmDisableCmd",      { 0xfa, 0x01, 0x10, 0x02, 0x01, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "RAlarmMonthCmd",        { 0xfa, 0x01, 0x0a, 0x01, 0x01, 0x00, 0xfb }, TYPE_READ , 7, 7, NULLP},
	{ "RAlarmDateCmd",         { 0xfa, 0x01, 0x0b, 0x01, 0x01, 0x00, 0xfb }, TYPE_READ , 7, 7, NULLP},
	{ "RAlarmHourCmd",         { 0xfa, 0x01, 0x0c, 0x01, 0x01, 0x00, 0xfb }, TYPE_READ , 7, 7, NULLP},
	{ "RAlarmMinuteCmd",       { 0xfa, 0x01, 0x0d, 0x01, 0x01, 0x00, 0xfb }, TYPE_READ , 7, 7, NULLP},
	{ "RAlarmSecondCmd",       { 0xfa, 0x01, 0x0e, 0x01, 0x01, 0x00, 0xfb }, TYPE_READ , 7, 7, NULLP},
	{ "RAlarmFlagCmd",         { 0xfa, 0x01, 0x0f, 0x01, 0x01, 0x00, 0xfb }, TYPE_READ , 7, 7, NULLP},
	{ "WAlarmMonthCmd",        { 0xfa, 0x01, 0x0a, 0x02, 0x01, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "WAlarmDateCmd",         { 0xfa, 0x01, 0x0b, 0x02, 0x01, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "WAlarmHourCmd",         { 0xfa, 0x01, 0x0c, 0x02, 0x01, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "WAlarmMinuteCmd",       { 0xfa, 0x01, 0x0d, 0x02, 0x01, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "WAlarmSecondCmd",       { 0xfa, 0x01, 0x0e, 0x02, 0x01, 0x00, 0xfb }, TYPE_WRITE, 7, 7, NULLP},
	{ "WDateAndTimeCmd",       { 0xfa, 0x01, 0x08, 0x02, 0x07, 0x17, 0xfb }, TYPE_WRITE, 7, 15, NULLP},
	{ "quit", 				   { 0 }                                    , TYPE_QUIT,  0, 0, NULLP},
	{ "hello", 				   { 0 }                                    , TYPE_CUSTOM,  0, 0, &hello},
	{ "ralarm", 			   { 0 }                                    , TYPE_CUSTOM,  0, 0, &ralarm},
	{ "walarm", 			   { 0 }                                    , TYPE_CUSTOM,  0, 0, &walarm},
};
#define NUM_TRIALS 20

// returns 0=>ok, 1=>errors.
int funs_do_set_get(int serial, const char *input, int input_size, char *output, int output_size, int value) {
	char *buffer = (char *)malloc(256);
	memset(buffer, '\0', output_size);
	int i;
	usleep(10000);
	char *cmd = (char *)malloc(256);
	memcpy(cmd,input,input_size);
	if (value != -1) {
		cmd[DATA_BIT] = to_hex_base8(value);
	}
	serial_write(serial, cmd, input_size);
	free(cmd);
	usleep(10000);
	serial_read(serial, buffer, output_size);
	int k = 1;
	while (buffer[2] != input[2]) {
		usleep(10000);
		serial_write(serial, cmd, input_size);
		usleep(10000);
		serial_read(serial, buffer, output_size);
		k++;
		if (k > NUM_TRIALS) {
			return 1;
		}
	}
	strncpy(output, buffer, output_size);
	free(buffer);
	return 0;
}

int funs_do_get(int serial, char *output, int output_size) {
	char *buffer = (char *)malloc(256);
	memset(buffer, '\0', output_size);
	int i;
	usleep(10000);
	serial_read(serial, buffer, 7);
	strncpy(output, buffer, output_size);
	free(buffer);
	return 0;
}

int funs_do_set_get_int(int serial, char *input, int size_write, int size_read, int value, int bit ) {
	char *buf = malloc(size_read);
	memset(buf, '\0', size_read);
	if (funs_do_set_get(serial, input, size_write, buf, size_read, value) > 0) {
		log_log(LOG_INFO, "Errore");
		return -1;
	}
	int ret = to_int(buf[bit]);
	free (buf);
	return ret;
}

void funs_manage(int serial, const char *cmd) {
	int i = 0;
	char *cmd1 = strdup(str_trim(cmd));
	char *token;
	char ch;
	char *buf;
	const char delimiters[] = " ";
	token = strtok(cmd1, delimiters);
	int count = 0;

	for (i = 0; i < sizeof(table) / sizeof(func); i++) {
		func function = table[i];
		if (str_cmp(function.name, token) == 0) {
			log_log(LOG_DEBUG, "Found: [%s] == [%s] (%d)", token, function.name, i);
			log_log(LOG_DEBUG, "Function Type:%d", function.type);
			switch (function.type) {
			case TYPE_READ: // read.
				log_log(LOG_DEBUG, "READ");
				buf = malloc(function.size_read);
				memset(buf, '\0', function.size_read);
				funs_do_set_get(serial, function.sequence, function.size, buf, function.size_read, -1);
				log_log(LOG_INFO, "%d", to_int(buf[DATA_BIT]));
				free(buf);
				log_log(LOG_DEBUG, "DONE");
				break;
			case TYPE_READ_RAW: // read.
				log_log(LOG_DEBUG, "READ_RAW");
				buf = malloc(function.size_read);
				memset(buf, '\0', function.size_read);
				funs_do_set_get(serial, function.sequence, function.size, buf, function.size_read, -1);
				log_string(LOG_INFO, buf, function.size_read, 30);
				free(buf);
				log_log(LOG_DEBUG, "DONE");
				break;
			case TYPE_WRITE: // write
				log_log(LOG_DEBUG, "DONE");
				//funs_do_put(serial, function.sequence, function.size);
				funs_do_set_get(serial, function.sequence, function.size, buf, function.size_read, -1);
				break;
			case TYPE_QUIT: // quit
				log_log(LOG_INFO, "QUIT");
				break;
			case TYPE_CUSTOM: // custom;
				function.handler(serial, cmd);
				break;
			}
			return;
		}
	}
	log_log(LOG_INFO, "Errore, comando non trovato %s", token);
	return;
	// else.
}

void hello(int serial, const char *cmd) {
	log_log(LOG_INFO, "Hello");
}
void ralarm(int serial, const char *cmd) {
	int dd, mo, hh, mm, ss;
	struct tm tm = *localtime(&(time_t) {time(NULL)});
	func function = table[CMD_RAlarmHourCmd];
	if ((hh = funs_do_set_get_int(serial, function.sequence, function.size, function.size_read, -1, DATA_BIT)) < 0) {
		log_log(LOG_INFO, "Errore");
		return;
	}
	function = table[CMD_RAlarmMinuteCmd];
	if ((mm = funs_do_set_get_int(serial, function.sequence, function.size, function.size_read, -1, DATA_BIT)) < 0) {
		log_log(LOG_INFO, "Errore");
		return;
	}
	function = table[CMD_RAlarmSecondCmd];
	if ((ss = funs_do_set_get_int(serial, function.sequence, function.size, function.size_read, -1, DATA_BIT)) < 0) {
		log_log(LOG_INFO, "Errore");
		return;
	}
	function = table[CMD_RAlarmDateCmd];
	if ((dd = funs_do_set_get_int(serial, function.sequence, function.size, function.size_read, -1, DATA_BIT)) < 0) {
		log_log(LOG_INFO, "Errore");
		return;
	}
	function = table[CMD_RAlarmMonthCmd];
	if ((mo = funs_do_set_get_int(serial, function.sequence, function.size, function.size_read, -1, DATA_BIT)) < 0) {
		log_log(LOG_INFO, "Errore");
		return;
	}
	log_log(LOG_INFO, "Alarm set date %02d/%02d time %02d:%02d:%02d UTC", dd, mo, hh, mm, ss);
}

void walarm(int serial, const char *cmd) {
	// walarm dd:mm hh:mm
	char *token;
	const char delimiters[] = " ";
	token = strtok((char *)cmd, delimiters);
	// token is "walarm"
	char *ddmm = strtok(NULL, delimiters);
	char *hhmm = strtok(NULL, delimiters);
	int hh, mm, dd, mo;
	sscanf(ddmm, "%d:%d", &dd, &mo);
	sscanf(hhmm, "%d:%d", &hh, &mm);
	log_log(LOG_INFO, " hh[%d] mm[%d] dd[%d] mo[%d]", hh, mm, dd, mo);
	/*
	read data: None
	set value: '\xfa\x01\r\x02\x01\x00\xfb'
	set value: '\xfa\x01\x0c\x02\x01\x13\xfb'
	set value: '\xfa\x01\x0b\x02\x01\x18\xfb'
	set value: '\xfa\x01\n\x02\x01\x10\xfb'
	set value: '\xfa\x01\x10\x02\x01\x01\xfb'
	read data: '\xfa0\x00\x00\x00\x00\xfb'
	read data: '\xfa0\x00\x00\x00\x00\xfb'
	read data: '\xfa0\x00\x00\x00\x00\xfb'
	read data: None
	set value: '\xfa\x01\r\x01\x01\x00\xfb'
	read data: '\xfa\x01\r\x01\x01\x00\xfb'
	set value: '\xfa\x01\x0c\x01\x01\x00\xfb'
	read data: '\xfa\x01\x0c\x01\x01\x13\xfb'
	set value: '\xfa\x01\x0b\x01\x01\x00\xfb'
	read data: '\xfa\x01\x0b\x01\x01\x18\xfb'
	set value: '\xfa\x01\n\x01\x01\x00\xfb'
	read data: '\xfa\x01\n\x01\x01\x10\xfb'
	*/
	func function = table[CMD_WAlarmMinuteCmd];
	function = table[CMD_WAlarmMinuteCmd];
	funs_do_set_get_int(serial, function.sequence, function.size, function.size_read, mm, DATA_BIT);
	function = table[CMD_WAlarmHourCmd];
	funs_do_set_get_int(serial, function.sequence, function.size, function.size_read, hh, DATA_BIT);
	function = table[CMD_WAlarmDateCmd];
	funs_do_set_get_int(serial, function.sequence, function.size, function.size_read, dd, DATA_BIT);
	function = table[CMD_WAlarmMonthCmd];
	funs_do_set_get_int(serial, function.sequence, function.size, function.size_read, mo, DATA_BIT);
	function = table[CMD_WAlarmEnableCmd];
	funs_do_set_get_int(serial, function.sequence, function.size, function.size_read, 0, DATA_BIT);

//
	char *buffer = (char *)malloc(256);
	memset(buffer, '\0', 7);
	funs_do_get(serial, buffer, 7);
	log_string(LOG_INFO, buffer, 7, 30);

	memset(buffer, '\0', 7);
	funs_do_get(serial, buffer, 7);
	log_string(LOG_INFO, buffer, 7, 30);

	memset(buffer, '\0', 7);
	funs_do_get(serial, buffer, 7);
	log_string(LOG_INFO, buffer, 7, 30);

	memset(buffer, '\0', 7);
	funs_do_get(serial, buffer, 7);
	log_string(LOG_INFO, buffer, 7, 30);

}
