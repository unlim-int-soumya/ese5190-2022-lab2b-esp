### TODO:

You can now actually measure the timings you modeled in the last lab. Add APDS9960 support to your sensor, then set your system up to capture the following timing info:
- color packet delivered to PIO module
- bit delivered to WS2812 (24x/packet)
- full packet delivered to WS2812
- brightness changed on APDS

Run this experiment in both dark and light room settings (record initial ambient brightness in each case). The Neopixel should start 'off' and the ADPS9960 should be initialized with your preferred sampling rate (you may want to try a few different rates). Run the experiment for at least 100 samples at brightness settings of 0%, 25%, 50%, 75%, 100% (making sure to give the ADPS reading enough time to 'settle' each time Neopixel is turned off).

Report the observed 'jitter' based on misalignment with the free-running PWM module on the WS2012.



Rather of utilizing the circuitpython code from lab 1, we will use the power of C and PIO in this code to collect sensor measurements from the APDS9960.  Many of our functions from Part 8 stayed the same in this section. for In our last application, we included one additional feature: Neopixel functionality. We displayed the color packet data while transmission when the PIO packet was supplied to the PIO module.

Short explanation of PIO program:

In this case, TX encoding was employed for encoding. The next n+1 words will be processed as instructions if the instruction has a value larger than 0, in which case the FIFO word has no data payload. If not, shift out the 8-data bits first, then the ACK bit.


With the help of this instruction mechanism, state machines may execute stop/start/repstart sequences that had been programmed by the processor at certain locations in the data stream.

The last byte of a transfer needs to have the "final" field set. Thus, a NAK is ignored by the state machine. Any NAK will cause the state machine to halt and interrupt if this field is left empty.

With a threshold of 16, autopull should be enabled. With a threshold of 8, autopush should be enabled.

To ensure that data is immediately available in the OSR, the TX FIFO should be accessed with halfword writes.
