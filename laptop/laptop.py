import serial
import csv
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque

# --- Configuration ---
SERIAL_PORT = 'COM6'  # Change to your port (e.g., '/dev/ttyUSB0' on Linux)
BAUD_RATE = 115200
CSV_FILE = "drone_telemetry_flap2.csv"
MAX_POINTS = 100  # Number of points to show on plot

# Data buffers
times = deque(maxlen=MAX_POINTS)
yaws = deque(maxlen=MAX_POINTS)
pitches = deque(maxlen=MAX_POINTS)
volts = deque(maxlen=MAX_POINTS)

# Initialize Serial
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
except Exception as e:
    print(f"Error opening serial port: {e}")
    exit()

# Setup CSV
with open(CSV_FILE, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["Timestamp", "Yaw", "Pitch", "Roll", "Voltage", "Current_mA"])

# Setup Plot
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
line1, = ax1.plot([], [], label='Yaw', color='r')
line2, = ax1.plot([], [], label='Pitch', color='g')
line3, = ax2.plot([], [], label='Voltage (V)', color='b')

def init():
    ax1.set_ylim(-180, 360)
    ax2.set_ylim(0, 15) # Adjust based on your battery
    ax1.legend()
    ax2.legend()
    return line1, line2, line3

def update(frame):
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        data = line.split(',')
        
        if len(data) == 5:
            try:
                # Parse data
                y, p, r, v, c = map(float, data)
                now = time.strftime("%H:%M:%S")
                timestamp = time.time()

                # Save to CSV
                with open(CSV_FILE, "a", newline="") as f:
                    writer = csv.writer(f)
                    writer.writerow([timestamp, y, p, r, v, c])

                # Update plot buffers
                times.append(time.time())
                yaws.append(y)
                pitches.append(p)
                volts.append(v)

                # Update lines
                line1.set_data(range(len(yaws)), yaws)
                line2.set_data(range(len(pitches)), pitches)
                line3.set_data(range(len(volts)), volts)
                
                ax1.relim()
                ax1.autoscale_view()
                ax2.relim()
                ax2.autoscale_view()

            except ValueError:
                pass 

    return line1, line2, line3

ani = FuncAnimation(fig, update, init_func=init, interval=50, blit=False)
plt.tight_layout()
plt.show()
ser.close()