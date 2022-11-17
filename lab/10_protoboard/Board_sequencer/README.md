# SEQUENCER HOST KEYS - MACROS

This demonstration video shows, a sequence of `BOOT` button keypresses on the RP2040, recorded to the host (laptop) on a text file, and played on the `neopixel` LED,
by reading the file stored on the host. This operation is achieved by encoding ***macros*** to do tasks like recording a sequence (to the host) or playing
it (on the RP2040).

https://user-images.githubusercontent.com/56625259/200972717-e51634c8-3d04-41c9-93bd-a67dc2d86f50.mp4


- This program too like the last one ([`sequencer_host_keys`](https://github.com/Ruturajn/Lab2b-esp/tree/main/lab2b_parts/03_sequencer/sequencer_host_keys)) has `2` parts
  to it.
- One of them is the `C` code running on the `RP2040`, and the other is a `python` script named [`rp2040_serial_macros.py`](https://github.com/Ruturajn/Lab2b-esp/blob/main/lab2b_parts/03_sequencer/sequencer_host_keys_macros/rp2040_serial_macros.py)
  running on the host, which interacts with the RP2040 via the serial communication that is established.
- In the `C` code:
  - Sequence of `BOOT` button presses is encoded by keeping a track of `wait_time` or the time between two consecutive key presses. This is done using
    a counter variable.
  - The `QTPY_BOOT_PIN_NUM` and `QTPY_BOOT_PIN_REG` variables are initialized to `21` and `((volatile uint32_t *)(IO_BANK0_BASE + 0x0A8))` respectively
    as per the datasheet. Also, the QtPy's `BOOT` pin is configured to run in `INPUT` mode.
  - In the main `while` loop the code waits for "*user input*" (which is technically done by the python script, since the serial communication is being accessed by the
    python script), once the "*user input*" is received, the code checks if the input was `r`, if so, it starts recording/monitoring `wait_time` until the `BOOT` button
    is pressed `5` times. The status of the `BOOT` button is polled using the `QTPY_BOOT_PIN_REG` register, whose address was defined earlier.
  - Once this is done, the code jumps to the beginning of the `while` loop and waits for the "*user input*" again.
  - Now, if the "*user input*" is `p`, the code takes a different branch, which is responsible for playing the sequence on the `neopixel` LED.
  - Here, a counter named `idx` is maintained to keep track of the number of times the LED needs to blink, that need to be performed (which is `5` in this case). This
    counter is updated (decremented to be precise) every time the LED blinks.
  - A `while` loop is run which keeps on running until `idx > 0`. In the loop, the code waits for the `data` or the `wait time` to be acquired. This is also provided
    by the python script, since it is the one accessing the serial communication. Once the `wait time` is received, the `neopixel` is made to blink, and the RP2040 waits
    until the `wait time` (as the name suggests). This continues until the condition on the `while` loop is not satisfied upon which, the code jumps to the main `while`
    loop and waits for the "*user input*".
- Let's look at the python script:
  - This is where the actual ***user*** comes into the picture, the python script takes in the macro that the user wants to send to the RP2040. The options in this case
    are `r`, and `p`, which stand for `record` and `play` respectively.
  - Here the `pySerial` library is used to access the `COM` port that the RP2040 is connected to.
  - Firstly, an object of the `serial` class is configured with the right `COM` port number and `baudrate`. An indefinite `while` loop is run to get user input
    (which is the macro) and perform actions accordingly.
  - If the user pressed `r`, python starts reading and storing the `wait time` that is being sent to the host (or printed onto the `tty` by RP2040) in a text file until
    a single `\n` is received. This indicates that `5` keypresses are recorded, so the text file is closed.
  - Then the code jumps to the beginning of the `while` loop again and waits for the user input. Now if `p` is pressed, the code opens the text file that was saved
    on to the host and starts sending data (which is `wait time`) to the RP2040, by reading the values in the file. While doing this the code also keeps reading the
    output on the `tty` to look if it ever receives the word `Complete`, which indicates that the play operation is done. Once this is done, the code jumps to the
    beginning again and waits for the user's input.
 - The `re_build.sh` file is a `bash` script that removes the exisiting `build` directory, and creates a new one. After that it navigates into it, and calls `cmake`,
  followed by `make`. This is a convinience script written to automate the build process.
 - Also, following is a screenshot of the recorded sequence that was stored in the text file on the host,

![image](https://user-images.githubusercontent.com/56625259/200970445-5dd0ec6b-acc1-4169-9e85-ad5e29843ae7.png)
