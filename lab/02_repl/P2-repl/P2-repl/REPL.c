#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "neopixel.h"
#include "hardware/gpio.h"
#include "registers.h"
#include <strings.h>
#include <string.h>

#define QTPY_BOOT_PIN 21
#define SIO
#ifndef PICO_DEFAULT_WS2812_PIN
#define PICO_DEFAULT_WS2812_PIN 12
#endif

#ifndef PICO_DEFAULT_WS2812_POWER_PIN
#define PICO_DEFAULT_WS2812_POWER_PIN 11
#endif

volatile uint32_t * address;
char inputs[9];   //input is an char array, size 9
char input_value;
uint32_t address_hex;
int conversion_valid;
int32_t int_register_value;
int length_of_inputs;
int  count = 0;
int new_value;
uint32_t value_hex;

typedef struct {
    uint32_t reg_address;
    uint32_t reg_value;
} Register_status; 

void render_to_console(Register_status status) {
    // adjust console window height to match 'frame size'

    printf("reg_address:  0x%08x\n",   status.reg_address);
    printf("reg_value:    0x%08x\n",   status.reg_value);
    for (uint8_t i=0; i<6; i++) { 
        printf("\n");
    }
}  

int input_function_serial(){
        sleep_ms(5000);//delay for 5s
        //loop for 8 times
        for (int i= 0; i<8; i++ ) {
            input_value = getchar_timeout_us(0);//timeout for 1us
            inputs[i] = input_value;
        }
        inputs[8] = '\0';
}

//test whether the input register address is valid or not
int valid_register(uint32_t convert_inputs){
    int quotient = convert_inputs % 4;//a valid address should have 8 digits
    if (quotient== 0){
        return 0;
    }
    else {
        printf("Not a valid address\n");
        sleep_ms(2000);
        return 1;
    }
}



int main() {
//initialization
    stdio_init_all();
    neopixel_init();
    stdio_usb_init();

    Register_status status;

    while (!stdio_usb_connected()){};//the program begin to work after detecting that usb is connected
    while (true) {

        sleep_ms(100);
        printf("Enter the register you want to access:\n");
        input_function_serial();
        sleep_ms(100);
        int_register_value = atoi(inputs);//change string to integer value
        conversion_valid = valid_register(int_register_value);

        // if its not a valid address
        if (conversion_valid == 1){
            address_hex = 0;
            status.reg_address = 0;
            status.reg_value = 0;
            continue;
        }
        sscanf(inputs,"%x",&address_hex);
        printf("The hex address is: %x\n", address_hex);

        address = (volatile uint32_t *) address_hex;
        status.reg_address = address_hex;
        status.reg_value = (uint32_t) *address;
        render_to_console(status);
        sleep_ms(10);
        
      
        printf("Want to write a new value to register? (Yes: 1, No: 0)\n");        
        sleep_ms(5000);
        new_value = getchar_timeout_us(0);
        if(new_value == '1'){
            printf("Enter the new 8 bit hex value you want to write in this register:\n");
            input_function_serial();
            sleep_ms(100);
            int sscanf_status = sscanf(inputs,"%x",&value_hex);
            *address = value_hex;
            status.reg_value = (uint32_t) *address;
            printf("Success");
            render_to_console(status); 

        }
        
        else {
            printf("Did not recieve the value or you entered 0\n");
            continue;
        }
    
    }
    return 0;
}  