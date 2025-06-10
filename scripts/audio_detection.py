import numpy as np
import scipy.io.wavfile as wav
from scipy.signal import resample
import matplotlib.pyplot as plt
import serial
import time as pytime

NUM_SAMPLES = 512
SAMPLING_RATE = 10000  # Desired sampling rate in Hz

def audio_sampling():
    # Read WAV file (returns sample_rate, data)
    sample_rate, data = wav.read("../test/8403__speedy__clean_g_str_pluck.wav")

    # Convert to mono if stereo
    if len(data.shape) > 1:
        data = data.mean(axis=1)

    # Resample if needed
    if sample_rate != SAMPLING_RATE:
        num_samples_total = int(len(data) * SAMPLING_RATE / sample_rate)
        data = resample(data, num_samples_total)

    # Take the first NUM_SAMPLES samples
    audio_samples = data[:NUM_SAMPLES]
    time = [i / SAMPLING_RATE for i in range(len(audio_samples))]
    plt.figure(figsize=(10, 4))
    plt.plot(time, audio_samples)
    plt.title("Audio Samples")
    plt.xlabel("time [seconds]")
    plt.show()

    return time, audio_samples

def serial_send(time, audio_sample):
    ser = serial.Serial('COM7', 115200, timeout=1)
    pytime.sleep(2)  # Wait for the serial connection to initialize
    start = pytime.time()
    for t, sample in zip(time, audio_sample):
        # Convert time and sample to bytes
        line = f"{t:.6f},{sample:.6f}\n"
        ser.write(line.encode())
    print("finished sending data")
    ser.write(b"END\n")
    response = ser.readline().decode()
    while response == "":
        response = ser.readline().decode()  
    end = pytime.time()
    print(f"Data sent in {end - start:.2f} seconds")
    print("Response from Arduino:", response)
    ser.close()
    
if __name__ == "__main__":
    time, audio_sample = audio_sampling()
    serial_send(time, audio_sample)

