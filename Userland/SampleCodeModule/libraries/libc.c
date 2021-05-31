#include <libc.h>
#include <syscalls.h>
#include <stdarg.h>

#define BUFF_LEN 30
static int buffSize = 0; //cantidad de elementos del buffer
char buffer[BUFF_LEN]={0};
void putChar(char c){
    _syscall(SYS_WRITE_ID, (uint64_t)&c, 1, BLACK, WHITE, 0);
}


//https://stackoverflow.com/questions/54352400/implementation-of-printf-function
void printf(char *str, ...){
    va_list args;
    int i = 0, j = 0;
    char buff[100] = {0}, tmp[20];
    char *str_arg;
    va_start(args, str);

    while (str && str[i]) {
        if (str[i] == '%')
        {
            i++;
            switch (str[i])
            {
                case 'c':
                {
                    buff[j] = (char)va_arg(args, int);
                    j++;
                    break;
                }
                case 'd':
                {
                    intToStr(va_arg(args, int),tmp,10);
                    strcpy(&buff[j], tmp);
                    j += strlen(tmp);
                    break;
                }
                case 'x':
                {
                    intToStr(va_arg(args, int),tmp,16);
                    strcpy(&buff[j], tmp);
                    j += strlen(tmp);
                    break;
                }
                case 's':
                {
                    str_arg = (char *)va_arg(args, char *);
                    strcpy(&buff[j], str_arg);
                    j += strlen(str_arg);
                    break;
                }
                case '%':
                {
                      strcpy(&buff[j], "%");
                      j++;
                      break;
                }
            }
        } else if (str[i] == '\\') {
            i++;
            switch (str[i])
            {
                // case '\\':
                // {
                //     strcpy(&buff[j], "\\");
                //     j++;
                //     break;
                // }
                case 'n':
                {
                    newLine();
                    break;
                }
            }
        }
        else
        {
            buff[j] = str[i];
            j++;
        }
        i++;
    }

    _syscall(SYS_WRITE_ID, (uint64_t)buff, j, BLACK, WHITE, 0);
    va_end(args);
    return ;
}


//inspirado en https://iq.opengenus.org/how-printf-and-scanf-function-works-in-c-internally/
// int scan (char * str, ...)
// {
//     va_list vl;
//     int i = 0, j=0, ret = 0;
//     char tmp[20], c;
//     char *out_loc;
//     while(c != '') 
//     {
//         if (fread(&c, 1, 1, stdin)) //fread(&c, 1, 1, stdin)
//         {
//  	       buffer[i] = c;
//  	       i++;
//  	    }
//  	}
//  	va_start( vl, str );
//  	i = 0;
//  	while (str && str[i])
//  	{
//  	    if (str[i] == '%') 
//  	    {
//  	       i++;
//  	       switch (str[i]) 
//  	       {
//  	           case 'c': 
//  	           {
// 	 	           *(char *)va_arg( vl, char* ) = buffer[j];
// 	 	           j++;
// 	 	           ret ++;
// 	 	           break;
//  	           }
//  	           case 'd': 
//  	           {
// 	 	           *(int *)va_arg( vl, int* ) =strtol(&buff[j], &out_loc, 10);
// 	 	           j+=out_loc -&buffer[j];
// 	 	           ret++;
// 	 	           break;
//  	            }
//  	            case 'x': 
//  	            {
// 	 	           *(int *)va_arg( vl, int* ) =strtol(&buff[j], &out_loc, 16);
// 	 	           j+=out_loc -&buffer[j];
// 	 	           ret++;
// 	 	           break;
//  	            }
//  	        }
//  	    } 
//  	    else 
//  	    {
//  	        buffer[j] =str[i];
//             j++;
//         }
//         i++;
//     }
//     va_end(vl);
//     return ret;
// }



int strlen(const char *s){
    int i;
    for (i = 0; s[i] != 0; i++);
    return i;
}

int getChar(){
    char buff[2]={0};
    int ret =_syscall(SYS_READ_ID,buff, 2, 0, 0,0);
    if(ret<=0){
        return -1;
    }
    return buff[0]; 
}


//funcion encargada a actualizar el buffer de la shell y de imprimir en pantalla el char en cuestion
int readText(){
    int c;
    while((c=getChar()) !='\n'){
        
        if(c!=-1){
            if(buffSize < BUFF_LEN-1){
                buffer[buffSize++]=c;
            }
            putChar(c);
        }
    }
    newLine();
    buffer[buffSize++]='\0';
    return buffSize++;
}

void newLine(){
    putChar('\n');
}



char *strcpy(char *destino, const char *fuente)
{

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


// https://www.techiedelight.com/implement-itoa-function-in-c/

int abs(int num){
    return num < 0? -num : num;
}

// function to reverse buffer[i..j]
char *reverse(char *buffer, int i, int j)
{
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);

    return buffer;
}

void swap(char *x, char *y)
{
    char t = *x;
    *x = *y;
    *y = t;
}

// Iterative function to implement itoa() function in C
char* intToStr(int value, char* buffer, int base)
{
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


