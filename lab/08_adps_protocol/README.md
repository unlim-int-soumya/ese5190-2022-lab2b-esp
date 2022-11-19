### TODO:

Use the capabilities of your sequencer to implement the ADPS9960 protocol and control the sensor.

# ANS:

Rather of utilizing the circuitpython code from lab 1, we will use the power of C and PIO in this code to collect sensor measurements from the APDS9960. <br />

### Short explanation of Edited PIO program:<br />

In this case, TX encoding was employed for encoding. The next n+1 words will be processed as instructions if the instruction has a value larger than 0, in which case the FIFO word has no data payload. If not, shift out the 8-data bits first, then the ACK bit.<br />
With the help of this instruction mechanism, state machines may execute stop/start/repstart sequences that had been programmed by the processor at certain locations in the data stream.<br />
The last byte of a transfer needs to have the "final" field set. Thus, a NAK is ignored by the state machine. Any NAK will cause the state machine to halt and interrupt if this field is left empty.<br />
With a threshold of 16, autopull should be enabled. With a threshold of 8, autopush should be enabled.<br />
To ensure that data is immediately available in the OSR, the TX FIFO should be accessed with halfword writes.<br />

## GIF of Implementation:<br />
![alt text](https://github.com/satyajeetburla/ese519-2022-lab2-2B/blob/main/lab/08_adps_protocol/part-8_2.gif)<br />

