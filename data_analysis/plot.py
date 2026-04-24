import pandas as pd
import matplotlib.pyplot as plt

# Load the processed telemetry files
# Ensure these files are in your working directory
df_flap = pd.read_csv('drone_telemetry_flap0_final.csv')
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
plt.title('Comparison of Total Energy Consumed (Joules)', fontsize=14)
plt.xlabel('Time (seconds)', fontsize=12)
plt.ylabel('Cumulative Energy (J)', fontsize=12)
plt.legend(loc='upper left')
plt.grid(True, linestyle='--', alpha=0.6)

# Optional: Annotate final energy values
final_flap = df_flap['Cumulative_Energy_J'].iloc[-1]
final_prop = df_prop['Cumulative_Energy_J'].iloc[-1]

plt.text(df_flap['Timestamp'].iloc[-1], final_flap, f' {final_flap:.1f}J', va='center')
plt.text(df_prop['Timestamp'].iloc[-1], final_prop, f' {final_prop:.1f}J', va='center')

# Display and save the plot
plt.tight_layout()
plt.savefig('energy_plot.png')
plt.show()