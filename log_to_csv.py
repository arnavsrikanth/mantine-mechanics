import serial
import csv
import time
from datetime import datetime
from collections import deque
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# --- Configuration ---
port = 'COM7'
baud_rate = 115200
output_file = "imu_data_log.csv"
window_size = 50 

# Data buffers for plotting
roll_data = deque(maxlen=window_size)
pitch_data = deque(maxlen=window_size)
yaw_data = deque(maxlen=window_size)

# Initialize Serial
ser = serial.Serial(port, baud_rate, timeout=0.1)
time.sleep(2)

# Prepare the CSV File
f = open(output_file, "w", newline="")
writer = csv.writer(f)
writer.writerow(["Timestamp", "Roll", "Pitch", "Yaw"])

# Setup Plotting
fig, ax = plt.subplots()
line1, = ax.plot([], [], label='Roll', color='r')
line2, = ax.plot([], [], label='Pitch', color='g')
line3, = ax.plot([], [], label='Yaw', color='b')

def init():
    ax.set_xlim(0, window_size)
    ax.set_ylim(-180, 180)
    ax.legend(loc='upper right')
    ax.set_title("Real-Time IMU Data & Logging")
    return line1, line2, line3

def update(frame):
    line = ser.readline().decode('utf-8').strip()
    
    if line:
        try:
            # Generate Timestamp
            now = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
            
            # Parse Data
            str_values = line.split(",")
            float_values = [float(x) for x in str_values]
            
            if len(float_values) == 3:
                # --- LOGGING ---
                writer.writerow([now] + str_values)
                f.flush() # Forces writing to disk so data isn't lost if script stops
                
                # --- PLOTTING ---
                roll_data.append(float_values[0])
                pitch_data.append(float_values[1])
                yaw_data.append(float_values[2])
                
        except (ValueError, IndexError):
            pass # Skip corrupted serial frames

    # Update line objects
    line1.set_data(range(len(roll_data)), list(roll_data))
    line2.set_data(range(len(pitch_data)), list(pitch_data))
    line3.set_data(range(len(yaw_data)), list(yaw_data))
    
    return line1, line2, line3

# Run the Animation
try:
    # interval=20 means roughly 50 updates per second
    ani = FuncAnimation(fig, update, init_func=init, blit=True, interval=20, cache_frame_data=False)
    plt.show()
finally:
    # Cleanup: This runs when you close the plot window
    print(f"Closing serial and saving {output_file}...")
    ser.close()
    f.close()