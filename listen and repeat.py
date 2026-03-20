import csv

import serial
import time

# --- Setup ---
port = 'COM7' 
baud_rate = 115200
output_file = "test.csv"


try:
    # Open the serial port
    ser = serial.Serial(port, baud_rate, timeout=1)
    
    # Small delay to let the ESP32 settle after the serial connection opens
    time.sleep(2)
    print(f"--- Connected to {port} at {baud_rate} baud ---")
    f = open(output_file, "w", newline="")
    writer = csv.writer(f)

    while True:
        # readlines() waits for a '\n' (newline) character
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            writer.writerow([line])  # Write the line to the CSV file
            print(f"ESP32 says: {line}")

except serial.SerialException as e:
    print(f"Error: Could not open serial port {port}. Is it already open in the Arduino IDE?")
except KeyboardInterrupt:
    print("\nStopping script...")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Serial port closed.")