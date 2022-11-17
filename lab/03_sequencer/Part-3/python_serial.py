
import serial, os

COM_NAME = 'COM3'
BAUDRATE = 115200
TIMEOUT = 5 

wait_time_arr = []
wait_time = ''

ser = serial.Serial(COM_NAME, BAUDRATE, timeout=TIMEOUT)

if os.path.exists("sequencer.txt"):
    os.remove("sequencer.txt")


while True:
    print("Serial Console has started")
    user_inp = input("Please Enter the Macro Values")
    if user_inp == 'r':
        if os.path.exists("sequencer.txt"):
            os.remove("sequencer.txt")
        input_val = bytes(user_inp, 'utf-8')
        ser.write(input_val)
        while (wait_time != b'\r\n'):
            if ser.is_open:
                wait_time = ser.readline()
                wait_time_arr.append(wait_time)
                print(wait_time)
                file_name = open("sequencer.txt", "a");
                final_str = wait_time.decode("utf-8")
                if final_str != "":
                    file_name.write(wait_time.decode("utf-8"))
        file_name.close() 
    if user_inp == 'p':         
        file_name = open("sequencer.txt", "r")
        flag = 0
        j = 0
        file_val_arr = [x for x in file_name]
        input_val = bytes(user_inp, 'utf-8')
        ser.write(input_val)
        wait_time = ser.readline()
        while (wait_time != b'Done\r\n'):
            print(wait_time)
            if (flag == 0 or wait_time == b'Iteration Complete\r\n'):
                filer_val = bytes(file_val_arr[j], 'utf-8')
                print(file_val)
                ser.write(file_val)
                j += 2
                if j > len(file_val_arr):
                    break
                flag = 1
            wait_time = ser.readline()
        file_name.close()

ser.close()

