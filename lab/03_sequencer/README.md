# SEQUENCER DEVICE KEYS

The following demonstration video shows recording and playing a sequence of `BOOT` button keypresses using the `neopixel` LED.

<div align='center'>
  <video src='https://user-images.githubusercontent.com/56625259/200099000-16d1b8dd-6086-43b5-8575-84a4f38e1443.mp4'>
</div>
  
- In this program the user is prompted to use `5` keypresses to record a sequence to the RP2040.
- The `QTPY_BOOT_PIN_REG` macro is defined with the register address for `GPIO21` which is `IO_BANK0_BASE + 0x0A8`, since the `BOOT` button is connected to it. The
  `IO_BANK0_BASE` is defined in the SDK to `0x40014000`.
- In the `main` function, `GPIO21` is initialized in ***input*** mode.
- Then, in the `while` loop, the current value stored at the address `QTPY_BOOT_PIN_REG` is read using the `register_read` function.
- Another `while` loop is nested in the `while` loop above which keeps running until `5` keypresses from the user are recorded. The `wait_time` counter is updated
  until a key press is detected (which happens when the value in the `QTPY_BOOT_PIN_REG` goes to `0`). Once a keypress is detected the value in the counter is 
  stored in the `wait_time_arr` array, and the counter is reinitialized to `0`. Also, the `idx` variable is updated, which keeps track of the number of user keypresses.
- The sequence is encoded as the amount of time between two consecutive keypresses. This is stored in an array named `wait_time_arr`.
- Once, the user, has recorded a sequence, it is played using a `for` loop, which iterates over the `wait_time_arr` array, and uses the `sleep_ms` function to wait
  as per the value in the `wait_time_arr` array at that iteration. This helps us replay a sequence.  
- The `re_build.sh` file is a `bash` script that removes the exisiting `build` directory, and creates a new one. After that it navigates into it, and calls `cmake`,
  followed by `make`. This is a convinience script written to automate the build process.
