import matplotlib.pyplot as plt

m = [7,8,9,10,11,12]

gcc_trace  = [25.72,21.66,18.00,15.33,13.46,12.30]
perl_trace = [20.60,16.17,13.79,11.62,10.59,08.94]
jpeg_trace = [07.84,07.74,07.69,07.65,07.51,07.48]

plt.plot(m,gcc_trace,  '-o', label = 'gcc_trace')
plt.plot(m,perl_trace, '-X', label = 'perl_trace')
plt.plot(m,jpeg_trace, '-*', label = 'jpeg_trace')

plt.xlabel('m - lower order pc bits')
plt.ylabel('mispredction rate')

plt.legend()
plt.show()