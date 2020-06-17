/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   grafino.h
 * Author: mufufu
 *
 * Created on June 16, 2020, 10:57 PM
 */


#include <zephyr.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>


#ifndef GRAFINO_H
#define GRAFINO_H

#ifdef __cplusplus
extern "C" {
#endif


 extern struct device *gpio_dev;
 extern struct device *spi_dev;
 extern struct spi_cs_control cs_ctrl;
 extern struct spi_config spi_cfg;

    



#ifdef __cplusplus
}
#endif

#endif /* GRAFINO_H */

