#include <libc.h>
#include <syscalls.h>
#include <stdarg.h>
#include <shell.h>

static int currentBGC = BLACK;
static int currentFTC = WHITE;

#define BUFF_LEN 100

static int buffSize = 0;
static char buffer[BUFF_LEN]={0};
static int firstChange=1;
static int changedUserName=0;

void setFTC(t_color colour) {
    currentFTC = colour;
}

void setBGC(t_color colour) {
    currentBGC = colour;
}

void putChar(char c) {
    (void)_syscall(SYS_WRITE_ID, (uint64_t)&c, 1, currentBGC, currentFTC, 0);
}

void sendUserData(char *userName, int len) {
   (void)_syscall(SYS_WRITE_ID, (uint64_t)userName, len+1, currentBGC, currentFTC, (uint64_t) &len);
}
void setFirstChange(int number){
    if(number<0 || number>1)return;
    changedUserName=number;
}

//https://stackoverflow.com/questions/54352400/implementation-of-printf-function
void printf(char *str, ...) {
    va_list args;
    
    char buff[BUFF_LEN] = {0};
    int strIdx = 0, buffIdx = 0;
    char *str_arg;
    char aux[20];
    va_start(args, str);

    while (str && str[strIdx]) {
        if (str[strIdx] == '%')
        {
            strIdx++;
            switch (str[strIdx])
            {
                case 'c':
                {
                    buff[buffIdx] = (char)va_arg(args, int);
                    buffIdx++;
                    break;
                }
                case 'd':
                {
                    intToStr(va_arg(args, int),aux,10);
                    strcpy(&buff[buffIdx], aux);
                    buffIdx += strlen(aux);
                    break;
                }
                case 'x':
                {
                    intToStr(va_arg(args, int),aux,16);
                    strcpy(&buff[buffIdx], aux);
                    buffIdx += strlen(aux);
                    break;
                }
                case 's':
                {
                    str_arg = (char *)va_arg(args, char *);
                    strcpy(&buff[buffIdx], str_arg);
                    buffIdx += strlen(str_arg);
                    break;
                }
                case '%':
                {
                    strcpy(&buff[buffIdx], "%");
                    buffIdx++;
                    break;
                }
            }
         } else {
            buff[buffIdx] = str[strIdx];
            buffIdx++;
        }
        strIdx++;
    }

    (void)_syscall(SYS_WRITE_ID, (uint64_t)buff, buffIdx, currentBGC, currentFTC, 0);
    va_end(args);
    return ;
}



//https://stackoverflow.com/questions/26860574/pow-implementation-in-cmath-and-efficient-replacement
int pow(int x, unsigned int y) {
    if (y == 0)
        return 1;
    else if ((y % 2) == 0)
        return pow (x, y / 2) * pow (x, y / 2);
    else
        return x * pow (x, y / 2) * pow (x, y / 2);

}

static void reverseAUX(char* str, int len) {
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

static int intToStrAUX(int x, char str[], int d) {
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
  
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
  
    reverseAUX(str, i);
    str[i] = '\0';
    return i;
}

// https://www.geeksforgeeks.org/convert-floating-point-number-string/
// Converts a floating-point/double number to a string.
void doubleToStr(double n, char* res, int afterpoint) {
    // Extract integer part
    int ipart = (int)n;
  
    // Extract floating part
    double fpart = n - (double)ipart;

    if(n < 0 && n > -1) {
         res[0] = '-';
         intToStr(ipart, res + 1, 10);
    } else {
         intToStr(ipart, res, 10);
    }
  
    // convert integer part to string
   

    int resIdx = strlen(res);
  
    // check for display option after point
    if (afterpoint != 0) {
        res[resIdx] = '.'; // add dot
  
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
        if(n < 0)
            fpart *= -1;
  
        intToStrAUX((int)fpart, res + resIdx + 1, afterpoint);
    }
}


// inspirado en https://iq.opengenus.org/how-printf-and-scanf-function-works-in-c-internally/
int scanf(char * str, ...) {
    buffSize = 0;
    va_list vl;
    int i = 0, j = 0;
    int sizeNum = 0;
 	va_start( vl, str );
    char *str_arg;
    readText();

 	while (str && str[i] && buffer[j])
 	{
 	    if (str[i] == '%') 
 	    {
 	       i++;
 	       switch (str[i]) 
 	       {
 	            case 'c': 
 	            {
	 	            *(char *)va_arg( vl, char* ) = buffer[j];
	 	            j++;
	 	            break;
 	            }
 	            case 'd': 
 	            {
	 	            *(int *)va_arg( vl, int* ) = strToInt(&buffer[j], &sizeNum);
	 	            j+=sizeNum;
	 	            break;
 	            }
                case 's':
                { 
                    str_arg = (char *)va_arg(vl, char *);
                    strcpy(str_arg, &buffer[j]);
                    j += strlen(str_arg);      
                    break;
                }
                
 	        }
 	    } 
 	    else 
 	    {
 	        buffer[j] =str[i];
            j++;
        }
        i++;
    }
    va_end(vl);
    return j;
}


int strlen(const char *s) {
    int i;
    for (i = 0; s[i] != 0; i++);
    return i;
}

char getChar() {
    return _syscall(SYS_READ_ID, 0, 0, 0, 0, 0);
}


//funcion encargada a actualizar el buffer de la shell y de imprimir en pantalla el char en cuestion
int readText() {
    int c;
    while((c=getChar()) !='\n'){
        
        if(c!=-1){
            if(buffSize < BUFF_LEN-1){
                buffer[buffSize++]=c;
            }
            if(c=='\t'){
                if(firstChange){
                    firstChange=0;
                    printUser();    
                }else{
                    if(changedUserName){
                        newLine();
                    printUser();    

                        changedUserName=0;

                    }
                }
                buffSize=0;
            }else{
                if(c=='\b'){
                    buffSize-=2;
                }
                putChar(c);
            }
        }
    }
    newLine();
    buffer[buffSize++]='\0';
    return buffSize++;
}

void newLine() {
    putChar('\n');
}



char *strcpy(char *destino, const char *fuente) {

    char *aux = destino;

    while (*fuente != '\0')
    {
        *destino = *fuente;
        fuente++;
        destino++;
    }

    *destino = '\0';
    return aux;
}




// http://vivi.dyndns.org/tech/cpp/atohex.html

static int isdigit(char ch) {
    return ch >= '0' && ch <= '9';
}

static int ishexdigit(char ch) {
        return isdigit(ch) || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f');
}

//https://www.geeksforgeeks.org/write-your-own-atoi/
int strToInt(char* str, int * size) {
    *size = 0;
    // Initialize result
    int res = 0;
 
    // Initialize sign as positive
    int sign = 1;
 
    // Initialize index of first digit
    int i = 0;
 
    // If number is negative,
    // then update sign
    if (str[0] == '-') {
        sign = -1;
        *size += 1;
        // Also update index of first digit
        i++;
    }
 
    // Iterate through all digits
    // and update the result
    for (; str[i] != '\0'; ++i) {
        if (str[i] < '0' || str[i] > '9')
             return res;
        res = res * 10 + str[i] - '0';
        *size += 1;
    }
        
 
    // Return result with sign
    return sign * res;
}

int strToHex(const char *str) {
    int val = 0;      
    while( *str != 0 ) {  
        if( !ishexdigit(*str) ) { 
            return -1;
        }
        if( isdigit(*str) ) {    
            val = val * 16 + *str++ - '0';      
        } else if( *str >= 'A' && *str <= 'F' ) {    
            val = val * 16 + *str++ - 'A' + 10;      
        } else {   
            val = val * 16 + *str++ - 'a' + 10;      
        }
    }
    return val;   
}

void strToDouble(char *numStr, double *result) {
      *result = 0;
      int i = 0, k = 0, sign = 0;
      double commaOffset = 0;
      char integerPart[BUFF_LEN] = {0};

    if (numStr[i] == '-' && numStr[i + 1] == '0') {
        sign = 1;
        i++;
    }

    for (k = 0; numStr[i] != 0 && numStr[i] != '.'; i++, k++) {
        integerPart[k] = numStr[i];
    }
    int aux;
    *result += strToInt(integerPart, &aux);
    if (numStr[i] == '.') {
        i++;
        for (; numStr[i] != 0; i++, commaOffset++) {
                *result *= 10;
                *result += numStr[i] - '0';
        }
        *result /= pow(10, commaOffset);
    }
    if (sign && numStr[1] == '0') {
        *result *= -1;
    }
}


// https://www.techiedelight.com/implement-itoa-function-in-c/

int abs(int num) {
    return num < 0? -num : num;
}

// function to reverse buffer[i..j]
char *reverse(char *buffer, int i, int j) {
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);

    return buffer;
}

void swap(char *x, char *y) {
    char t = *x;
    *x = *y;
    *y = t;
}

// Iterative function to implement itoa() function in C
char* intToStr(int value, char* buffer, int base) {
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // consider the absolute value of the number
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
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
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}

int strcmp(char string1[], char string2[]) {
    int i = 0;   
    while(string1[i]!=0 && string2[i]!=0){
        if(string1[i] < string2[i])
            return -1;
        if(string1[i] > string2[i])
            return 1;
        i++;
    }
    if(string1[i]==0 && string2[i]==0)
        return 0;
    if(string1[i]!=0 && string2[i]==0)
        return 1;
    return -1;
}

void *memset(void *b, int c, int len) {
      unsigned char *p = b;
      while (len > 0)
      {
            *p = c;
            p++;
            len--;
      }
      return (b);
}

void * malloc(uint64_t qty) {
    return (void *)_syscall(SYS_MALLOC_ID, qty, 0, 0, 0, 0);
}

void free(void * memory) {
    (void)_syscall(SYS_FREE_ID, (uint64_t)memory, 0, 0, 0, 0);
}