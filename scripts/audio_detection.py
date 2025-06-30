import numpy as np
import scipy.io.wavfile as wav
from scipy.signal import resample
import matplotlib.pyplot as plt
import serial
import time as pytime

NUM_SAMPLES = 2048
SAMPLING_RATE = 5120  # Desired sampling rate in Hz

def audio_sampling():
    # Read WAV file (returns sample_rate, data)
    sample_rate, data = wav.read("../test/g.wav")

    # Convert to mono if stereo
    if len(data.shape) > 1:
        data = data.mean(axis=1)

    # Resample if needed
    if sample_rate != SAMPLING_RATE:
        num_samples_total = int(len(data) * SAMPLING_RATE / sample_rate)
        data = resample(data, num_samples_total)

    # Take the last NUM_SAMPLES samples
    audio_samples = data[-NUM_SAMPLES:]
    # audio_samples = data[NUM_SAMPLES*4:NUM_SAMPLES*5]
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
    
def plot_fft(audio_sample):
    # Perform FFT
    fft_result = np.fft.fft(audio_sample)
    freqs = np.fft.fftfreq(len(fft_result), 1/SAMPLING_RATE)

    # Plot the FFT result
    plt.figure(figsize=(10, 4))
    plt.plot(freqs[:len(freqs)//2], np.abs(fft_result)[:len(fft_result)//2])
    plt.title("FFT of Audio Samples")
    plt.xlabel("Frequency [Hz]")
    plt.ylabel("Magnitude")
    #mark the maximum frequency
    plt.legend()
    plt.xlim(0, SAMPLING_RATE / 2)
    plt.grid()
    plt.show()

    #find core frequency real value signal
    core_freq = freqs[np.argmax(np.abs(fft_result)[:len(fft_result)//2])]
    print(f"Core frequency: {core_freq:.2f} Hz")

if __name__ == "__main__":
    time, audio_sample = audio_sampling()
    serial_send(time, audio_sample)
    # plot_fft(audio_sample)

