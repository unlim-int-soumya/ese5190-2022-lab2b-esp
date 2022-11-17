

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/structs/bus_ctrl.h"
#include "registers.h"

#include "C:\Pico\pico-sdk\src\boards\include\boards\adafruit_qtpy_rp2040.h"

#define BOOT_PIN 21
#define BOOT_PIN_REG ((volatile uint32_t *)(IO_BANK0_BASE + 0x0A8))

#include "hardware/structs/pwm.h"

const uint CAPTURE_PIN_BASE = 23;                 
const uint CAPTURE_PIN_COUNT = 3;       
const uint CAPTURE_N_SAMPLES = 200;     

void logic_analyser_arm(PIO pio, uint sm, uint dma_chan, uint32_t *capture_buf, size_t capture_size_words,
                        uint trigger_pin, bool trigger_level) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_clear_fifos(pio, sm);
    pio_sm_restart(pio, sm);

    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, true);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, false));

    dma_channel_configure(dma_chan, &c,
        capture_buf,        
        &pio->rxf[sm],      
        capture_size_words, 
        true                
    );

    pio_sm_exec(pio, sm, pio_encode_wait_gpio(trigger_level, trigger_pin));
    pio_sm_set_enabled(pio, sm, true);
}


void logic_analyser_init(PIO pio, uint sm, uint pin_base, uint pin_count, float div) {
    uint16_t capture_prog_instr = pio_encode_in(pio_pins, pin_count);
    struct pio_program capture_prog = {
            .instructions = &capture_prog_instr,
            .length = 1,
            .origin = -1
    };
    uint offset = pio_add_program(pio, &capture_prog);
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_in_pins(&c, pin_base);
    sm_config_set_wrap(&c, offset, offset);
    sm_config_set_clkdiv(&c, div);
    sm_config_set_in_shift(&c, true, true, bits_packed_per_word(pin_count));
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);
    pio_sm_init(pio, sm, offset, &c);
}

static inline uint bits_packed_per_word(uint pin_count) {
    const uint SHIFT_REG_WIDTH = 32;
    return SHIFT_REG_WIDTH - (SHIFT_REG_WIDTH % pin_count);
}


void print_capture_buf(const uint32_t *buf, uint pin_base, uint pin_count, uint32_t n_samples) {
    // Display the capture buffer in text form, like this:
    // 00: __--__--__--__--__--__--
    // 01: ____----____----____----
    printf("Capture:\n");
    uint record_size_bits = bits_packed_per_word(pin_count);
    for (int pin = 0; pin < pin_count; ++pin) {
        printf("%02d: ", pin + pin_base);
        for (int sample = 0; sample < n_samples; ++sample) {
            uint bit_index = pin + sample * pin_count;
            uint word_index = bit_index / record_size_bits;
            uint word_mask = 1u << (bit_index % record_size_bits + 32 - record_size_bits);
            printf(buf[word_index] & word_mask ? "-" : "_");
        }
        printf("\n");
    }
}


//Main function
int main() {


    gpio_init(BOOT_PIN);
    gpio_set_dir(BOOT_PIN, GPIO_IN);

    
    stdio_init_all();
    printf("PIO Based Logic Analyzer\n");
    

    bus_ctrl_hw->priority = BUSCTRL_BUS_PRIORITY_DMA_W_BITS | BUSCTRL_BUS_PRIORITY_DMA_R_BITS;

    PIO pio0 = pio0;     //We used pio0 as our PIO 
    uint sm0 = 0;        //state machine 0 
    uint dma_chan0 = 0;  //dma channel 0

    
    logic_analyser_init(pio0, sm0, CAPTURE_PIN_BASE, CAPTURE_PIN_COUNT, 56.f);
    
    while(true) {
        if (register_read(BOOT_PIN_REG) == 0) {

            
            uint total_sample_bits = CAPTURE_N_SAMPLES * CAPTURE_PIN_COUNT;
            total_sample_bits += bits_packed_per_word(CAPTURE_PIN_COUNT) - 1;
            uint buf_size_words = total_sample_bits / bits_packed_per_word(CAPTURE_PIN_COUNT);
            uint32_t *capture_buf = malloc(buf_size_words * sizeof(uint32_t));
            hard_assert(capture_buf);
            

            
            printf("Arming trigger\n");
            logic_analyser_arm(pio0, sm0, dma_chan0, capture_buf, buf_size_words, CAPTURE_PIN_BASE, true);
            

            
            dma_channel_wait_for_finish_blocking(dma_chan0);

            print_capture_buf(capture_buf, CAPTURE_PIN_BASE, CAPTURE_PIN_COUNT, CAPTURE_N_SAMPLES);
                       
        }
        sleep_ms(1000);
    }
}