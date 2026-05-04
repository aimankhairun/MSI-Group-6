import serial
import time
import math
from collections import deque

# -- Replace 'COMx' with your Arduino's serial port (e.g., 'COM3')
SERIAL_PORT = 'COM4' 
BAUD_RATE = 9600

# Buffers to store the last 40 data points (approx. 2 seconds of motion)
history_x = deque(maxlen=40)
history_y = deque(maxlen=40)

def is_circular_motion(x_data, y_data):
    """
    Analyzes an array of X and Y accelerometer data.
    Returns True if the data represents a circular motion, False otherwise.
    """
    if len(x_data) < 10: 
        return False

    n = len(x_data)
    x_list = list(x_data)
    y_list = list(y_data)

    # ===== Step 1: Find the center (mean) of the X and Y movement =====
    # The center of a circle is approximately the average of all the points.
    mean_x = sum(x_list) / n
    mean_y = sum(y_list) / n

    # Center the data (shift so the circle is around the origin)
    centered_x = [x - mean_x for x in x_list]
    centered_y = [y - mean_y for y in y_list]

    # ===== Step 2: Check Amplitude (Variance) =====
    # Calculate variance to make sure there's enough real movement,
    # not just small vibrations/noise sitting on a table.
    variance_x = sum(cx ** 2 for cx in centered_x) / n
    variance_y = sum(cy ** 2 for cy in centered_y) / n

    # Threshold: if variance is too small, it's just noise, not real motion.
    # You may need to tune this value based on your sensor sensitivity.
    MIN_VARIANCE = 500  # Adjust this if needed
    if variance_x < MIN_VARIANCE or variance_y < MIN_VARIANCE:
        return False

    # ===== Step 3: Check Symmetry =====
    # A circle has similar spread in X and Y directions.
    # A straight-line motion would have high spread in one axis but low in the other.
    std_x = math.sqrt(variance_x)
    std_y = math.sqrt(variance_y)

    # Ratio should be close to 1.0 for a circle
    # (e.g., between 0.5 and 2.0 to allow some tolerance for imperfect circles)
    if std_y == 0 or std_x == 0:
        return False

    ratio = std_x / std_y
    if ratio < 0.5 or ratio > 2.0:
        return False  # Too elongated — probably a line, not a circle

    # ===== Step 4: Check Continuous Rotation using 2D Cross Product =====
    # The cross product of consecutive centered vectors tells us the turning direction:
    #   cross = x1 * y2 - y1 * x2
    #   Positive (+) = counter-clockwise turn
    #   Negative (-) = clockwise turn
    cw_count = 0   # clockwise turns
    ccw_count = 0  # counter-clockwise turns

    for i in range(n - 1):
        cross = centered_x[i] * centered_y[i + 1] - centered_y[i] * centered_x[i + 1]

        if cross > 0:
            ccw_count += 1
        elif cross < 0:
            cw_count += 1
        # If cross == 0, the points are collinear (ignore)

    # ===== Step 5: Tally the turns =====
    # If the movement is consistently turning in ONE direction (>70%),
    # it's very likely a circle!
    total_turns = cw_count + ccw_count
    if total_turns == 0:
        return False

    dominant_direction = max(cw_count, ccw_count)
    consistency = dominant_direction / total_turns

    if consistency > 0.70:
        direction = "Clockwise" if cw_count > ccw_count else "Counter-Clockwise"
        print(f"  → Detected {direction} circle (consistency: {consistency:.0%})")
        return True

    return False

def main():
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2) # Wait for Arduino to reset
    print("System ready. Move the MPU6050 in a circle to toggle the LED...")

    # Keep track of the LED state
    led_is_on = False 

    try:
        while True:
            raw = ser.readline().decode(errors='ignore').strip()
            if raw:
                parts = raw.split(',')
                
                if len(parts) >= 3:
                    try:
                        ax = int(parts[0])
                        ay = int(parts[1])
                        
                        # Add new data to our buffers
                        history_x.append(ax)
                        history_y.append(ay)
                        
                        # Only analyze if our buffer is full (40 points)
                        if len(history_x) == 40:
                            if is_circular_motion(history_x, history_y):
                                
                                # Toggle the state
                                led_is_on = not led_is_on 
                                
                                if led_is_on:
                                    print("Circle Detected! Toggling LED: ON")
                                    ser.write(b'O')
                                else:
                                    print("Circle Detected! Toggling LED: OFF")
                                    ser.write(b'F')
                                
                                # DEBOUNCE: Clear the buffers!
                                # Forces the system to wait for 2 full seconds of 
                                # NEW data before it can trigger another toggle.
                                history_x.clear()
                                history_y.clear()
                                
                    except ValueError:
                        pass # Ignore corrupted serial lines
                        
    except KeyboardInterrupt:
        print("\nProgram terminated by user.")
    finally:
        ser.write(b'F') # Ensure LED turns off when script stops
        ser.close()
        print("Serial connection closed.")

if __name__ == "__main__":
    main()
