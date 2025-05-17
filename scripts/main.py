import pyaudio
import socket
import struct
import matplotlib.pyplot as plt


# ESP32 IP & Port
ESP32_IP = '192.168.1.122'  # Change to your ESP32 IP
ESP32_IP = '192.168.237.187'
ESP32_PORT = 1234
MAX_LEN = 200

# Audio settings
CHUNK = 256
RATE = 16000

p = pyaudio.PyAudio()
stream = p.open(format=pyaudio.paInt16,
                channels=1,
                rate=RATE,
                input=True,
                frames_per_buffer=CHUNK)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print("Streaming mic audio to ESP32...")
plt.ion()
fig, ax = plt.subplots()
ydata = []
xdata = []
line, = ax.plot(xdata, ydata)
i = 0

try:
    while True:
        data = stream.read(CHUNK, exception_on_overflow=False)
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
        samples = struct.unpack('<' + 'h' * CHUNK, data)
        data = struct.pack('>' + 'h' * CHUNK, *samples)
        sock.sendto(data, (ESP32_IP, ESP32_PORT))
except KeyboardInterrupt:
    pass
finally:
    stream.stop_stream()
    stream.close()
    p.terminate()
