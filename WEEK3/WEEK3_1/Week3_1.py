import serial
import time

ser = serial.Serial('COM4', 9600)
time.sleep(2)
ser.reset_input_buffer()

print("Running... Press CTRL+C to stop.")

try:
    while True:
        line = ser.readline().decode().strip()
        if "/" in line:
            try:
                data = line.split("/")
                voltage = float(data[0])
                adc_value = int(data[1].replace("ADC:", "").strip())
                resistance = float(data[2].replace("R:", "").strip())

                print(f"Voltage: {voltage:.2f} V | ADC: {adc_value} | Resistance: {resistance:.2f} Ω")

                if adc_value > (1023 / 2):
                    ser.write(b"LED ON\n")
                    ser.flush()
                    print("→ LED ON")
                else:
                    ser.write(b"LED OFF\n")
                    ser.flush()
                    print("→ LED OFF")
            except Exception as e:
                print("Parsing Error:", line, "->", e)

except KeyboardInterrupt:
    print("\nStopped by user.")
finally:
    ser.close()
    print("Serial connection closed.")
# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.


def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    print_hi('PyCharm')

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
