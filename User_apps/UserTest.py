import matplotlib.pyplot as plt
import numpy as np

from time import sleep
from gpiozero import DigitalInputDevice

NUM_GPIO_PINS = 2
PINS = [17, 22]
DURATION = 10  # Duration in seconds for data collection

# Create DigitalInputDevice objects for each pin
devices = [DigitalInputDevice(pin) for pin in PINS]

# Initialize data arrays
timestamps = [[] for _ in range(NUM_GPIO_PINS)]
values = [[] for _ in range(NUM_GPIO_PINS)]

# Collect data for the specified duration
start_time = time.time()
while time.time() - start_time < DURATION:
    for i, device in enumerate(devices):
        value = device.value
        timestamp = time.time()
        
        # Append the current value and timestamp to the data arrays
        values[i].append(value)
        timestamps[i].append(timestamp)
        
        # Sleep for a short interval to limit the data collection rate
        sleep(0.01)

# Create subplots for each pin
fig, axs = plt.subplots(NUM_GPIO_PINS, 1, sharex=True)

# Plot the data for each pin
for i in range(NUM_GPIO_PINS):
    axs[i].plot(timestamps[i], values[i])
    axs[i].set_ylabel(f"Pin {PINS[i]}")
    axs[i].set_ylim([-0.1, 1.1])

# Set the common x-axis label and show the plot
plt.xlabel("Time")
plt.show()
