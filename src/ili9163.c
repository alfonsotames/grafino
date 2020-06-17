/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ili9163.h"




/***************************** ILI9163 INIT **********************************/
void tft_initDisplay() {
    
    printk("Init Display...\n");

    //Software reset -----
    transmitCommand(CMD_SWRESET);
    k_msleep(122); //500
    transmitCommand(CMD_SLPOUT);
    k_msleep(5);
    transmitCommand(CMD_IDLEOF);
    transmitCommand(CMD_DISPOFF);
    
    u8_t pwc1[] = {0x1D,0x02};
    transmit(CMD_PWCTR1, &pwc1, 2);
    
    u8_t pwc2[] = {0x02};
    transmit(CMD_PWCTR2, &pwc2, 1);
    
    u8_t pwc3[] = {0x01};
    transmit(CMD_PWCTR3, &pwc3, 1);
    
    u8_t pwc4[] = {0x01};
    transmit(CMD_PWCTR4, &pwc4, 1);

    u8_t pwc5[] = {0x01};
    transmit(CMD_PWCTR5, &pwc5, 1);    
    
    u8_t vcom1[] = {0x24,0x48};
    transmit(CMD_VCOMCTR1, &vcom1, 2);
    
    u8_t dfunc[] = {0x3F, 0x01};
    transmit(CMD_DFUNCTR, &dfunc, 2);
    
    u8_t frmctr1[] = {0x08, 0x02};
    transmit(CMD_FRMCTR1, &frmctr1, 2);
    
    u8_t frmctr2[] = {0x08, 0x02};
    transmit(CMD_FRMCTR2, &frmctr2, 2);
    
    u8_t frmctr3[] = {0x08, 0x02};
    transmit(CMD_FRMCTR3, &frmctr3, 2);
    
    u8_t pxl_fmt = {0x65};
    transmit(CMD_PIXFMT, &pxl_fmt, 1);

    u8_t gammac[] = {0x08};
    transmit(CMD_GAMMASET, &gammac, 1);
    
    u8_t gammad[] = {0x01};
    transmit(CMD_GAMRSEL, &gammad, 1);

    u8_t pgammac[] = {0x3F,0x26,0x23,0x30,0x28,0x10,0x55,0xB7,0x40,0x19,0x10,0x1E,0x02,0x01,0x00};
    transmit(CMD_PGAMMAC, &pgammac, 15);
    
    u8_t ngammac[] = {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
    transmit(CMD_NGAMMAC, &ngammac, 15);
    
    u8_t coladdset[] = {0x00,0x00,0x00,0x80};
    transmit(CMD_CLMADRS, &coladdset, 4);
    
    u8_t pageaddset[] = {0x00,0x00,0x00,0xA0};
    transmit(CMD_PGEADRS, &pageaddset, 4);
    
    u8_t dinvctr[] = {0x07};
    transmit(CMD_DINVCTR, &dinvctr, 1);
    
    transmitCommand(CMD_NORML);
    
    transmitCommand(CMD_DISPON);
    
    //setRotation(0);
    
    //setScrollDirection(0);
    k_msleep(10);
    u8_t vscrolldata[] = {0x00,0x00,0x00,0xA0,0x00,0x00};
    transmit(CMD_VSCLLDEF, &vscrolldata, 6);
    u8_t madctl[] = {0x08};
    transmit(CMD_MADCTL,&madctl,1);
    
    //printk("SPI Initialization finished...\n\n");
    //printk("Frquency set at %d\n",spi_cfg.frequency);
    
}


void tft_setAddrWindow(u16_t x0,u16_t y0, u16_t x1, u16_t y1) {
    
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

void tft_fillScreen(u16_t color16) {
    u8_t bytes[2];
    bytes[0] =  *((u8_t*)&(color16)+1); //high byte
    bytes[1] =  *((u8_t*)&(color16)+0); //low byte
    tft_setAddrWindow(0,0,128,160);
    startDataStream();
    //We must send (TFT_ILI9163C_W*TFT_ILI9163C_W) * 2 bytes
    // buffer size for 16 sends
    int bufflen = 2560;
    u8_t buffer[bufflen];
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