
The robust direct memory access unit (DMA) of the RP2040 can transmit data for you in the background. When properly coded, the DMA can do fairly lengthy transfer sequences unattended. To put it technically correct, a PIO state machine can transfer up to one word every system clock, which is an enormous amount of bandwidth. Although the bandwidth is shared by all state machines, one state machine can utilize the entire amount.

In order to record a timestamped trace of an RP2040-ADPS9960 exchange while the BOOT button is pushed, we adapted the logic analyzer example from the datasheet.


Then, using the stemma QT Bus of an ADPS9960 linked to the first QT PY board running the lab 1 Python code, we connected a second QT Py board.



You can see the output's GIF file below.

<br />
# ANS:<br />
Please find the code in the main directory.<br />
Gif:<br />
![alt text](https://github.com/satyajeetburla/ese519-2022-lab2-2B/blob/main/lab/06_pioscope/part-6.gif)<br />





