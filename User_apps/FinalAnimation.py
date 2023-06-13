import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

style.use('fivethirtyeight')

def animate(i):
    with open(filename, 'r') as file:
        y_values = file.readline().strip()  # Read the line and remove leading/trailing whitespace
        ys = [float(y) for y in y_values]  # Convert each character to a float
        xs = list(range(1, len(ys) + 1))  # Generate x values
    ax1.clear()
    ax1.plot(xs, ys)
    ax1.set_title(f"raspiGpio: {filename}")

def get_filename():
    gpio_number = input("Enter the GPIO pin number (or 'q' to quit): ")
    if gpio_number == 'q':
        return None
    return f"raspiGpio{gpio_number}"

filename = None

while True:
    if filename is None:
        filename = get_filename()
        if filename is None:
            break
    
    try:
        with open(filename, 'r'):
            fig = plt.figure()
            ax1 = fig.add_subplot(1, 1, 1)
            ani = animation.FuncAnimation(fig, animate, interval=1000)
            plt.show()
            plt.close(fig)
            filename = None
    except FileNotFoundError:
        print(f"File '{filename}' not found. Please make sure the file exists.")
        filename = None
