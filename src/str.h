// str.h
int str_len(const char *s);
int str_cmp(const char *a, const char *b);
char *str_trim(const char * s);
char *str_substr(const char *src, int start, int length);
int str_indexof(const char *s, const char *d);
char *str_cat(char *strings[], int start, int end , int max, const char *delim);


int chr(char c);
char ord(int a);
int to_hex_base8(int integer);
int to_int(char hex_val);

#define TRUE (1==1)
#define FALSE (!TRUE)