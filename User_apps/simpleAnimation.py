import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

style.use('fivethirtyeight')

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))
fig.tight_layout()

def animate(i):
    # Read the values from device files for pin 17 and pin 22
    with open('/dev/raspiGpio17', 'rb') as file17, open('/dev/raspiGpio22', 'rb') as file22:
        value17 = int.from_bytes(file17.read(1), byteorder='big')
        value22 = int.from_bytes(file22.read(1), byteorder='big')

    # Append the values to the respective data arrays
    xs17.append(i)
    ys17.append(value17)
    xs22.append(i)
    ys22.append(value22)

    # Limit the data shown in the plots to the last 100 points
    xs17_plot = xs17[-100:]
    ys17_plot = ys17[-100:]
    xs22_plot = xs22[-100:]
    ys22_plot = ys22[-100:]

    # Clear the plots
    ax1.clear()
    ax2.clear()

    # Plot the data for pin 17
    ax1.plot(xs17_plot, ys17_plot)
    ax1.set_title('Pin 17')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Value')
    ax1.set_ylim([-0.1, 1.1])

    # Plot the data for pin 22
    ax2.plot(xs22_plot, ys22_plot)
    ax2.set_title('Pin 22')
    ax2.set_xlabel('Time')
    ax2.set_ylabel('Value')
    ax2.set_ylim([-0.1, 1.1])

# Initialize data arrays
xs17 = []
ys17 = []
xs22 = []
ys22 = []

ani = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()
