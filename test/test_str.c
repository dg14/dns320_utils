#include "str.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	char *prova="  aoweiaowiea";
	char *prova2="aoweiaowiea   ";
	char *prova3="1234567";


	printf("[%s],[%s]\n",prova, str_trim(prova));
	printf("[%s],[%s]\n",prova2, str_trim(prova2));	
	printf("[%s]\n",prova3);
	printf("[%s 0 3]\n",str_substr(prova3,0,3));	
	printf("[%s 1 4]\n",str_substr(prova3,1,4));	
	printf("[%s -1 8]\n",str_substr(prova3,-1,8));	
	char *buffer="prova 1234 aaa";
	char *find="1234";
	printf("[%s][%s][%d]\n", buffer,find,str_indexof(buffer, find));

	char *strings[]= {"abc","def","ghi"};
	printf("[%s]\n",str_cat(strings,1,2,255," "));
}
