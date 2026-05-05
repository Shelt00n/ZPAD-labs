"""
Laboratory work #4: Data visualization 2
Interactive harmonic signal with noise and filtering.

Run:
    python main.py
"""

from __future__ import annotations

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, CheckButtons
from scipy.signal import butter, filtfilt


# Initial parameters
INITIAL = {
    "amplitude": 1.0,
    "frequency": 0.25,
    "phase": 0.0,
    "noise_mean": 0.0,
    "noise_covariance": 0.10,
    "cutoff_frequency": 2.5,
}

# Signal settings
T_START = 0.0
T_END = 10.0
POINTS = 1000
SAMPLE_RATE = POINTS / (T_END - T_START)
FILTER_ORDER = 4


def harmonic_with_noise(
    amplitude: float,
    frequency: float,
    phase: float,
    noise_mean: float,
    noise_covariance: float,
    show_noise: bool = True,
    x: np.ndarray | None = None,
    noise: np.ndarray | None = None,
) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    """Return harmonic, noisy harmonic and noise arrays.

    y(t) = A * sin(omega * t + phi), where omega = 2*pi*frequency.
    If show_noise is False, the noisy signal is equal to the clean harmonic.
    """
    if x is None:
        x = np.linspace(T_START, T_END, POINTS)

    harmonic = amplitude * np.sin(2 * np.pi * frequency * x + phase)

    if noise is None:
        std = np.sqrt(max(noise_covariance, 0.0))
        noise = np.random.normal(noise_mean, std, size=x.shape)

    noisy_signal = harmonic + noise if show_noise else harmonic.copy()
    return harmonic, noisy_signal, noise


def lowpass_filter(signal: np.ndarray, cutoff_frequency: float) -> np.ndarray:
    """Filter signal using a Butterworth low-pass filter."""
    nyquist = 0.5 * SAMPLE_RATE
    normalized_cutoff = cutoff_frequency / nyquist
    normalized_cutoff = float(np.clip(normalized_cutoff, 0.001, 0.999))
    b, a = butter(FILTER_ORDER, normalized_cutoff, btype="low", analog=False)
    return filtfilt(b, a, signal)


class HarmonicApp:
    """Matplotlib GUI for visualizing a harmonic signal with noise and filtering."""

    def __init__(self) -> None:
        self.x = np.linspace(T_START, T_END, POINTS)
        self.show_noise = True
        self.rng_noise = self._generate_noise(
            INITIAL["noise_mean"], INITIAL["noise_covariance"]
        )

        self.fig, self.ax = plt.subplots(figsize=(10, 7))
        self.fig.canvas.manager.set_window_title("Lab 4 - Harmonic visualization")
        plt.subplots_adjust(left=0.10, right=0.96, top=0.92, bottom=0.36)

        clean, noisy, _ = harmonic_with_noise(
            INITIAL["amplitude"],
            INITIAL["frequency"],
            INITIAL["phase"],
            INITIAL["noise_mean"],
            INITIAL["noise_covariance"],
            self.show_noise,
            self.x,
            self.rng_noise,
        )
        filtered = lowpass_filter(noisy, INITIAL["cutoff_frequency"])

        (self.clean_line,) = self.ax.plot(self.x, clean, label="Clean harmonic", linewidth=2)
        (self.noisy_line,) = self.ax.plot(
            self.x, noisy, label="Noisy harmonic", alpha=0.65, linewidth=1
        )
        (self.filtered_line,) = self.ax.plot(
            self.x, filtered, label="Filtered harmonic", linewidth=2
        )

        self.ax.set_title("Harmonic signal, noise and Butterworth filtering")
        self.ax.set_xlabel("Time, t")
        self.ax.set_ylabel("Amplitude")
        self.ax.grid(True, alpha=0.3)
        self.ax.legend(loc="upper right")

        self.sliders = self._create_sliders()
        self._connect_sliders()
        self._create_buttons()
        self._create_help_text()

    def _generate_noise(self, mean: float, covariance: float) -> np.ndarray:
        std = np.sqrt(max(covariance, 0.0))
        return np.random.normal(mean, std, size=self.x.shape)

    def _create_sliders(self) -> dict[str, Slider]:
        axes = {
            "amplitude": plt.axes([0.20, 0.27, 0.62, 0.025]),
            "frequency": plt.axes([0.20, 0.23, 0.62, 0.025]),
            "phase": plt.axes([0.20, 0.19, 0.62, 0.025]),
            "noise_mean": plt.axes([0.20, 0.15, 0.62, 0.025]),
            "noise_covariance": plt.axes([0.20, 0.11, 0.62, 0.025]),
            "cutoff_frequency": plt.axes([0.20, 0.07, 0.62, 0.025]),
        }
        return {
            "amplitude": Slider(axes["amplitude"], "Amplitude", 0.1, 3.0, valinit=INITIAL["amplitude"]),
            "frequency": Slider(axes["frequency"], "Frequency", 0.05, 2.0, valinit=INITIAL["frequency"]),
            "phase": Slider(axes["phase"], "Phase", -np.pi, np.pi, valinit=INITIAL["phase"]),
            "noise_mean": Slider(axes["noise_mean"], "Noise mean", -1.0, 1.0, valinit=INITIAL["noise_mean"]),
            "noise_covariance": Slider(axes["noise_covariance"], "Noise covariance", 0.0, 1.0, valinit=INITIAL["noise_covariance"]),
            "cutoff_frequency": Slider(axes["cutoff_frequency"], "Cutoff frequency", 0.1, 20.0, valinit=INITIAL["cutoff_frequency"]),
        }

    def _connect_sliders(self) -> None:
        for name, slider in self.sliders.items():
            if name in {"noise_mean", "noise_covariance"}:
                slider.on_changed(self._update_noise_parameters)
            else:
                slider.on_changed(self._update_harmonic_parameters)

    def _create_buttons(self) -> None:
        reset_ax = plt.axes([0.06, 0.015, 0.12, 0.04])
        self.reset_button = Button(reset_ax, "Reset")
        self.reset_button.on_clicked(self._reset)

        checkbox_ax = plt.axes([0.82, 0.015, 0.15, 0.06])
        self.checkbox = CheckButtons(checkbox_ax, ["Show noise"], [self.show_noise])
        self.checkbox.on_clicked(self._toggle_noise)

    def _create_help_text(self) -> None:
        self.fig.text(
            0.20,
            0.005,
            "Instructions: move sliders to change harmonic, noise and filter parameters. "
            "Use Show noise to hide/show noise. Reset restores initial values.",
            fontsize=9,
        )

    def _current_values(self) -> dict[str, float]:
        return {name: slider.val for name, slider in self.sliders.items()}

    def _redraw(self, regenerate_noise: bool) -> None:
        values = self._current_values()
        if regenerate_noise:
            self.rng_noise = self._generate_noise(
                values["noise_mean"], values["noise_covariance"]
            )

        clean, noisy, _ = harmonic_with_noise(
            values["amplitude"],
            values["frequency"],
            values["phase"],
            values["noise_mean"],
            values["noise_covariance"],
            self.show_noise,
            self.x,
            self.rng_noise,
        )
        filtered = lowpass_filter(noisy, values["cutoff_frequency"])

        self.clean_line.set_ydata(clean)
        self.noisy_line.set_ydata(noisy)
        self.filtered_line.set_ydata(filtered)
        self.noisy_line.set_visible(self.show_noise)

        y_min = min(clean.min(), noisy.min(), filtered.min()) - 0.25
        y_max = max(clean.max(), noisy.max(), filtered.max()) + 0.25
        self.ax.set_ylim(y_min, y_max)
        self.fig.canvas.draw_idle()

    def _update_harmonic_parameters(self, _value: float) -> None:
        # Harmonic/filter changes keep the current noise sample unchanged.
        self._redraw(regenerate_noise=False)

    def _update_noise_parameters(self, _value: float) -> None:
        # Noise parameter changes regenerate only the noise.
        self._redraw(regenerate_noise=True)

    def _toggle_noise(self, _label: str) -> None:
        self.show_noise = not self.show_noise
        self._redraw(regenerate_noise=False)

    def _reset(self, _event) -> None:
        for name, value in INITIAL.items():
            self.sliders[name].reset()
        self.show_noise = True
        # CheckButtons has no direct simple reset in old matplotlib versions;
        # toggle only if the visual state is off.
        if not self.checkbox.get_status()[0]:
            self.checkbox.set_active(0)
        self.rng_noise = self._generate_noise(
            INITIAL["noise_mean"], INITIAL["noise_covariance"]
        )
        self._redraw(regenerate_noise=False)

    def run(self) -> None:
        plt.show()


def main() -> None:
    app = HarmonicApp()
    app.run()


if __name__ == "__main__":
    main()
