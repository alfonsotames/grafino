
/************************ SPI Communication Center ****************************/

#include "spicomms.h"
#include <zephyr.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>
#include "grafino.h"

  struct device *gpio_dev;
  struct device *spi_dev;
  struct spi_cs_control cs_ctrl;
  struct spi_config spi_cfg;

void startSPICommCenter() {
    //printk("Configuring pins...\n");
        if (gpio_dev == NULL) {
        //printk("GPIO Device not initialized!\n");
        return;
    }  
    
    gpio_pin_configure(gpio_dev, 7, GPIO_OUTPUT_ACTIVE);

    //printk("Populating strcut cs_ctrl...\n");
    cs_ctrl.gpio_dev = gpio_dev;
    cs_ctrl.gpio_pin = 18;
    cs_ctrl.delay = 50;
    
    //printk("Populating struct spi_cfg...\n");
    spi_cfg.operation  = SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB;
    spi_cfg.frequency = 20000000;
    spi_cfg.slave = 0;
    spi_cfg.cs = &cs_ctrl;
    
    
    
    //printk("Configuring pins for SPI's D/C (Data Command) and CS (Chip Select)\n");
    gpio_pin_configure(spi_cfg.cs->gpio_dev, spi_cfg.cs->gpio_pin, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure(gpio_dev, 7, GPIO_OUTPUT_ACTIVE);
    //printk("Getting SPI device binding...\n");
    spi_dev = device_get_binding(SPI_DEV);
    
    //printk("SPI Comm Center Initialization Complete\n");
}



// Transmit command & data
void transmit(u8_t cmd, void *tx_data, size_t tx_len) {
    struct spi_buf tx_buf = {.buf = &cmd, .len = 1};
    struct spi_buf_set tx_bufs = {.buffers = &tx_buf, .count = 1};
    gpio_pin_set(gpio_dev, DC_PIN, SEND_COMMAND);
    spi_write(spi_dev, &spi_cfg, &tx_bufs);

    if (tx_data != NULL) {
        tx_buf.buf = tx_data;
        tx_buf.len = tx_len;
        gpio_pin_set(gpio_dev, DC_PIN, SEND_DATA);
        spi_transceive(spi_dev, &spi_cfg, &tx_bufs,NULL);
    }
}

// Transmit Command
void transmitCommand(u8_t cmd) {
    struct spi_buf tx_buf = {.buf = &cmd, .len = 1};
    struct spi_buf_set tx_bufs = {.buffers = &tx_buf, .count = 1};
    gpio_pin_set(gpio_dev, DC_PIN, SEND_COMMAND);
    spi_transceive(spi_dev, &spi_cfg, &tx_bufs,NULL);
}

// Transmit Data
void transmitData(void *tx_data, size_t tx_len) {
    struct spi_buf tx_buf = {.buf = tx_data, .len = tx_len};
    struct spi_buf_set tx_bufs = {.buffers = &tx_buf, .count = 1};
    spi_transceive(spi_dev, &spi_cfg, &tx_bufs,NULL);
}

void sendData16(u16_t data) {
    u8_t bytes[2];
    bytes[0] =  *((u8_t*)&(data)+1); //high byte
    bytes[1] =  *((u8_t*)&(data)+0); //low byte
    transmitData(&bytes, 2);
}

void startDataStream() {
    spi_cfg.operation |= HOLD;
    gpio_pin_set(gpio_dev, DC_PIN, SEND_DATA);
}

void endDataStream() {
    spi_cfg.operation &= ~HOLD;
}
