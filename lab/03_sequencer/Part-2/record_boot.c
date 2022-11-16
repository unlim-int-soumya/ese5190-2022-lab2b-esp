#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "registers.h"
#include "C:\Users\satya\Desktop\EMSW\pico-sdk\src\boards\include\boards\adafruit_qtpy_rp2040.h"

#define IS_RGBW true
#define NUM_PIXELS 150
#define PICO_DEFAULT_WS2812_POWER_PIN 11
#define NUMBER_KEY_PRESS 8
#define QTPY_BOOT_PIN_NUM 21
#define QTPY_BOOT_PIN_REG ((volatile uint32_t *)(IO_BANK0_BASE + 0x0A8))
#define CONST_TIME_SCALING_FACTOR 50

#define NUMBER_KEY_PRESS 8

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
#define WS2812_PIN 2
#endif


void set_neopixel_color(uint32_t color_num){
    uint32_t r_num = color_num >> 16;
    uint32_t g_num = color_num << 16;
    g_num = g_num >> 24;
    uint32_t b_num = color_num << 24;
    b_num = b_num >> 24;
    uint32_t color_final = ((r_num << 8) |
                            (g_num << 16)|
                            (b_num));
    pio_sm_put_blocking(pio0, 0, color_final << 8u);
}


int main() {

    const uint POWER_PIN = PICO_DEFAULT_WS2812_POWER_PIN;
    gpio_init(POWER_PIN);
    gpio_init(QTPY_BOOT_PIN_NUM);
    gpio_set_dir(POWER_PIN, GPIO_OUT);
    gpio_set_dir(QTPY_BOOT_PIN_NUM, GPIO_IN);
    gpio_put(POWER_PIN, 1);
    
    stdio_init_all();

    // todo get free sm
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    while(!stdio_usb_connected());

    while(1){
        int wait_time = 0;
        int i = 0;
        int wait_time_arr[NUMBER_KEY_PRESS] = {0};
        

        
        while (i < NUMBER_KEY_PRESS){
            uint32_t button_status = register_read(QTPY_BOOT_PIN_REG);
            if(button_status != 0){
                wait_time+=1;
            }else{
                if (wait_time != 0){
                    printf("%d\n", wait_time);
                    wait_time_arr[i] = wait_time;
                    i += 1;
                }
                wait_time = 0;
            }
            sleep_ms(100);
        }
        printf("\n");

        uint32_t wait_time_input;
        
        while (i > 0){
            scanf("%d", &wait_time_input);
            set_neopixel_color(0x6433ff);
            sleep_ms(500);
            set_neopixel_color(0);
            sleep_ms(wait_time_input*CONST_TIME_SCALING_FACTOR);
            i -= 1;
            printf("Iteration Complete\n");
        }
        printf("Done\n");
        sleep_ms(100);
    }
}
