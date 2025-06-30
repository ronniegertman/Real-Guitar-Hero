import numpy as np
from scipy.io import wavfile

# Parameters
N = 4096  # Frame size

# List of frequencies (copied from your C++ code)
NOTE_FREQS = [
    65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00, 103.83, 110.00, 116.54, 123.47,
    130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
    261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88,
    523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880.00, 932.33, 987.77,
    1046.50, 1108.73, 1174.66, 1244.51, 1318.51
]

def goertzel(input_frame, target_freq, fs):
    if target_freq <= 0 or target_freq > fs / 2:
        return 0.0

    bin_index = int(0.5 + (N * target_freq) / fs)
    omega = 2 * np.pi * bin_index / N
    coeff = 2 * np.cos(omega)
    s_prev = s_prev2 = 0.0

    for x in input_frame:
        s = x + coeff * s_prev - s_prev2
        s_prev2, s_prev = s_prev, s

    power = s_prev2**2 + s_prev**2 - 2 * np.cos(2 * omega) * s_prev * s_prev2
    return power

def detect_note(input_frame, fs):
    max_power = 0.0
    max_freq = 0.0
    for f in NOTE_FREQS:
        power = goertzel(input_frame, f, fs)
        if power > max_power:
            max_power = power
            max_freq = f
    return max_freq

def core_freqs(input_frame, fs):
    results = [{"freq": f, "power": goertzel(input_frame, f, fs)} for f in NOTE_FREQS]
    sorted_results = sorted(results, key=lambda x: x["power"], reverse=True)
    return sorted_results[:6]

# ── Main ────────────────────────────────────────
if __name__ == "__main__":
    fs_wav, data = wavfile.read("test/e_low.wav")
    fs = fs_wav
    data = data.mean(axis=1) if data.ndim > 1 else data
    data = data.astype(float)
    data /= np.max(np.abs(data))

    start = len(data) // 2 - N // 2
    frame = data[start:start + N]

    best_freq = detect_note(frame, fs)
    print(f"Detected frequency: {best_freq:.2f} Hz")
    print(f"Sampling frequency: {fs:.2f} Hz")

    top = core_freqs(frame, fs)
    print("Top candidates:")
    for item in top:
        print(f"  {item['freq']} Hz → {item['power']:.2e}")
