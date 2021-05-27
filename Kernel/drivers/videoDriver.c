#include <videoDriver.h>
#include <stdint.h>


unsigned int WIDTH = 1024;
unsigned int HEIGHT= 768;
unsigned int PIXEL_SIZE=3; //bytes por pixel 


//codigo basado de https://wiki.osdev.org/User:Omarrx024/VESA_Tutorial
unsigned int DEFAULT_BG_COLOUR=0X0;
unsigned int DEFAULT_FONT_COLOUR=0XFFFF;
struct vbe_mode_info_structure{
    uint16_t attributes;    // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;       // deprecated
    uint8_t window_b;       // deprecated
    uint16_t granularity;   // deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;  // deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;         // number of bytes per horizontal line
    uint16_t width;         // width in pixels
    uint16_t height;        // height in pixels
    uint8_t w_char;         // unused...
    uint8_t y_char;         // ...
    uint8_t planes;
    uint8_t bpp;   // bits per pixel in this mode
    uint8_t banks; // deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer; //puntero al buffer de la memoria de video physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__((packed));

static int getPixData(uint32_t x, uint32_t y);

//static struct vbe_mode_info_structure *screen_info = (void *)0x5C00;

struct  vbe_mode_info_structure * screenData = (void*) 0x5C00; //direccion de memoria donde esta la informacion de modo video

void initialize(){//POR AHORA LO DEJO A VALORES DEFAULT PERO DESPUES POR PARAMETRO RECIBIR BACKGROUND COLOR Y FONT COLOR
	WIDTH = screenData->width;
    HEIGHT = screenData->height;
    PIXEL_SIZE= screenData->bpp / 8;
    
}
static uint8_t * currentVideo = (uint8_t*)0xB8000;

void putpixel(int x, int y, int colour)
{
    char *currentFrame = (char *)((uint64_t)screenData->framebuffer);
    unsigned int offset=getPixData(x,y);

    currentFrame[offset] = colour & 255; //azul
    currentFrame[offset + 1] = (colour >> 8) & 255; // verde
    currentFrame[offset + 2] = (colour >> 16) & 255; // rojo
}


static int getPixData(uint32_t x, uint32_t y){
    return (x + y*WIDTH) * PIXEL_SIZE;
}

void printchar(char c, int fontColour,int bgColour  )

//PRE TP MODO TEXTO
// void printChar(char character, int colour) {
// 	*currentVideo = character;
// 	currentVideo += 1;
// 	*currentVideo = colour;
// 	currentVideo += 1;
// }

// void printString(const char * string) {
// 	int i;
// 	for(i=0; string[i] != 0; i++){
// 		printChar(string[i], 0xF2);
// 	}
// }