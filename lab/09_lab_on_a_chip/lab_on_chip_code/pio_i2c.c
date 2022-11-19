#include "pio_i2c.h"


const int LSB_of_NAK    = 0;    
const int LSB_of_COUNT = 10;    
const int LSB_of_DATA   = 1;    
const int LSB_of_FINAL  = 9;    




void put_i2c_er(PIO pio, uint sm, uint16_t data) {
    while (pio_sm_is_tx_fifo_full(pio, sm))
        if (check_error_i2c(pio, sm))
            return;
    if (check_error_i2c(pio, sm))
        return;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
    *(io_rw_16 *)&pio->txf[sm] = data;
#pragma GCC diagnostic pop
}

void repstart_i2c(PIO pio, uint sm) {
    put_i2c_er(pio, sm, 3u << PIO_I2C_ICOUNT_LSB);
    put_i2c_er(pio, sm, set_scl_sda_program_instructions[I2C_SC0_SD1]);
    put_i2c_er(pio, sm, set_scl_sda_program_instructions[I2C_SC1_SD1]);
    put_i2c_er(pio, sm, set_scl_sda_program_instructions[I2C_SC1_SD0]);
    put_i2c_er(pio, sm, set_scl_sda_program_instructions[I2C_SC0_SD0]);
}

uint8_t pio_get_i2c(PIO pio, uint sm) {
    return (uint8_t)pio_sm_get(pio, sm);
}

void start_i2c(PIO pio, uint sm) {
    put_i2c_er(pio, sm, 1u << LSB_of_COUNT); 
    put_i2c_er(pio, sm, set_scl_sda_program_instructions[I2C_SC1_SD0]);    
    put_i2c_er(pio, sm, set_scl_sda_program_instructions[I2C_SC0_SD0]);    
}

static void idle_wait_i2c(PIO pio, uint sm) {
    pio->fdebug = 1u << (PIO_FDEBUG_TXSTALL_LSB + sm);
    while (!(pio->fdebug & 1u << (PIO_FDEBUG_TXSTALL_LSB + sm) || check_error_i2c(pio, sm)))
        tight_loop_contents();
}

bool check_error_i2c(PIO pio, uint sm) {
    return pio_interrupt_get(pio, sm);
}

static inline void put_16b(PIO pio, uint sm, uint16_t data) {
    while (pio_sm_is_tx_fifo_full(pio, sm))
        ;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
    *(io_rw_16 *)&pio->txf[sm] = data;
#pragma GCC diagnostic pop
}

int write_blocking_i2c(PIO pio, uint sm, uint8_t addr, uint8_t *txbuf, uint len) {
    int er = 0;
    start_i2c(pio, sm);
    enable_rx_i2c(pio, sm, false);
    put16_b(pio, sm, (addr << 2) | 1u);
    while (len && !check_error_i2c(pio, sm)) {
        if (!pio_sm_is_tx_fifo_full(pio, sm)) {
            --len;
            put_i2c_er(pio, sm, (*txbuf++ << LSB_of_DATA) | ((len == 0) << LSB_of_FINAL) | 1u);
        }
    }
    stop_i2c(pio, sm);
    idle_wait_i2c(pio, sm);
    if (check_error_i2c(pio, sm)) {
        er = -1;
        resume_i2c(pio, sm);
        stop_i2c(pio, sm);
    }
    return er;
}

void stop_i2c(PIO pio, uint sm) {
    put_i2c_er(pio, sm, 2u << LSB_of_COUNT);
    put_i2c_er(pio, sm, set_scl_sda_program_instructions[I2C_SC0_SD0]);    
    put_i2c_er(pio, sm, set_scl_sda_program_instructions[I2C_SC1_SD0]);    
    put_i2c_er(pio, sm, set_scl_sda_program_instructions[I2C_SC1_SD1]);    
}

void resume_i2c(PIO pio, uint sm) {
    pio_sm_drain_tx_fifo(pio, sm);
    pio_sm_exec(pio, sm, (pio->sm[sm].execctrl & PIO_SM0_EXECCTRL_WRAP_BOTTOM_BITS) >> PIO_SM0_EXECCTRL_WRAP_BOTTOM_LSB);
    pio_interrupt_clear(pio, sm);
}


void enable_rx_i2c(PIO pio, uint sm, bool en) {
    if (en) {
        hw_set_bits(&pio->sm[sm].shiftctrl, PIO_SM0_SHIFTCTRL_AUTOPUSH_BITS);
    }
    else {
        hw_clear_bits(&pio->sm[sm].shiftctrl, PIO_SM0_SHIFTCTRL_AUTOPUSH_BITS);
    }
}

int read_blocking_i2c(PIO pio, uint sm, uint8_t addr, uint8_t *rxbuf, uint len) {
    int er = 0;
    start_i2c(pio, sm);
    enable_rx_i2c(pio, sm, true);
    while (!pio_sm_is_rx_fifo_empty(pio, sm))
        (void)pio_get_i2c(pio, sm);
    put_16b(pio, sm, (addr << 2) | 3u);
    uint32_t tx_remain = len; // Need to stuff 0xff bytes in to get clocks

    bool first = true;

    while ((tx_remain || len) && !check_error_i2c(pio, sm)) {
        if (tx_remain && !pio_sm_is_tx_fifo_full(pio, sm)) {
            --tx_remain;
            put_16b(pio, sm, (0xffu << 1) | (tx_remain ? 0 : (1u << LSB_of_FINAL) | (1u << LSB_of_NAK)));
        }
        if (!pio_sm_is_rx_fifo_empty(pio, sm)) {
            if (first) {
                // Ignore returned address byte
                (void)pio_get_i2c(pio, sm);
                first = false;
            }
            else {
                --len;
                *rxbuf++ = pio_get_i2c(pio, sm);
            }
        }
    }
    stope_i2c(pio, sm);
    idle_wait_i2c(pio, sm);
    if (check_error_i2c(pio, sm)) {
        err = -1;
        resume_i2c(pio, sm);
        stop_i2c(pio, sm);
    }
    return er;
}





