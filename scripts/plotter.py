import serial
import matplotlib.pyplot as plt

MAX_LEN = 200

ser = serial.Serial('COM5', 9600)
plt.ion()
fig, ax = plt.subplots()
ydata = []
xdata = []
line, = ax.plot(xdata, ydata)
i = 0
while True:
    line_data = ser.readline().decode().strip()
    try:
        value = float(line_data)
        ydata.append(value)
        xdata.append(i)
        i += 1
        if i >= MAX_LEN:
            xdata.pop(0)
            ydata.pop(0)

        line.set_xdata(xdata)
        line.set_ydata(ydata)
        ax.relim()
        ax.autoscale_view()
        plt.draw()
        plt.pause(0.01)
    except:
        continue