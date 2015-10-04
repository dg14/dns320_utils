//

#include <stdio.h>
#include "str.h"
#include "ini.h"

int main(int argc, char *argv[]) {

	if (argc<=1) {
		printf("%s file\n",argv[0]);		
		return 0;
	}
	FILE *file;
	if ( (file=ini_open(argv[1])) == NULL) {
		printf("open failed\n");
		return 0;
	}
	char *v=ini_get(file, "Fan","TempHigh");
	if (v!=NULL) {
		printf("%s\n",v);
	}
	char *v2=ini_get(file, "Fan","PollTime");
	if (v2!=NULL) {
		printf("%s\n",v2);
	}
	ini_close(file);
	return 0;
	
}