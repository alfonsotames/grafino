/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   dotstar.h
 * Author: mufufu
 *
 * Created on June 16, 2020, 8:22 PM
 */

/*********************** DotStar*******************************/

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   250



#define GPIO_DATA_PIN	1
#define GPIO_CLK_PIN	0
#define GPIO_NAME	"GPIO_"
#define GPIO_DRV_NAME	DT_LABEL(DT_ALIAS(port_a))
#define APA102C_START_FRAME	0x00000000
#define APA102C_END_FRAME	0xFFFFFFFF
#define NUM_RGB		3
#define NUM_LEDS	1
/* The LED is very bright. So to protect the eyes,
 * brightness is set very low, and RGB values are
 * set low too.
 */
u32_t rgb[] = {
	0x11FFFF00,
	0x1100FF00,
	0x11FF0000
};

void send_rgb(struct device *gpio_dev, u32_t rgb)
{
	int i;
	for (i = 0; i < 32; i++) {
		/* MSB goes in first */
		gpio_pin_set_raw(gpio_dev, GPIO_DATA_PIN, (rgb & BIT(31)) != 0);
		/* Latch data into LED */
		gpio_pin_set_raw(gpio_dev, GPIO_CLK_PIN, 1);
		gpio_pin_set_raw(gpio_dev, GPIO_CLK_PIN, 0);
		rgb <<= 1;
	}
}
/***************************************************************/