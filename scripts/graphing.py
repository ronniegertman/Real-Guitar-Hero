# Goertzel algorithm for detecting frequencies in a signal
import numpy as np 
import matplotlib.pyplot as plt

NUM_SAMPLES = 512
def goertzel(samples, target_freq, sample_rate):
    k = int(0.5 + (NUM_SAMPLES * target_freq) / sample_rate)
    omega = (2.0 * np.pi * k) / NUM_SAMPLES
    coeff = 2.0 * np.cos(omega)
    
    s_prev = 0.0
    s_prev2 = 0.0
    
    for sample in samples:
        s_current = sample + coeff * s_prev - s_prev2
        s_prev2 = s_prev
        s_prev = s_current
    
    return (s_prev2**2 + s_prev**2 - coeff * s_prev * s_prev2) / NUM_SAMPLES

sine = np.sin(2 * np.pi * np.arange(NUM_SAMPLES) / NUM_SAMPLES)
def plot_goertzel(target_freq, sample_rate):
    freqs = np.linspace(0, sample_rate / 2, NUM_SAMPLES // 2)
    magnitudes = [goertzel(sine, f, sample_rate) for f in freqs]
    
    plt.figure(figsize=(10, 5))
    plt.plot(freqs, magnitudes)
    plt.title(f'Goertzel Algorithm Frequency Detection\nTarget Frequency: {target_freq} Hz')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude')
    plt.grid()
    plt.xlim(0, sample_rate / 2)
    plt.ylim(0, max(magnitudes) * 1.1)
    plt.show()

    print("frequency detected:", np.argmax(magnitudes) * sample_rate / NUM_SAMPLES)

if __name__ == "__main__":
    target_freq = 50  # Example target frequency
    sample_rate = 1000  # Example sample rate
    plot_goertzel(target_freq, sample_rate)
    print(f"Goertzel algorithm plot saved for target frequency {target_freq} Hz.")
