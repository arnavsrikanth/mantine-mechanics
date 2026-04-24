import pandas as pd

# Load the CSV file
df = pd.read_csv('drone_telemetry_flap0 - Copy.csv')

# Subtract the first timestamp from the entire column to start at 0
df['Timestamp'] = df['Timestamp'] - df['Timestamp'].iloc[0]

# Save the updated data to a new file
df.to_csv('drone_telemetry_relative_time.csv', index=False)

# Display the first few rows to verify the change
print(df.head())