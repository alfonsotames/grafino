/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   i2ccomms.h
 * Author: mufufu
 *
 * Created on June 28, 2020, 1:54 AM
 */

#ifndef I2CCOMMS_H
#define I2CCOMMS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr.h>
#include <drivers/gpio.h>
#include <drivers/i2c.h>

    
#define I2C_DEV DT_LABEL(DT_ALIAS(i2c_0))
#define TP_ADDR 0x00 // ThermoPile Address
    
void i2c_test();
void startI2CCommCenter();


#ifdef __cplusplus
}
#endif

#endif /* I2CCOMMS_H */

