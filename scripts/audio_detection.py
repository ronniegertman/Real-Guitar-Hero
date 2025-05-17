import pyaudio
import struct

CHUNK = 256
RATE = 8000

p = pyaudio.PyAudio()
stream = p.open(format=pyaudio.paInt16,
                channels=1,
                rate=RATE,
                input=True,
                frames_per_buffer=CHUNK)

print("Listening... (press Ctrl+C to stop)")
try:
    while True:
        data = stream.read(CHUNK, exception_on_overflow=False)
        # Convert bytes to 16-bit integers
        samples = struct.unpack('<' + 'h' * CHUNK, data)

        # Print first 10 samples
        print("Samples:", samples[:10])
except KeyboardInterrupt:
    print("Stopped.")
finally:
    stream.stop_stream()
    stream.close()
    p.terminate()
