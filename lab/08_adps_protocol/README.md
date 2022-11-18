In this case, TX encoding was employed for encoding. The next n+1 words will be processed as instructions if the instruction has a value larger than 0, in which case the FIFO word has no data payload. If not, shift out the 8-data bits first, then the ACK bit.


With the help of this instruction mechanism, state machines may execute stop/start/repstart sequences that had been programmed by the processor at certain locations in the data stream.

The last byte of a transfer needs to have the "final" field set. Thus, a NAK is ignored by the state machine. Any NAK will cause the state machine to halt and interrupt if this field is left empty.

With a threshold of 16, autopull should be enabled. With a threshold of 8, autopush should be enabled.

To ensure that data is immediately available in the OSR, the TX FIFO should be accessed with halfword writes.
