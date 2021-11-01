// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboardDriver.h>
#include <prints.h>
#include <syscalls.h>
#include <videoDriver.h>

static int scan(char const *fmt, va_list arg);

void printLine() { newLine(); }

int strlen(char *str) {
  int size = 0;
  for (int i = 0; str[i] != 0; i++) {
    size++;
  }
  return size;
}

void strcpy(char dest[], const char source[]) {
  int i = 0;
  while (1) {
    dest[i] = source[i];

    if (dest[i] == 0) {
      break;
    }

    i++;
  }
}

// FUNCION DE CATEDRA
static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base) {
  char *p = buffer;
  char *p1, *p2;
  uint32_t digits = 0;

  // Calculate characters for each digit
  do {
    uint32_t remainder = value % base;
    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
    digits++;
  } while (value /= base);

  // Terminate string in buffer.
  *p = 0;

  // Reverse string in buffer.
  p1 = buffer;
  p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }

  return digits;
}

int log2(uint32_t number) {
  if (number == 0) {
    return -1;
  }

  int result = -1;
  while (number) {
    result++;
    number >>= 1;
  }
  return result;
}

int pow(int x, unsigned int y) {
  if (y == 0) {
    return 1;
  } else if ((y % 2) == 0) {
    return pow(x, y / 2) * pow(x, y / 2);
  }

  else {
    return x * pow(x, y / 2) * pow(x, y / 2);
  }
}

void printIntDec(uint64_t num) {
  char buffer[10];
  uintToBase(num, buffer, 10);
  sysWrite(buffer, strlen(buffer), BLACK, WHITE, 0);
}

void printIntHex(uint64_t num) {
  char buffer[10];
  uintToBase(num, buffer, 16);
  sysWrite(buffer, strlen(buffer), BLACK, WHITE, 0);
}

void printIntBin(uint64_t num) {
  char buffer[10];
  uintToBase(num, buffer, 2);
  sysWrite(buffer, strlen(buffer), BLACK, WHITE, 0);
}

int printf(char const *fmt, ...) {
  va_list arguments;
  int length;

  va_start(arguments, fmt);
  length = scan(fmt, arguments);
  va_end(arguments);
  return length;
}

void putChar(char c) { sysWrite(&c, 1, BLACK, WHITE, 0); }

void printString(char *string) {
  sysWrite(string, strlen(string), BLACK, WHITE, 0);
}

// https://www.techiedelight.com/implement-itoa-function-in-c/

int abs(int num) { return num < 0 ? -num : num; }

void swap(char *x, char *y) {
  char t = *x;
  *x = *y;
  *y = t;
}

// function to reverse buffer[i..j]
char *reverse(char *buffer, int i, int j) {
  while (i < j) swap(&buffer[i++], &buffer[j--]);

  return buffer;
}

// Iterative function to implement itoa() function in C
char *intToStr(int value, char *buffer, int base) {
  // invalid input
  if (base < 2 || base > 32) {
    return buffer;
  }

  // consider the absolute value of the number
  int n = abs(value);

  int i = 0;
  while (n) {
    int r = n % base;

    if (r >= 10) {
      buffer[i++] = 65 + (r - 10);
    } else {
      buffer[i++] = 48 + r;
    }

    n = n / base;
  }

  // if the number is 0
  if (i == 0) {
    buffer[i++] = '0';
  }

  // If the base is 10 and the value is negative, the resulting string
  // is preceded with a minus sign (-)
  // With any other base, value is always considered unsigned
  if (value < 0 && base == 10) {
    buffer[i++] = '-';
  }

  buffer[i] = '\0';  // null terminate string

  // reverse the string and return it
  return reverse(buffer, 0, i - 1);
}

static int scan(char const *fmt, va_list arg) {
  int int_temp;
  char char_temp;
  char *string_temp;

  char ch;
  int length = 0;

  char buffer[512];

  while ((ch = *fmt++)) {
    if ('%' == ch) {
      switch (ch = *fmt++) {
        case '%':
          putChar('%');
          length++;
          break;
        case 'c':
          char_temp = va_arg(arg, int);
          putChar(char_temp);
          length++;
          break;
        case 's':
          string_temp = va_arg(arg, char *);
          printString(string_temp);
          length += strlen(string_temp);
          break;
        case 'd':
          int_temp = va_arg(arg, int);
          intToStr(int_temp, buffer, 10);
          printString(buffer);
          length += strlen(buffer);
          break;
        case 'x':
          int_temp = va_arg(arg, int);
          intToStr(int_temp, buffer, 16);
          printString("0x");
          printString(buffer);
          length += strlen(buffer);
          break;
      }
    } else {
      putChar(ch);
      length++;
    }
  }
  return length;
}
