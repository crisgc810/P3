import matplotlib as mpl
import matplotlib.pyplot as plt
import soundfile as sf
import numpy as np
from wave import WAVE_FORMAT_PCM
import wave

#leemos el fichero
prueba, fs = sf.read("draeiou.wav")

#escogemos el tiempo en muestras el cual analizaremos
prueba=prueba[112000:112480]
time_sf = (np.linspace(start=0, stop = (len(prueba)-1), num = len(prueba)))/fs
#calculamos la autocorrelacion
correlation = np.correlate(prueba, prueba, "full")
correlation = correlation/correlation[int(len(correlation)/2)]
correlation = correlation[int(len(correlation)/2):]



#señal
plt.subplot(2,1,1)
plt.title("Señal temporal")
plt.plot(time_sf, prueba)

#autocorrelación
plt.subplot(2,1,2)
plt.title("Autocorrelación")
plt.plot(np.arange(len(correlation)), correlation)
plt.show()
