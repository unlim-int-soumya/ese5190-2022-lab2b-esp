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
    tx_buff[1] = (uint8_t)(0xFF);
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, tx_buff, 2);

    txbuf[0] = APDS_CONTROL_ONE_REGISTER;
    txbuf[1] = APDS_CONTROL_ONE_AGAIN;
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, tx_buff, 2);

    txbuf[0] = APDS_ENABLE_REGISTER;
    txbuf[1] = APDS_ENABLE_PON | APDS_ENABLE_AEN | APDS_ENABLE_PEN;
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, tx_buff, 2);
}

uint32_t proximity_and_color_read(PIO pio, uint sm){
    uint8_t rx_buff[1] = {0};
    adps_read(pio, sm, STATUS_REGISTER, rx_buff, 1);
    adps_read(pio, sm, ID_REGISTER, rx_buff, 1);

    uint16_t value_of_c = 0;
    uint16_t value_of_r = 0;
    uint16_t value_of_g = 0;
    uint16_t value_of_b =0;

    uint8_t data[8] = {0};
    if ((rx_buff[0] & STATUS_REGISTER_PVALID) == STATUS_REGISTER_PVALID) {
        adps_read(pio, sm, PROXIMITY_DATA_REGISTER, data, 1);
        printf("Our obtained proximity data : %d\n", data[0] - 230);
    } 
    if ((rx_buff[0] & STATUS_REGISTER_AVALID) == STATUS_REGISTER_AVALID) {
        adps_read(pio, sm, RGBC_DATA_REGISTER_CDATAL, data, 8);
        value_of_c = (data[1] << 8 | data[0]); 
        value_of_r = (data[3] << 8 | data[2]); 
        value_of_g = (data[5] << 8 | data[4]); 
        value_of_b = (data[7] << 8 | data[6]); 
        printf("Our obtained color data : (%d, %d, %d, %d)\n", value_of_r, value_of_g, value_of_b, value_of_c);
    }
    uint32_t packet = 0;
    if ((value_of_r > value_of_g) && (value_of_r > value_of_b)) {
        packet = ((uint8_t)((value_of_r*255) / 65536) << 16);
    } else if ((value_of_g > value_of_r) && (value_of_g > value_of_b)) {
        packet = ((uint8_t)((value_of_g*255) / 65536) << 8);
    } else if ((value_of_b > value_of_r) && (value_of_b > value_of_r)) {
        packet = (uint8_t)((value_of_b*255) / 65536);
    }
    return packet;
}

void read_data_adps(PIO pio, uint sm, uint8_t reg_addr, uint8_t *rx_buff, uint num_bytes) {
    pio_i2c_write_blocking(pio, sm, APDS_ADDRESS, &reg_addr, 1);  
    pio_i2c_read_blocking(pio, sm, APDS_ADDRESS, rx_buff, num_bytes);
}

void set_color_for_neopixel(uint32_t color_data){
    uint32_t r_data = color_data >> 16; uint32_t g_data = color_data << 16;
    g_num = g_data >> 24;
    uint32_t b_data = color_data << 24;
    b_data = b_data >> 24;
    uint32_t final_color_data = ((r_data << 8) |
                            (g_data << 16)|
                            (b_data));
    pio_sm_put_blocking(pio0, 0, final_color_data << 8u);
}



int main() {

    const uint POWER_PIN_NAME = PICO_DEFAULT_WS2812_POWER_PIN;
    gpio_init(POWER_PIN_NAME);
    gpio_set_dir(POWER_PIN_NAME, GPIO_OUT);
    gpio_put(POWER_PIN_NAME, 1);
    stdio_init_all();
    
    PIO pio_0 = pio0;
    PIO pio_1 = pio1;
    uint sm_0 = 0;
    uint sm_1 = 1;

    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm_0, offset, WS2812_PIN, 800000, IS_RGBW);

    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm_1, offset, SDA_PIN_NUM, SCL_PIN_NUM);
    
    while(!stdio_usb_connected());

    configuration_of_adps(pio_1, sm_1);

    while(1) {
        color_data = proximity_and_color_read(pio_1, sm_1);
        printf("Obtained color packets: 0x%07x\n", color_data);
        set_color_for_neopixel(color_data);
        sleep_ms(500);
        set_color_for_neopixel(0);
    }
 
    return 0;
}
