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
#include <zephyr.h>   
#include "ili9163_registers.h"
#include "colors.h"
#include "spicomms.h"

#ifndef ILI9163_H
#define ILI9163_H

#ifdef __cplusplus
extern "C" {
#endif

 
    
void tft_initDisplay();
void tft_setAddrWindow(u16_t x0,u16_t y0, u16_t x1, u16_t y1);
void tft_fillScreen(u16_t color16);



#ifdef __cplusplus
}
#endif

#endif /* ILI9163_H */

