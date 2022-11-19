### TODO:

- modify your sequencer to use the PIO as its primary I/O engine, including the ability to R/W any register 

# ANS:</Br>
Please find the code directory.

The sequencer that we developed in part three has the same code as this one. The boot presses are being recorded here using PIO's state machines, and the sequence is then played back on the neopixel led that is built into the QT PY board.</Br>

We defined our state machines 0 and 1 and used the pio0 instance of our PIO to do this. The boot statu program and ws2812 program PIO programs were added to the main C source file.</Br>

Our PIO was designed to serve as the user's primary search engine and runs at an 8MHz baud rate.</Br>

Implementation GIF:</Br>
![alt text](https://github.com/satyajeetburla/ese519-2022-lab2-2B/blob/main/lab/07_pio_sequencer/lab7_pio_sequencer%2000_00_00-00_00_30.gif)<br />


