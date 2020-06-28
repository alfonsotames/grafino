/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "i2ccomms.h"

struct device *i2c_dev;




void startI2CCommCenter() {
    i2c_dev = device_get_binding(I2C_DEV);
    
    i2c_configure(i2c_dev, I2C_SPEED_STANDARD | I2C_MODE_MASTER);
}

void i2c_test() {
    //printk("*** I2C Test ***\n");
    
    struct i2c_msg msgs[2];
    
    uint8_t data[6];
    
    uint8_t cmd[1] = {0xAF};
    
    msgs[0].buf = cmd;
    msgs[0].len = 1U;
    msgs[0].flags = I2C_MSG_WRITE;
    
    msgs[1].buf = data;
    msgs[1].len = 6;
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;
    
    
    
//    i2c_transfer(i2c_dev, msgs, 2U, TP_ADDR);
    
    
    
    
}