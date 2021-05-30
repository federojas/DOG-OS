#ifndef LIBC_H
#define LIBC_H
void putChar(char c);
void printf(char *str, ...);
int strlen(const char *s);
int strToInt(char *str, int* size);
char *intToStr(int value, char *buffer, int base);
void newLine();
char *reverse(char *buffer, int i, int j);
int abs(int num);
void swap(char *x, char *y);
char *strcpy(char *destination, const char *source);
void newLine();
int getChar();
int readText();
#endif