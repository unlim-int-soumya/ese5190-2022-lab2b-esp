### TODO:

Update your sequencer to be able to slow down and speed up recordings/replay. On the input side, the goal is ultimately to be able to handle the full 4 Gbps "firehose" from the PIO logic analyzer example in the SDK manual, which would fill up 256Kb of RAM in only 31 µs at a system clock speed of 125000 MHz if captured 'raw'! On the output side, the goal is to be able to output precisely timed sequences from the PIO at system clock resolution based on a handful of control points.

- update your sequencer to be able to record just the timestamped transitions between input values, and to be able to play data in this format
- give your sequencer the ability to select a range of output rates for both live and recorded input

# Ans: 

Please find the code folder above.

## Explain: 
Updated the sequencer to be able to slow down and speed up recordings/replay. On the input side, the goal is ultimately to be able to handle the full 4 Gbps "firehose" from the PIO logic analyzer example in the SDK manual, which would fill up 256Kb of RAM in only 31 µs at a system clock speed of 125000 MHz if captured 'raw'! On the output side, the goal is to be able to output precisely timed sequences from the PIO at system clock resolution based on a handful of control points.</Br>

We updated your sequencer to be able to record just the timestamped transitions between input values, and to be able to play data in this format and gave our sequencer the ability to select a range of output rates for both live and recorded input</Br>

GIF Normal Motion:</Br>
![alt text](https://github.com/satyajeetburla/ese519-2022-lab2-2B/blob/main/lab/04_slow_motion/lab4_normal_motion%2000_00_00-00_00_30.gif)<br />

GIF Fast Motion:</Br></Br>
![alt text](https://github.com/satyajeetburla/ese519-2022-lab2-2B/blob/main/lab/04_slow_motion/lab4_fast_motion%2000_00_00-00_00_30.gif)<br />

GIF Slow Motion:</Br>
![alt text](https://github.com/satyajeetburla/ese519-2022-lab2-2B/blob/main/lab/04_slow_motion/lab4_slow_motion%2000_00_00-00_00_30.gif)<br />
