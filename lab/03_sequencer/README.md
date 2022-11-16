Part-1
To implement this task, First of all we extracted all the required header files and pins of the QT PY.

After that we defined a method called set_neopixel_color() where by using Bit-banging we defined our desired functionality that we expect from the neopixel LED.

Inside the main code, we declared PICO_DEFAULT_WS2812_POWER_PIN as our POWER_PIN and initialized it followed by setting it as our output pin. We also initialized the gpio pin for boot button.


After that we used PIO pio0 as well as state machined sm0 for our program. 

Next, we used a forever running while loop where we kept all our instrtuctions for recording boot button presses and giving instructions to blink LEDs according to boot button presses.

In this code,we defined the i value as well as NUMBER_KEY_PRESS, If 'NUMBER_KEY_PRESS' becomes less than 'i', then by using the register_read() method we read the input coming to QTPY_BOOT_PIN_REG.


Part-2
We repeated our first section's actions in our second part. However, we are documenting the boot presses on a specific occasion file in the laptop is text. The recorded sequence was then played out in LED using the values recorded in the text file.

For this, the Python Serial Library was utilized. Here, we establish the baud rate, timeout value, and the name of our Command port. After initializing the serial library, we established an array to record our wait times and a variable to read from the COM Port. To keep our values in a text file, we made a text file named sequencer.txt.

To read till we reached the end of the line, we employed a while loop. The data is continuously read from the port, stored in the wait time, and appended to the "wait time arr" field. The data is finally printed to the terminal.

We then opened the file again to read its contents, put the contents in an array, and repeated this process until the entire message was read from the terminal.

Check to see if this is the initial writing or if the first iteration's blinking is complete. The output from the console is then read.
