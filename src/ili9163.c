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


    int16_t       _centerText = 0;
    int16_t  _textScaleX = 1;
    int16_t  _textScaleY = 1;
      const tFont *_currentFont;
    bool  _portrait = false;
    int16_t  _cursorX=0;
    int16_t  _cursorY=0;
     int16_t _width =128;
    int16_t _height=160;
    int16_t  _spaceCharWidth = 0;
    int16_t  _charSpacing = 0;
    int16_t  _fontRemapOffset = 0;
    int16_t  _fontInterline=0;
    uint16_t  _textBackground = BLACK;
    uint16_t  _textForeground = WHITE;
    bool  _textWrap = false; // mucho mas rapido cuando es falso
    int16_t charW = 0;

     
    

/***************************** ILI9163 INIT **********************************/
void tft_initDisplay() {
    
    //printk("Init Display...\n");



     
    tft_setFont(&temp);

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


void tft_setAddrWindow(int16_t x0,int16_t y0, int16_t x1, int16_t y1) {
    
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
    int bufflen = 2560;
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
    if (faltan > 0) {
        transmitData(&buffer, faltan);
    }
    endDataStream();    
}

void tft_drawPixel(int16_t x, int16_t y, uint16_t color) {
    tft_setAddrWindow(x, y, x+1, y+1);
    startDataStream();
    sendData16(color);
    endDataStream();
}

void tft_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    tft_setAddrWindow(x, y, x+w-1, y);
    tft_pushColors(color,w);
}

void tft_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    tft_setAddrWindow(x, y, x, y+h-1);
    tft_pushColors(color,h);
}


void tft_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1, uint16_t color2) {
    if (w < 2 && h < 2) {
        tft_drawPixel(x, y, color1);
        return;
    }
    if (h < 2) {
        tft_drawFastHLine(x, y, w, color1);
        return;
    }
    if (w < 2) {
        tft_drawFastVLine(x, y, h, color1);
        return;
    }
    tft_setAddrWindow(x,y,(x+w)-1,(y+h)-1);
    tft_pushColors(color1, w * h);
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


void tft_setCursor(int16_t x, int16_t y) {
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
									int16_t 			charW,
									int16_t 		x,
									int16_t 		y,
									int16_t 		scaleX,
                                                                        int16_t 		scaleY,
									int16_t 		currentYposition,
									uint16_t 		foreColor
									)
{
        //printk("tft_charLineRender\n");
	int16_t xlinePos = 0;
	int16_t px;
	int16_t endPix = 0;
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
						foreColor,
						foreColor
					);
					
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
									int16_t 	x,
									int16_t 	y,
									int16_t 		glyphWidth,
									int16_t 		glyphHeight,
									int16_t 	scaleX,
									int16_t	 	scaleY,
									int16_t 	totalBytes,
									int16_t 	cspacing,
									uint16_t 	foreColor,
									uint16_t 	backColor,
									bool 		inverse)
{
    //printk("tft_glyphRender_unc x: %" PRId16 "\n",x);
	//start by getting some glyph data...
	int16_t i;
	int16_t temp = 0;
	//some basic variable...
	int16_t currentXposition = 0;//the current position of the writing cursor in the x axis, from 0 to glyphWidth
	int16_t currentYposition = 0;//the current position of the writing cursor in the y axis, from 0 to _FNTheight
	int16_t tempYpos = 0;
	int16_t currentByte = 0;//the current byte in reading (from 0 to totalBytes)
	bool lineBuffer[glyphWidth+1];//the temporary line buffer
	int lineChecksum = 0;//part of the optimizer
	//Fill background if needed.
	if (foreColor != backColor) tft_fillRect(x,y,((glyphWidth * scaleX) + (cspacing * scaleX)),(glyphHeight * scaleY),backColor,foreColor);
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
							foreColor,
							backColor
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
    printk("pre  var: %i,%i\n",_cursorX,_cursorY); 
    //printk("tft_renderSingleChar\n");
	int16_t borderRight = 0;
	int16_t borderBottom = 0;
	if (c == 13){//------------------------------- CARRIAGE (detected) -----------------------
		return 0;//ignore, always OK
	} else if (c == 10){//------------------------- NEW LINE (detected) -----------------------
		if (!_portrait){
			borderBottom = (_currentFont->font_height * _textScaleY) + (_fontInterline * _textScaleY);
			if (_cursorY + borderBottom  > _height) return 1;//too high!
			_cursorX = 0;
			_cursorY += borderBottom;
		} else {//portrait
			borderBottom = (_currentFont->font_height * _textScaleX) + (_fontInterline * _textScaleX);
			if (_cursorX + borderBottom  > _width) return 1;//too high!
			_cursorX += borderBottom;
			_cursorY = 0;
		}
		return 0;
	} else if (c == 32){//--------------------------- SPACE (detected) -----------------------
		if (!_portrait){
			borderRight = (_spaceCharWidth * _textScaleX) + (_charSpacing * _textScaleX);
			if (_textForeground != _textBackground) {//fill the space
				if (_cursorX + borderRight >= _width) borderRight = _width - _cursorX;
				tft_fillRect(
					_cursorX,_cursorY,
					borderRight + (_charSpacing * _textScaleX),
					(_currentFont->font_height * _textScaleY),
					_textBackground,
					_textBackground
				);
			}
			_cursorX += borderRight;
			return 0;
		} else {//portrait
			borderRight = (_spaceCharWidth * _textScaleY) + (_charSpacing * _textScaleY);
			if (_textForeground != _textBackground) {//fill the space
				if (_cursorY + borderRight >= _height) borderRight = _height - _cursorY;
				tft_fillRect(
					_cursorY,_cursorX,
					borderRight,
					(_currentFont->font_height * _textScaleX),
					_textBackground,
					_textBackground
				);
			}
			_cursorY += borderRight;
			return 0;
		}
	} else {//-------------------------------------- CHAR  (detected) -------------------------
		int charIndex = tft_getCharCode(c);//get char code
		if (charIndex > -1){//check if it's valid
			charW = 0;
			//I need to know the width...

				charW = _currentFont->chars[charIndex].image->image_width;

			//---------------------------------- WRAP is ON? --------------------------------
			if (_textWrap){//wrap, goes in the new line 
                            //printk("_cursorX = %d  charW = %d  _textScaleX = %d  _textScaleY = %d  _charSpacing=%d  suma = %d\n",_cursorX,charW,_textScaleX,_textScaleY,_charSpacing,(_cursorX + (charW * _textScaleX) + (_charSpacing * _textScaleX)));
				if (!_portrait && (_cursorX + (charW * _textScaleX) + (_charSpacing * _textScaleX)) >= _width){
					_cursorX = 0;
					_cursorY += (_currentFont->font_height * _textScaleY) + (_fontInterline * _textScaleY);
				} else if (_portrait && (_cursorY + (charW * _textScaleY) + (_charSpacing * _textScaleY)) >= _width){
					_cursorX += (_currentFont->font_height * _textScaleX) + (_fontInterline * _textScaleX);
					_cursorY = 0;
				}
			} else {//not wrap, will get rid of the data
				if (_portrait){
					if (_cursorY + (charW * _textScaleY) + (_charSpacing * _textScaleY) >= _width) return 1;
				} else {
					if (_cursorX + (charW * _textScaleX) + (_charSpacing * _textScaleX) >= _width) {
                                            printk("******* REGRESO 1 *******");
                                            return 1;
                                        }
				}
			}
			//-------------------------Actual single char drawing here -----------------------------------

				const _smCharType * charGlyp = _currentFont->chars[charIndex].image->data;
				int			  totalBytes = _currentFont->chars[charIndex].image->image_datalen;

			if (!_portrait){
				if (_cursorY + (_currentFont->font_height * _textScaleY) > _height) {
                                    //printk("Too High! %d > %d\n",_cursorY + (_currentFont->font_height * _textScaleY),_height);
                                    return 1;//too high!
                                }
                                
                                /*
                                printk("Invoking glyphRender for %c   _cursorX = %d"
                                        "    _cursorY = %d   _textForeground = %x    "
                                        "_textBackground = %x\n",c,_cursorX,_cursorY,_textForeground,_textBackground);
                                */
                                //printk("char: %i  _cursorX = %i _cursorY = %i  charW = %i _textScaleX = %i  _textScaleY = %i  _charSpacing=%i \n",c,_cursorX,charW,_textScaleX,_textScaleY,_charSpacing);

                                printk("post var: %i,%i\n",_cursorX,_cursorY); 
                                k_msleep(10);
                                
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
				_cursorX += _cursorX + (charW * _textScaleX) + (_charSpacing * _textScaleX);//add charW to total
                               
			} else {
				if (_cursorX + (_currentFont->font_height * _textScaleX) > _width) return 1;//too high!
				tft_glyphRender_unc(
								charGlyp,
								_cursorY,
								_cursorX,
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
				_cursorY += (charW * _textScaleX) + (_charSpacing * _textScaleY);//add charW to total

			}
			return 0;
		}//end valid
		return 0;
	}//end char
}


int tft_STRlen_helper(const char* buffer,int len)
{
    //printk("tft_STRlen_helper\n");
	int charIndex = -1;
	int i;
	//if (len < 1) len = strlen(buffer);		//try to get data from string
	//if (len < 1) return 0;					//better stop here
	if (_currentFont->font_widthType != 0){		// fixed width font
		return ((len * _spaceCharWidth));
	} else {								// variable width, need to loop trough entire string!
		int totW = 0;
		for (i = 0;i < len;i++){			//loop trough buffer
			if (buffer[i] == 32){			//a space
				totW += _spaceCharWidth;
			} else if (buffer[i] != 13 && buffer[i] != 10 && buffer[i] != 32){//avoid special char
				charIndex = tft_getCharCode(buffer[i]);
				if (charIndex > -1) {		//found!
					#if defined(_FORCE_PROGMEM__)
						totW += (pgm_read_byte(&(_currentFont->chars[charIndex].image->image_width)));
					#else
						totW += (_currentFont->chars[charIndex].image->image_width);
					#endif
					totW += _charSpacing;
				}
			}//inside permitted chars
		}//buffer loop
		return totW;
	}//end variable w font
}

void tft_textWrite(const char* buffer, int16_t len)
{
    //printk("tft_textWrite\n");
    //printk("_width: %d   _height: %d\n\n",_width,_height);
    //printk("buffer = %s\n",buffer);
    //printk("**** TEXT WRITE **** \n\n");
	int16_t i;
	if (len < 1) len = strlen(buffer);//try get the info from the buffer
	if (len < 1) return;//better stop here, the string it's really empty!
	// Center text flag enabled
        //printk("len mide %d\n",len);
        /*
	if (_centerText > 0){
		uint8_t stringWide = (tft_STRlen_helper(buffer,len) * _textScaleX) / 2;
		uint8_t strMidHeight = (((_currentFont->font_height - _currentFont->font_descent) * _textScaleY) / 2);
		if (_centerText < 4) {
			//absolute
			if (_portrait){
				if (_centerText > 1)  _cursorX = (_height / 2) - strMidHeight;
				if (_centerText != 2) _cursorY = (_width / 2) - stringWide;
			} else {
				if (_centerText > 1)  _cursorY = (_height / 2) - strMidHeight;
				if (_centerText != 2) _cursorX = (_width / 2) - stringWide;
			}
		} else {
			//relative
			if (_portrait){
				if (_centerText > 4)  _cursorX = _cursorX - strMidHeight;
				if (_centerText != 5) _cursorY = _cursorY - stringWide;
			} else {
				if (_centerText > 4)  _cursorY = _cursorY - strMidHeight;
				if (_centerText != 5) _cursorX = _cursorX - stringWide;
			}
		}
		if (_cursorX < 0)_cursorX = 0;
		if (_cursorY < 0)_cursorY = 0;
		_centerText = 0;//
	}//end center flag
*/
	//Loop trough every char and write them one by one until end (or a break!)

        //revisar porque quite el inicio de la transaccion
        //printk("Sigo vivo y len vale %d!!!!!!\n\n",len);
	for (i=0;i<len;i++){
                                //printk("Renderando la letra: %d\n",buffer[i]);
                    //printk("_cursorX: %" PRId16 "\n",_cursorX);
            
            
            
		if (tft_renderSingleChar(buffer[i])) {

			//aha! in that case I have to break out!
			break;
		}
               
                
		/*

	*/
	}//end loop

}




        
        