### TODO:

- modify the PIO/DMA logic analyzer example to record a timestamped trace of an RP2040-ADPS9960 exchange while the BOOT button is pressed
- with a partner, connect a second Qt Py to the Stemma Qt bus of an ADPS9960 that's attached to a first Qt Py running the Lab 1 Python code
- record a trace of this exchange.<br />
# ANS:<br />
The robust direct memory access unit (DMA) of the RP2040 can transmit data for you in the background. When properly coded, the DMA can do fairly lengthy transfer sequences unattended. To put it technically correct, a PIO state machine can transfer up to one word every system clock, which is an enormous amount of bandwidth. Although the bandwidth is shared by all state machines, one state machine can utilize the entire amount.<br />

In order to record a timestamped trace of an RP2040-ADPS9960 exchange while the BOOT button is pushed, we adapted the logic analyzer example from the datasheet.

Then, using the stemma QT Bus of an ADPS9960 linked to the first QT PY board running the lab 1 Python code, we connected a second QT Py board.

Please find the code in the main directory.<br />
## Implementation Gif:<br />
![alt text](https://github.com/satyajeetburla/ese519-2022-lab2-2B/blob/main/lab/06_pioscope/part-6.gif)<br />





