import numpy as np

rho = 1000 # Density of water in kg/m^3
l = 0.4 # Length of the wing in meters
w = 0.25 # Width of the wing in meters
omega = np.pi # Angular velocity in rad/s
cd = 1.28 # Drag coefficient for a flat plate

torque = (1/40) * rho * cd * w * omega**2 * l**4

print(torque)