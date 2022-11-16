To implement this task, First of all we extracted all the required header files and pins of the QT PY.

After that we defined a method called set_neopixel_color() where by using Bit-banging we defined our desired functionality that we expect from the neopixel LED.

Inside the main code, we declared PICO_DEFAULT_WS2812_POWER_PIN as our POWER_PIN and initialized it followed by setting it as our output pin. We also initialized the gpio pin for boot button.


After that we used PIO pio0 as well as state machined sm0 for our program. 

Next, we used a forever running while loop where we kept all our instrtuctions for recording boot button presses and giving instructions to blink LEDs according to boot button presses.

In this code,we defined the i value as well as NUMBER_KEY_PRESS, If 'NUMBER_KEY_PRESS' becomes less than 'i', then by using the register_read() method we read the input coming to QTPY_BOOT_PIN_REG.
