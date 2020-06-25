/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ili9163.h
 * Author: mufufu
 *
 * Created on June 16, 2020, 8:30 PM
 */



#ifndef ILI9163_H
#define ILI9163_H

#ifdef __cplusplus
 "C" {
#endif

#include <zephyr.h>   
#include "ili9163_registers.h"
#include "colors.h"
#include "spicomms.h"


    typedef struct {
        const uint8_t *data;
        uint8_t image_width;
        int image_datalen;
    } tImage;

    typedef struct {
        uint8_t char_code;
        const tImage *image;
    } tChar;
    
    typedef struct {
        uint8_t length;
        const tChar *chars;
        bool font_widthType;
        uint8_t font_height;
        uint8_t font_descent;
        bool rle;
        uint8_t remap_code;
    } tFont;

    typedef struct {
        const int16_t *data; //R5G6B5
        uint8_t image_width;
        uint8_t image_height;
        int16_t image_datalen;
        uint8_t image_depth;
        bool image_comp;
    } tPicture;    

    extern int16_t       _centerText;
    extern int16_t  _textScaleX;
    extern int16_t  _textScaleY;
    extern  const tFont *_currentFont;
    extern bool  _portrait;
    extern int16_t  _cursorX;
    extern int16_t  _cursorY;
    extern  int16_t _width;
    extern int16_t _height;
    extern int16_t  _spaceCharWidth;
    extern int16_t  _charSpacing;
    extern int16_t  _fontRemapOffset;
    extern int16_t  _fontInterline;
    extern uint16_t  _textBackground;
    extern uint16_t  _textForeground;
    extern bool  _textWrap;
    extern int16_t charW;


#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

#define swapVals(a, b) { int16_t t = a; a = b; b = t; }
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))    


    void tft_initDisplay();
    void tft_setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    void tft_fillScreen(uint16_t color16);
    void tft_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void tft_drawFastVLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void tft_pushColors(uint16_t color16, int16_t times);
    void tft_drawPixel(int16_t x, int16_t y, uint16_t color);
    void tft_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1, uint16_t color2);
    void tft_drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color);
    int  tft_getCharCode(uint8_t ch);
    void tft_setFont(const tFont *font);
    bool tft_renderSingleChar(const char c);
    int tft_STRlen_helper(const char* buffer,int len);
    void tft_textWrite(const char* buffer, int16_t len);
    void tft_setCursor(int16_t x, int16_t y);


#ifdef __cplusplus
}
#endif

#endif /* ILI9163_H */

