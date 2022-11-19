### TODO:

Create a 'sequencer' that allows you to record BOOT button presses and play them on the Neopixel, and also play a sequence of read/write commands. You should be able to:
- record at a least a few seconds of button input to your RP2040 (in RAM)
- replay a recorded sequence on your NeoPixel
- loop a recording
- save a recording to your laptop (the Python Serial library is one way to do this)
- play a recording from your laptop
- record 'macros' (a sequence of console commands) based on keystrokes in your serial console
- hand-edit a list of register read/write commands on your laptop, and play them on the RP2040
- include multiple I/O sources in a recording, and remap among the following:
    - inputs: BOOT button, console commands, register read/write commands
    - outputs: neopixel color, neopixel brightness, data over serial, register read/write commands

# Ans:
Please find the code folder in the directory.
## Brief Explaination:
Part-1

To implement this task, First of all we extracted all the required header files and pins of the QT PY.

After that we defined a method called set_neopixel_color() where by using Bit-banging we defined our desired functionality that we expect from the neopixel LED.

Inside the main code, we declared PICO_DEFAULT_WS2812_POWER_PIN as our POWER_PIN and initialized it followed by setting it as our output pin. We also initialized the gpio pin for boot button.

After that we used PIO pio0 as well as state machined sm0 for our program.

Next, we used a forever running while loop where we kept all our instrtuctions for recording boot button presses and giving instructions to blink LEDs according to boot button presses.

In this code,we defined the i value as well as NUMBER_KEY_PRESS, If 'NUMBER_KEY_PRESS' becomes less than 'i', then by using the register_read() method we read the input coming to QTPY_BOOT_PIN_REG.

GIF:
![alt text](https://github.com/satyajeetburla/ese519-2022-lab2-2B/blob/main/lab/03_sequencer/Part-1/sequencer1%2000_00_00-00_00_30.gif)<br />


Part-2
We repeated our first section's actions in our second part. However, we are documenting the boot presses on a specific occasion file in the laptop is text. The recorded sequence was then played out in LED using the values recorded in the text file.

For this, the Python Serial Library was utilized. Here, we establish the baud rate, timeout value, and the name of our Command port. After initializing the serial library, we established an array to record our wait times and a variable to read from the COM Port. To keep our values in a text file, we made a text file named sequencer.txt.

To read till we reached the end of the line, we employed a while loop. The data is continuously read from the port, stored in the wait time, and appended to the "wait time arr" field. The data is finally printed to the terminal.

We then opened the file again to read its contents, put the contents in an array, and repeated this process until the entire message was read from the terminal.

Check to see if this is the initial writing or if the first iteration's blinking is complete. The output from the console is then read.

GIF:
![alt text](https://github.com/satyajeetburla/ese519-2022-lab2-2B/blob/main/lab/03_sequencer/Part-2/sequencer2%2000_00_00-00_00_30.gif)<br />

Part-3
Part 3 is identical to Part 2, except in this case we are reading or playing the recorded sequences in the neopixel dependent on the key strokes on the console.
Here, pressing "r" initiates recording, and pressing "p" initiates playback of the recorded sequence. It is implemented using an if else statement inside of a while loop that never ends and regulates the Record/Play sequence operations in accordance with the input received.

GIF:
![alt text](https://github.com/satyajeetburla/ese519-2022-lab2-2B/blob/main/lab/03_sequencer/Part-3/sequencer3%2000_00_00-00_00_30.gif)<br />
