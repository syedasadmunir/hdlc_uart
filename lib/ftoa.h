#ifndef ftoa_h
#define ftoa_h


void reverse(char *str, int len);
int intToStr(int x, char str[], int d);

/*
 * Convert floating point number to string
 */
void ftoa(float n, char *res, int afterpoint);

#endif