import pyaudio
import numpy as np
import matplotlib.pyplot as plt
from typing import Sequence
import time
from dataclasses import dataclass

def goertzel_magnitude(samples: Sequence[float], sample_rate: float, target_freq: float) -> float:
    """Compute the magnitude (intensity) of a target frequency inside a single block using the Goertzel algorithm.

    Parameters
    - samples: sequence or 1D numpy array of audio samples (ints or floats). Length N.
    - sample_rate: sampling rate in Hz.
    - target_freq: frequency in Hz to measure.

    Returns
    - magnitude: float, the magnitude of target_freq in the block (sqrt(power)).

    Notes
    - Uses a real-valued Goertzel implementation producing the magnitude equivalent to the DFT bin energy for the closest bin to target_freq.
    - For short blocks, windowing may reduce spectral leakage; this function does not apply a window by default.
    """
    samples_arr = np.asarray(samples)
    if samples_arr.ndim != 1:
        raise ValueError("samples must be a 1-D sequence")
    N = samples_arr.size
    if N <= 0:
        raise ValueError("samples must contain at least one sample")
    # Normalize samples to float
    x = samples_arr.astype(float)

    # Compute the normalized frequency bin
    k = int(0.5 + (N * target_freq) / sample_rate)
    # Angular frequency for the bin
    omega = (2.0 * np.pi * k) / N
    coeff = 2.0 * np.cos(omega)

    s_prev = 0.0
    s_prev2 = 0.0
    for n in range(N):
        s = x[n] + coeff * s_prev - s_prev2
        s_prev2 = s_prev
        s_prev = s

    magnitude = np.sqrt(s_prev2**2 + s_prev**2 - 2 * np.cos(2*omega) * s_prev * s_prev2)
    return magnitude


# record from microphone and show on graph
CHUNK = 2048
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 5120
MAGNITUDE_THRESHOLD = 3.5e5

@dataclass
class Note:
    freq: float
    name: str

notes = [
    Note(freq=65.41, name="C2"),
    Note(freq=69.30, name="C#2"),
    Note(freq=73.42, name="D2"),
    Note(freq=77.78, name="D#2"),
    Note(freq=82.41, name="E2"),
    Note(freq=87.31, name="F2"),
    Note(freq=92.50, name="F#2"),
    Note(freq=98.00, name="G2"),
    Note(freq=103.83, name="G#2"),
    Note(freq=110.00, name="A2"),
    Note(freq=116.54, name="A#2"),
    Note(freq=123.47, name="B2"),
    Note(freq=130.81, name="C3"),
    Note(freq=138.59, name="C#3"),
    Note(freq=146.83, name="D3"),
    Note(freq=155.56, name="D#3"),
    Note(freq=164.81, name="E3"),
    Note(freq=174.61, name="F3"),
    Note(freq=185.00, name="F#3"),
    Note(freq=196.00, name="G3"),
    Note(freq=207.65, name="G#3"),
    Note(freq=220.00, name="A3"),
    Note(freq=233.08, name="A#3"),
    Note(freq=246.94, name="B3"),
    Note(freq=261.63, name="C4"),
    Note(freq=277.18, name="C#4"),
    Note(freq=293.66, name="D4"),
    Note(freq=311.13, name="D#4"),
    Note(freq=329.63, name="E4"),
    Note(freq=349.23, name="F4"),
    Note(freq=369.99, name="F#4"),
    Note(freq=392.00, name="G4"),
    Note(freq=415.30, name="G#4"),
    Note(freq=440.00, name="A4"),
    Note(freq=466.16, name="A#4"),
    Note(freq=493.88, name="B4"),
    Note(freq=523.25, name="C5"),
    Note(freq=554.37, name="C#5"),
    Note(freq=587.33, name="D5"),
    Note(freq=622.25, name="D#5"),
    Note(freq=659.25, name="E5"),
    Note(freq=698.46, name="F5"),
    Note(freq=739.99, name="F#5"),
    Note(freq=783.99, name="G5"),
    Note(freq=830.61, name="G#5"),
    Note(freq=880.00, name="A5"),
    Note(freq=932.33, name="A#5"),
    Note(freq=987.77, name="B5"),
    Note(freq=1046.50, name="C6")
]

# frequencies = [261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25]  # C4 to C5


def mic_plot_loop():
    p = pyaudio.PyAudio()
    stream = p.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    frames_per_buffer=CHUNK)
    plt.ion()
    fig, ax = plt.subplots()
    x = np.arange(0, 2 * CHUNK, 2)
    line, = ax.plot(x, np.random.rand(CHUNK))
    ax.set_ylim(-32768, 32767)
    ax.set_xlim(0, 2 * CHUNK)
    try:
        while True:
            data = stream.read(CHUNK)
            data_int = np.frombuffer(data, dtype=np.int16)
            line.set_ydata(data_int)
            fig.canvas.draw()
            fig.canvas.flush_events()
    except KeyboardInterrupt:
        print("Stopping microphone plot.")
    finally:
        stream.stop_stream()
        stream.close()
        p.terminate()
        plt.ioff()
        plt.show()


def highest_magnitude_note(data: bytes, notes: Sequence[Note]) -> tuple[Note, float]:
    data_int = np.frombuffer(data, dtype=np.int16)
    max_magnitude = 0.0
    dominant_note = notes[0]
    for note in notes:
        magnitude = goertzel_magnitude(data_int, RATE, note.freq)
        if magnitude > max_magnitude:
            max_magnitude = magnitude
            dominant_note = note
    return dominant_note, max_magnitude

def is_freq_played(data: bytes, freq: float, threshold: float = 100_000.0) -> bool:
    data_int = np.frombuffer(data, dtype=np.int16)
    magnitude = goertzel_magnitude(data_int, RATE, freq)
    return magnitude >= threshold

def process_audio():
    #show on graph the magnitude of target_freq from microphone input
    p = pyaudio.PyAudio()
    stream = p.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    frames_per_buffer=CHUNK)

    print(f"Listening for frequencies")
    try:
        while True:
            data = stream.read(CHUNK)
            start_time = time.time()
            detetcted_note, magnitude = highest_magnitude_note(data, notes)
            stop_time = time.time()
            if magnitude >= MAGNITUDE_THRESHOLD:
                print(f"Detected note: {detetcted_note.name} with magnitude {magnitude} in time {stop_time - start_time:.4f} seconds")
    except KeyboardInterrupt:
        print("Stopping audio processing.")
    finally:
        stream.stop_stream()
        stream.close()
    

if __name__ == '__main__':
    # mic_plot_loop()
    process_audio()
