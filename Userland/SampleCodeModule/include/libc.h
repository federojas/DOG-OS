#ifndef LIBC_H
#define LIBC_H

//int scanf(const char *format, ...);
void printf(char *str, ...);
void putChar(char c);
int getChar();

int strlen(const char *s);
int strToInt(char *str, int* size);
char *intToStr(int value, char *buffer, int base);
void newLine();
char *reverse(char *buffer, int i, int j);
int abs(int num);
void swap(char *x, char *y);
char *strcpy(char *destination, const char *source);
void newLine();
int readText();
int strToHex(const char *str);
// float sqrt(float a);
// void floatToStr(float n, char* res, int afterpoint);
// int pow(int x, unsigned int y);
#endif