#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "log.h"

int str_len(const char *t) {
	return strlen(t);
}
int str_cmp(const char *a, const char *b) {
	return strcmp(a, b);
}

char *str_trim(const char * s) {
	int l = str_len(s);
	while (isspace(s[l - 1])) --l;
	while (* s && isspace(* s)) ++s, --l;
	return strndup(s, l);
}

char *str_substr(const char *src, int start, int length) {
	int srcLen = str_len(src);
	int start1 = start >= 0 ? (start > srcLen ? srcLen : start) : 0;
	char *ret = (char *)malloc(length <= srcLen ? length : srcLen);
	memcpy( ret, &src[start1], length <= (srcLen - start1) ? length : (srcLen - start1) )	;
	return ret;
}

int str_indexof(const char *src, const char *find) {
	int l0 = str_len(src), l1 = str_len(find), i;
	//printf("SrC:[%s](%d) find:[%s](%d)\n",src,l0,find,l1);
	//printf("%d",l0-l1);
	if (l0 - l1 < 0) return -1;
	for (i = 0; i < l0 - l1; i++) {
		//printf("%d:[%s][%s]\n",i,substr(src,i,l1),find);
		if (str_cmp(str_substr(src, i, l1), find) == 0) {
			return i;
		}
	}
	return -1;
}

char *str_cat(char *strings[], int start, int end , int max, const char *delim) {
	int length = 0;
	int i;
	for (i = start; i <= end; i++) {
		char *ptr = strings[i];
		if (ptr == NULL) return NULL;
		int l = str_len(ptr);
		if (l > max) return NULL;
		if (i > start) length += str_len(delim);
		length += str_len(ptr);
	}
	char *ret = (char *)malloc(length);
	memcpy(ret, "\0", length);
	for (i = start; i <= end; i++) {
		char *ptr = strings[i];
		if (i > start) strcat(ret, delim);
		strcat(ret, ptr);
	}
	return str_trim(ret);
}

int chr(char c) {
	return c;
}
char ord(int a) {
	return a;
}

int to_hex_base8(int integer) {
	return chr(integer + (integer / 10) * 6);
}
int to_int(char hex_val) {
	int temp = ord(hex_val);
	return ( temp >> 4 ) * 10 + (temp & 0x0f);
}
