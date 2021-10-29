#ifndef LIBC_H
#define LIBC_H

#include <colors.h>
#include <stdint.h>

#define C1_WIDTH 13
#define C2_WIDTH 38
#define TABLE_WIDHT (C1_WIDTH + C2_WIDTH + 3)

void setBGC(t_color colour);
void setFTC(t_color colour);
int scanf(char * str, ...);
void printf(char *str, ...);
void printc(t_color color, char *str, ...);
void putChar(char c);
// int getChar(); // MODO ESPERA ACTIVA
char getChar(); // MODO HLT
int strlen(const char *s);
int strcmp(char string1[], char string2[]);
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
void sendUserData(char *userName, int len);
void doubleToStr(double n, char* res, int afterpoint);
int pow(int x, unsigned int y);
void strToDouble(char *numStr, double *result);
void setFirstChange(int number);
void * memset(void * destination, int32_t c, uint64_t length);
int isVowel(char c);
void printDivider(int uniformly, int c1, int);
void printCenteredHeading(char * str, ...);
void printRow(char *str1, char *str2, int firstRow);
void printCol(char *str, int width);
void printFullDivider();
void sleep(int seconds);
void printLogo(t_color color, int colored);
void printDog1();
void printDog2();
void printDog3();
void printDog4();
void printDogAnimation(int i);
#endif