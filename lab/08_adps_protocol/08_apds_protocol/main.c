#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pio_i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "registers.h"
#include "apds_registers.h"
#include "C:\Users\satya\Desktop\EMSW\pico-sdk\src\boards\include\boards\adafruit_qtpy_rp2040.h"

#define SDA_PIN_NUM 22
#define SCL_PIN_NUM 23

void configuration_of_adps(PIO pio, uint sm){
    uint8_t tx_buff[2] = {0};

    tx_buff[0] = ATIME_REGISTER;
    tx_buff[1] = (uint8_t)(0x81);
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, tx_buff, 2);

    txbuf[0] = APDS_CONTROL_ONE_REGISTER;
    txbuf[1] = APDS_CONTROL_ONE_AGAIN;
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, tx_buff, 2);

    txbuf[0] = APDS_ENABLE_REGISTER;
    txbuf[1] = APDS_ENABLE_PON | APDS_ENABLE_AEN | APDS_ENABLE_PEN;
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, tx_buff, 2);
}

void read_data_adps(PIO pio, uint sm, uint8_t reg_addr, uint8_t *rxbuf, uint num_bytes) {
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, &reg_addr, 1);  
    pio_i2c_read_blocking(pio, sm, APDS_ADDRESS, rx_buff, num_bytes);
}

int main() {
    stdio_init_all();

    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm, offset, SDA_PIN_NUM, SCL_PIN_NUM);
    
    while(!stdio_usb_connected());

    configuration_of_adps(pio, sm);

    while(1) {
        uint8_t rx_buff[1] = {0};
        read_data_adps(pio, sm, STATUS_REGISTER, rx_buff, 1);
        read_data_adps(pio, sm, ID_REGISTER, rx_buff, 1);

        uint8_t data_arr[8] = {0};
        if ((rxbuf[0] & STATUS_REGISTER_PVALID) == STATUS_REGISTER_PVALID) {
            read_data_adps(pio, sm, PROXIMITY_DATA_REGISTER, data_arr, 1);
            printf("Displaying the Proximity Data : %d\n", data_arr[0] - 230);
        } 
        if ((rxbuf[0] & STATUS_REGISTER_AVALID) == STATUS_REGISTER_AVALID) {
            read_data_adps(pio, sm, RGBC_DATA_REGISTER_CDATAL, data_arr, 8);
            uint16_t val_of_c = (data_arr[1] << 8 | data_arr[0]); 
            uint16_t val_of_r = (data_arr[3] << 8 | data_arr[2]); 
            uint16_t val_of_g = (data_arr[5] << 8 | data_arr[4]); 
            uint16_t val_of_b = (data_arr[7] << 8 | data_arr[6]); 
            printf("Obtained Color data : (%d, %d, %d, %d)\n", val_of_r, val_of_g, val_of_b, val_of_c);
        }

        sleep_ms(500); 
    }
 
    return 0;
}
