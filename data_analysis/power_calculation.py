import pandas as pd

def process_drone_telemetry_with_relative_time(input_file, output_file):
    # 1. Load the dataset
    df = pd.read_csv(input_file)

    # 2. Rewrite the Timestamp column to start at 0
    # We subtract the first timestamp value from every entry in the column
    df['Timestamp'] = df['Timestamp'] - df['Timestamp'].iloc[0]

    # 3. Calculate Instantaneous Power (P = V * I)
    # Voltage (V) * Current (mA) = Power in milliwatts (mW)
    df['Power_mW'] = df['Voltage'] * df['Current_mA']

    # 4. Calculate the time interval (dt) until the NEXT datapoint
    # .shift(-1) looks at the next row's timestamp
    df['dt_seconds'] = df['Timestamp'].shift(-1) - df['Timestamp']

    # 5. Calculate energy consumed in this interval (milliwatt-seconds / millijoules)
    # Energy = Power * Time
    df['Interval_Energy_mJ'] = df['Power_mW'] * df['dt_seconds']

    # 6. Calculate the running total (Cumulative Energy)
    # Fill the last row's NaN (from the shift) with 0 before summing
    df['Cumulative_Energy_mJ'] = df['Interval_Energy_mJ'].fillna(0).cumsum()

    # 7. Convert Cumulative Energy to Joules for easier reading (1000 mJ = 1 J)
    df['Cumulative_Energy_J'] = df['Cumulative_Energy_mJ'] / 1000

    # Save the modified dataframe to a new CSV
    df.to_csv(output_file, index=False)
    
    print(f"File saved as: {output_file}")
    print(f"Total Duration: {df['Timestamp'].iloc[-1]:.2f} seconds")
    print(f"Total Energy: {df['Cumulative_Energy_J'].iloc[-1]:.2f} Joules")

# Run the script
process_drone_telemetry_with_relative_time('drone_telemetry_prop0.csv', 'drone_telemetry_prop0_final.csv')