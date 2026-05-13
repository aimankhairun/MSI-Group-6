import serial
import matplotlib.pyplot as plt
import time

# Connect to Arduino
try:
    ser = serial.Serial('COM4', 9600)
except serial.SerialException:
    print("Error: Could not open COM4. Check that Arduino is connected and the port is correct.")
    exit()

time.sleep(2)  # Wait for Arduino to reset
ser.reset_input_buffer()  # Clear any stale data

plt.ion()  # Interactive mode for real-time plotting
fig, ax = plt.subplots()
x_vals, y_vals = [], []

print("Plotting real-time servo angles. Press CTRL+C in the terminal to stop.")

try:
    while True:
        try:
            raw = ser.readline().decode().strip()
        except UnicodeDecodeError:
            continue  # Skip corrupted data

        # Skip the "STOP" message from Arduino
        if raw == "STOP":
            print("Arduino stopped.")
            break

        # Check if the incoming data is a valid number
        if raw.isdigit():
            angle = int(raw)
            print(f"Servo Angle: {angle}")

            x_vals.append(len(x_vals))
            y_vals.append(angle)

            # Update the plot
            ax.clear()
            ax.set_ylim(0, 180)
            ax.set_title("Real-Time Servo Angle Plot")
            ax.set_xlabel("Samples")
            ax.set_ylabel("Angle")
            ax.plot(x_vals, y_vals, color='blue')
            plt.pause(0.05)

except KeyboardInterrupt:
    print("\nSending stop command to Arduino...")
    ser.write(b'x')
    ser.flush()
    time.sleep(1)

finally:
    ser.close()
    plt.ioff()
    plt.show()  # Display the final static plot
    print("Serial connection closed.")
