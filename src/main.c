/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>
#include <string.h>
#include "grafino.h"

#include "dotstar.h"
#include "ili9163.h"
#include "colors.h"


#define GPIO_DRV_NAME DT_LABEL(DT_ALIAS(port_a))



/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   250

#define GPIO_DRV_NAME DT_LABEL(DT_ALIAS(port_a))
#define SPI_DEV DT_LABEL(DT_ALIAS(spi_0))




void main(void) {


    /************************** GPIO Init *************************************/


    gpio_dev = device_get_binding(GPIO_DRV_NAME);
    if (gpio_dev == NULL) {
        //printk("Could not get %s device\n", GPIO_DRV_NAME);
        return;
    }

    /*****************************Indicators**********************************/
    // DotStar
    gpio_pin_configure(gpio_dev, 1, GPIO_OUTPUT);
    gpio_pin_configure(gpio_dev, 0, GPIO_OUTPUT);
    // Led
    gpio_pin_configure(gpio_dev, 17, GPIO_OUTPUT_ACTIVE);
    
    /*****************************Backlight**********************************/
    gpio_pin_configure(gpio_dev, 16, GPIO_OUTPUT_ACTIVE);

    //printk("SPI Init ...\n");

    startSPICommCenter();
    tft_initDisplay();

    
    //tft_drawPixel(30,60,RED);
    //tft_fillRect(30,80,1,20,MAGENTA,MAGENTA);
    //tft_drawLine(0,0,128,160,RED);
    
    u16_t colores[] = {RED,GREEN,BLUE,MAGENTA,YELLOW,CYAN,BLACK,MAGENTA};



    bool led_is_on = true;
    u8_t idx = 0;
    int cnt = 0;
    while (1) {


            tft_fillScreen(BLACK);

        
            


        int sx = 0;
        int sy = 0;
        for (int n = 0; n <= 14; n = n + 1) {
            tft_drawFastHLine(8, 10 + sy, 110, colores[cnt]);
            tft_drawFastVLine(8 + sx, 10, 140, colores[cnt]);
            if (sx < 110) sx = sx + 10;
            if (sy < 140) sy = sy + 10;
        }

        
         
        k_msleep(250);
       
            //tft_fillScreen(BLACK);
            tft_setCursor(25,(cnt*20)+2);
            
            tft_drawFastHLine(20,(cnt*20),80,RED);
            tft_drawFastHLine(20,(cnt*20)+46,80,RED);
            tft_drawFastVLine(20,(cnt*20),46,RED);
            tft_drawFastVLine(100,(cnt*20),46,RED);
            
            char buf[3];
            buf[0] = 'A';
            buf[1] = 'B';
            buf[2] = cnt+'0';
            
        tft_textWrite(buf, 3);
        
 



        

        /********************Leds and Whistles  *******************************/
        gpio_pin_set(gpio_dev, 17, (int) led_is_on);
        //printk("%d Serafino in C !\n", cnt);
        led_is_on = !led_is_on;
        send_rgb(gpio_dev, APA102C_START_FRAME);
        send_rgb(gpio_dev, rgb[idx]);
        send_rgb(gpio_dev, APA102C_END_FRAME);
        idx++;
        if (idx >= NUM_RGB) {
            idx = 0;
        }
        cnt++;
        if (cnt == 6) {
            cnt=0;
        }
        k_msleep(400);
    }
}
