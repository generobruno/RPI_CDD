import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

style.use('fivethirtyeight')

fig = plt.figure()
ax1 = fig.add_subplot(1, 1, 1)

gpio_number = input("Enter the GPIO pin number: ")
filename = f"/dev/raspiGpio{gpio_number}"

def animate(i):
    with open(filename, 'r') as file:
        y_values = file.readline().strip()  # Read the line and remove leading/trailing whitespace
        ys = [float(y) for y in y_values]  # Convert each character to a float
        xs = list(range(1, len(ys) + 1))  # Generate x values
    ax1.clear()
    ax1.plot(xs, ys)

try:
    with open(filename, 'r'):
        pass
except FileNotFoundError:
    print(f"File '{filename}' not found. Please make sure the file exists.")
    raise

ani = animation.FuncAnimation(fig, animate, interval=1000)

try:
    plt.show()
except KeyboardInterrupt:
    pass
