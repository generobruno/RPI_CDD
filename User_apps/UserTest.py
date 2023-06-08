import matplotlib.pyplot as plt

NUM_GPIO_PINS = 2
PIN_FILES = ["/dev/raspiGpio17", "/dev/raspiGpio22"]
DURATION = 10  # Duration in seconds for data collection

# Open the device files for reading
fd = [open(file, "rb") for file in PIN_FILES]

# Initialize data arrays
timestamps = [[] for _ in range(NUM_GPIO_PINS)]
values = [[] for _ in range(NUM_GPIO_PINS)]

# Collect data for the specified duration
start_time = time.time()
while time.time() - start_time < DURATION:
    for i, file in enumerate(fd):
        # Read the logic level from the device file
        read_buf = file.read(1)
        if len(read_buf) > 0:
            value = int.from_bytes(read_buf, byteorder="big")
            timestamp = time.time()

            # Append the current value and timestamp to the data arrays
            values[i].append(value)
            timestamps[i].append(timestamp)

    # Sleep for a short interval to limit the data collection rate
    sleep(0.01)

# Close the device files
for file in fd:
    file.close()

# Create subplots for each pin
fig, axs = plt.subplots(NUM_GPIO_PINS, 1, sharex=True)

# Plot the data for each pin
for i in range(NUM_GPIO_PINS):
    axs[i].plot(timestamps[i], values[i])
    axs[i].set_ylabel(f"Pin {i + 1}")
    axs[i].set_ylim([-0.1, 1.1])

# Set the common x-axis label and show the plot
plt.xlabel("Time")
plt.show()
