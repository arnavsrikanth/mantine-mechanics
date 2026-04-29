import pandas as pd
import matplotlib.pyplot as plt

# Load the processed telemetry files
# Ensure these files are in your working directory
df_flap = pd.read_csv('drone_telemetry_flap0_test.csv')
df_prop = pd.read_csv('drone_telemetry_prop0_final.csv')

# Create the plot
plt.figure(figsize=(10, 6))

# Plot Flap0 data
plt.plot(df_flap['Timestamp'], df_flap['Cumulative_Energy_J'], 
         label='Flapping Configuration', color='#1f77b4', linewidth=2)

# Plot Prop0 data
plt.plot(df_prop['Timestamp'], df_prop['Cumulative_Energy_J'], 
         label='Propeller Configuration', color='#ff7f0e', linewidth=2)

# Add chart metadata
plt.title('Energy Consumption Comparison vs. Time', fontsize=16)
plt.xlabel('Time (seconds)', fontsize=14)
plt.ylabel('Cumulative Energy (J)', fontsize=14)
plt.legend(loc='upper left')
plt.grid(True, linestyle='--', alpha=0.6)

# Optional: Annotate final energy values
final_flap = df_flap['Cumulative_Energy_J'].iloc[-1]
final_prop = df_prop['Cumulative_Energy_J'].iloc[-1]
midpoint_prop = df_prop['Cumulative_Energy_J'].iloc[len(df_prop) // 2]

plt.annotate(f'{final_flap:.1f}J', 
             xy=(df_flap['Timestamp'].iloc[-1], final_flap),
             xytext=(-5, -10), textcoords='offset points',
             ha='right', va='top', fontsize=12)
plt.annotate(f'{final_prop:.1f}J', 
             xy=(df_prop['Timestamp'].iloc[-1], final_prop),
             xytext=(-5, -10), textcoords='offset points',
             ha='right', va='top', fontsize=12)
plt.annotate(f'{midpoint_prop:.1f}J',
             xy=(df_prop['Timestamp'].iloc[len(df_prop) // 2], midpoint_prop),
             xytext=(7, -10), textcoords='offset points',
             ha='left', va='bottom', fontsize=12)

# Add dots at annotated points
plt.plot(df_flap['Timestamp'].iloc[-1], final_flap, 'o', 
         color='#1f77b4', markersize=8)
plt.plot(df_prop['Timestamp'].iloc[-1], final_prop, 'o', 
         color='#ff7f0e', markersize=8)
plt.plot(df_prop['Timestamp'].iloc[len(df_prop) // 2], midpoint_prop, 'o', 
         color='#ff7f0e', markersize=8)

# Display and save the plot
plt.tight_layout()
plt.savefig('energy_plot_test.png')
plt.show()