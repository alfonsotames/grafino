
/** SPI Communication Center **/

#include <zephyr.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>


#ifndef SPICOMMS_H
#define SPICOMMS_H

#ifdef __cplusplus
extern "C" {
#endif



#define SEND_COMMAND 0
#define SEND_DATA 1

#define HIGH 1
#define LOW 1

#define HOLD 0x2000
// Pin for Data/Command (Only using PORT A)
#define DC_PIN 7
#define SPI_DEV DT_LABEL(DT_ALIAS(spi_0))





void startSPICommCenter();
void transmit(u8_t cmd, void *tx_data, size_t tx_len);
void transmitData(void *tx_data, size_t tx_len);
void transmitCommand(u8_t cmd);
void startDataStream();
void endDataStream();
void sendData16(u16_t data);


#ifdef __cplusplus
}
#endif

#endif /* SPICOMMS_H */