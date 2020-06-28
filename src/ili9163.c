/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdlib.h>
#include <string.h>
#include "ili9163.h"
#include "fonts/nullfont.c"
#include "fonts/arial20.c"
#include "fonts/temp.c"
#include "icons/spkOn.c"


    uint8_t       _centerText = 0;
    volatile int  _textScaleX = 1;
    volatile int  _textScaleY = 1;
      const tFont *_currentFont;
    bool  _portrait = false;
     int  _cursorX=0;
     int  _cursorY=0;
     int16_t _width =128;
    int16_t _height=160;
    int  _spaceCharWidth = 0;
    volatile int  _charSpacing = 0;
    uint8_t  _fontRemapOffset = 0;
    int16_t  _fontInterline=0;
    uint16_t  _textBackground = BLACK;
    uint16_t  _textForeground = WHITE;
    bool  _textWrap = false; // mucho mas rapido cuando es falso
     int charW = 0;
    
    int16_t posX = 0;
     
    

/***************************** ILI9163 INIT **********************************/
void tft_initDisplay() {
    
    //printk("Init Display...\n");



     
    tft_setFont(&arial20);

    //Software reset -----
    transmitCommand(CMD_SWRESET);
    k_msleep(122); //500
    transmitCommand(CMD_SLPOUT);
    k_msleep(5);
    transmitCommand(CMD_IDLEOF);
    transmitCommand(CMD_DISPOFF);
    
    uint8_t pwc1[] = {0x1D,0x02};
    transmit(CMD_PWCTR1, &pwc1, 2);
    
    uint8_t pwc2[] = {0x02};
    transmit(CMD_PWCTR2, &pwc2, 1);
    
    uint8_t pwc3[] = {0x01};
    transmit(CMD_PWCTR3, &pwc3, 1);
    
    uint8_t pwc4[] = {0x01};
    transmit(CMD_PWCTR4, &pwc4, 1);

    uint8_t pwc5[] = {0x01};
    transmit(CMD_PWCTR5, &pwc5, 1);    
    
    uint8_t vcom1[] = {0x24,0x48};
    transmit(CMD_VCOMCTR1, &vcom1, 2);
    
    uint8_t dfunc[] = {0x3F, 0x01};
    transmit(CMD_DFUNCTR, &dfunc, 2);
    
    uint8_t frmctr1[] = {0x08, 0x02};
    transmit(CMD_FRMCTR1, &frmctr1, 2);
    
    uint8_t frmctr2[] = {0x08, 0x02};
    transmit(CMD_FRMCTR2, &frmctr2, 2);
    
    uint8_t frmctr3[] = {0x08, 0x02};
    transmit(CMD_FRMCTR3, &frmctr3, 2);
    
    uint8_t pxl_fmt = {0x65};
    transmit(CMD_PIXFMT, &pxl_fmt, 1);

    uint8_t gammac[] = {0x08};
    transmit(CMD_GAMMASET, &gammac, 1);
    
    uint8_t gammad[] = {0x01};
    transmit(CMD_GAMRSEL, &gammad, 1);

    uint8_t pgammac[] = {0x3F,0x26,0x23,0x30,0x28,0x10,0x55,0xB7,0x40,0x19,0x10,0x1E,0x02,0x01,0x00};
    transmit(CMD_PGAMMAC, &pgammac, 15);
    
    uint8_t ngammac[] = {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
    transmit(CMD_NGAMMAC, &ngammac, 15);
    
    uint8_t coladdset[] = {0x00,0x00,0x00,0x80};
    transmit(CMD_CLMADRS, &coladdset, 4);
    
    uint8_t pageaddset[] = {0x00,0x00,0x00,0xA0};
    transmit(CMD_PGEADRS, &pageaddset, 4);
    
    uint8_t dinvctr[] = {0x07};
    transmit(CMD_DINVCTR, &dinvctr, 1);
    
    transmitCommand(CMD_NORML);
    
    transmitCommand(CMD_DISPON);
    
    //setRotation(0);
    //setScrollDirection(0);
    k_msleep(10);
    uint8_t vscrolldata[] = {0x00,0x00,0x00,0xA0,0x00,0x00};
    transmit(CMD_VSCLLDEF, &vscrolldata, 6);
    uint8_t madctl[] = {0x08};
    transmit(CMD_MADCTL,&madctl,1);
    

    
}


void tft_setAddrWindow(int x0, int y0, int x1, int y1) {

    
    // Cols
    transmitCommand(CMD_CLMADRS);
    startDataStream();
    sendData16(x0);
    sendData16(x1);
    endDataStream();
    
    // Pages
    transmitCommand(CMD_PGEADRS);
    startDataStream();
    sendData16(y0);
    sendData16(y1);
    endDataStream();
    
    // Ram Write
    transmitCommand(CMD_RAMWR);
}

void tft_fillScreen(uint16_t color16) {
    uint8_t bytes[2];
    bytes[0] =  *((uint8_t*)&(color16)+1); //high byte
    bytes[1] =  *((uint8_t*)&(color16)+0); //low byte
    tft_setAddrWindow(0,0,128,160);
    startDataStream();
    //We must send (TFT_ILI9163C_W*TFT_ILI9163C_W) * 2 bytes
    // buffer size for 16 sends
    int bufflen = 2560;
    uint8_t buffer[bufflen];
    int i=0;
    int c=0;
    while (c < 40960) {
        buffer[i] = bytes[0]; i++;
        buffer[i] = bytes[1]; i++;
        c++;
        if (i == bufflen) {
            transmitData(&buffer, i);
            i=0;
        }
    }
    endDataStream();
}


void tft_pushColors(uint16_t color16, int16_t times) {
    
    
    uint8_t bytes[2];
    bytes[0] =  *((uint8_t*)&(color16)+1); //high byte
    bytes[1] =  *((uint8_t*)&(color16)+0); //low byte
    startDataStream();
    int bufflen = 512;
    uint8_t buffer[bufflen];
    int i=0;
    int c=0;
    times = (times+1)*2; // dos bytes por color! (16bit)
    int faltan=times;
    while (c < times) {
        buffer[i] = bytes[0]; i++;
        buffer[i] = bytes[1]; i++;
        c++;
        if (i == bufflen) {
            transmitData(&buffer, i);
            faltan=faltan-i;
            i=0;
        }
    }
    //printk("Faltan: %d\n",faltan);
    if (faltan > 0) {
        transmitData(&buffer, faltan);
    }
    endDataStream();
          
}

void tft_drawPixel(int x, int y, uint16_t color) {
    tft_setAddrWindow(x, y, x+1, y+1);
    startDataStream();
    sendData16(color);
    endDataStream();
}

void tft_drawFastHLine(int x, int y, int w, uint16_t color) {
    tft_setAddrWindow(x, y, x+w-1, y);
    tft_pushColors(color,w);
}

void tft_drawFastVLine(int x, int y, int h, uint16_t color) {
    tft_setAddrWindow(x, y, x, y+h-1);
    tft_pushColors(color,h);
}


void tft_fillRect(int x, int y, int w, int h, uint16_t color) {
    
    if (w < 2 && h < 2) {
        tft_drawPixel(x, y, color);
        return;
    }
    if (h < 2) {
        tft_drawFastHLine(x, y, w, color);
        return;
    }
    if (w < 2) {
        tft_drawFastVLine(x, y, h, color);
        return;
    }
     
    tft_setAddrWindow(x,y,(x+w)-1,(y+h)-1);
    tft_pushColors(color, w * h);
}


void tft_drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }
  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }
  int16_t dx, dy;
  dx = (x1 - x0);
  dy = (abs(y1 - y0));
  int16_t err = dx / 2;
  int16_t ystep;
  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }
  for (; x0 < x1; x0++) {
    if (steep) {
      tft_drawPixel(y0, x0, color);
    } else {
      tft_drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}



/************************** Text Rendering Functions **************************/


void tft_setCursor(int x, int y) {
    // por que esta al reves!???
    _cursorX = x;
    _cursorY = y;
}

int tft_getCharCode(uint8_t ch)
{

	int i;
	//remap ?
	if (_fontRemapOffset == 1 && (ch > 96 && ch < 123)){
		ch -= 32;
	} else if (_fontRemapOffset == 2 && ((ch > 64 && ch < 91))){
		ch += 32;
	}
	//search for char code
	for (i = 0;i < _currentFont->length;i++){
		if (_currentFont->chars[i].char_code == ch) return i;
	}
	return -1;
}


void tft_setFont(const tFont *font) 
{
	_currentFont = font;
	_fontRemapOffset =  _currentFont->remap_code;
	//get all needed infos
	// We just get the space width now...
	int temp = tft_getCharCode(0x20);
	if (temp > -1){
		#if defined(_FORCE_PROGMEM__)
			_spaceCharWidth = pgm_read_byte(&(_currentFont->chars[temp].image->image_width));
		#else
			_spaceCharWidth = (_currentFont->chars[temp].image->image_width);
		#endif
	} else {
		//font malformed, doesn't have needed space parameter will return to system font
		#if defined(_ILI9163C_DEF_FONT_PATH)
			setFont(&_ILI9163C_DEF_FONT_NAME);
		#else
			tft_setFont(&nullfont);
		#endif
		return;
	}
}


void tft_charLineRender(
									bool 			lineBuffer[],
									int 			charW,
									int 		x,
									int 		y,
									int 		scaleX,
									int 		scaleY,
									int 		currentYposition,
									uint16_t 		foreColor
									)
{
	int xlinePos = 0;
	int px;
	int endPix = 0;
	bool refPixel = false;
        
	while (xlinePos < charW){
		refPixel = lineBuffer[xlinePos];//xlinePos pix as reference value for next pixels
		//detect and render concurrent pixels
		for (px = xlinePos;px <= charW;px++){
			if (lineBuffer[px] == lineBuffer[xlinePos] && px < charW){
				//grouping pixels with same val
				endPix++;
			} else {
				
				if (refPixel) {
                                    
						tft_fillRect(
						x,
						y + (currentYposition * scaleY),
						(endPix * scaleX),
						scaleY,
						WHITE);
                                                
					
				} else {
					//do nothing
				}
				//reset and update some vals
				xlinePos += endPix;
				x += endPix * scaleX;
				endPix = 0;
				break;//exit cycle for...
			}
		}
	}//while
        
}




void tft_glyphRender_unc(
									const 		_smCharType *pixelsArray,
									int 	x,
									int 	y,
									int 		glyphWidth,
									int 		glyphHeight,
									int 	scaleX,
									int	 	scaleY,
									int 	totalBytes,
									int 	cspacing,
									uint16_t 	foreColor,
									uint16_t 	backColor,
									bool 		inverse)
{
    //printk("tft_glyphRender_unc x: %" PRId16 "\n",x);
	//start by getting some glyph data...
	int i;
	uint8_t temp = 0;
	//some basic variable...
	int currentXposition = 0;//the current position of the writing cursor in the x axis, from 0 to glyphWidth
	int currentYposition = 0;//the current position of the writing cursor in the y axis, from 0 to _FNTheight
	int tempYpos = 0;
	int currentByte = 0;//the current byte in reading (from 0 to totalBytes)
	bool lineBuffer[glyphWidth+1];//the temporary line buffer
	int lineChecksum = 0;//part of the optimizer
	//Fill background if needed.
	//if (foreColor != backColor) tft_fillRect(x,y,((glyphWidth * scaleX) + (cspacing * scaleX)),(glyphHeight * scaleY),backColor,foreColor);
	//the main loop that will read all bytes of the glyph
	while (currentByte < totalBytes){
		//read n byte

		temp = pixelsArray[currentByte];

		if (inverse) temp = ~temp;//invert byte if needed
		//read current bits inside current byte
		for (i = 7; i >= 0; i--){
			lineBuffer[currentXposition] = bitRead(temp,i);//continue fill line buffer
			lineChecksum += lineBuffer[currentXposition++];
			//----------------------------------- exception
			if (currentXposition == glyphWidth){
				//line buffer has been filled!
				currentXposition = 0;//reset the line x position
				tempYpos = y + (currentYposition * scaleY);
				if (lineChecksum < 1){
					//do nothing
				} else if (lineChecksum == glyphWidth){
					//full line
					tft_fillRect(
							x,
							tempYpos,
							(glyphWidth * scaleX),
							scaleY,
							foreColor
					);
				} else { 
					//line render
                                    
					tft_charLineRender(
							lineBuffer,
							glyphWidth,
							x,
							y,
							scaleX,
							scaleY,
							currentYposition,
							foreColor
					);
                                     
				}
				currentYposition++;//next line
				lineChecksum = 0;//reset checksum
			}//end exception
		}//end reading single byte
		currentByte++;
	}
}


bool tft_renderSingleChar(const char c)
{


		int charIndex = tft_getCharCode(c);//get char code
		if (charIndex > -1){//check if it's valid
			int charW = 0;
			//I need to know the width...

				charW = _currentFont->chars[charIndex].image->image_width;

			//-------------------------Actual single char drawing here -----------------------------------

				const _smCharType * charGlyp = _currentFont->chars[charIndex].image->data;
				int totalBytes = _currentFont->chars[charIndex].image->image_datalen;

                                //printk("_cursorX: %d  charW: %d\n",_cursorX,charW);
                                
                                tft_glyphRender_unc(
								charGlyp,
								_cursorX,
								_cursorY,
								charW,
								_currentFont->font_height,
								_textScaleX,
								_textScaleY,
								totalBytes,
								_charSpacing,
								_textForeground,
								_textBackground,
								false
				);
				//_cursorX += _cursorX + (charW * _textScaleX) + (_charSpacing * _textScaleX);//add charW to total
                                
                                _cursorX += charW;
                                
                                
			return 0;

	}//end char
        return 1;        
}



void tft_textWrite(const char* buffer, int16_t len)
{
    //printk("\n\n\nRendering %s\n",buffer);
	uint16_t i;
	if (len < 1) len = strlen(buffer);//try get the info from the buffer
	if (len < 1) return;//better stop here, the string it's really empty!

	for (i=0;i<len;i++){

		if (tft_renderSingleChar(buffer[i])) {

			//aha! in that case I have to break out!
			break;
		}
               
	}//end loop

}



void tft_drawIcon(int x, int y,const tIcon *icon,uint8_t scale,uint16_t f,uint16_t b,bool inverse)
{

		const uint8_t* iconData	= icon->data;//icon data
		uint8_t		iWidth				= icon->image_width;
		uint8_t		iHeight				= icon->image_height;
		uint16_t	datalen				= icon->image_datalen;
		//uint8_t		dataComp		= icon->image_comp;//not yet


	//LGPO Rendering (uncomp)
	tft_glyphRender_unc(
					iconData,
					x,
					y,
					iWidth,
					iHeight,
					scale,
					scale,
					datalen,
					0,
					f,
					b,
					inverse
	);

}
        

void tft_drawIcons() {
    tft_drawIcon(80, 80,&spkOn, 1,WHITE,WHITE,false);
}