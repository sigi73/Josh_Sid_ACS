import pandas as pd
import matplotlib.pyplot as plt

import sys

bw_data = pd.read_csv(sys.argv[1], names=('Time', 'BW', 'dir', 'Size', 'Unknown'))
lat_data = pd.read_csv(sys.argv[2], names=('Time', 'Latency', 'dir', 'Size', 'Unknown'))

bw_data.set_index('Time')
lat_data.set_index('Time')

bw_data = bw_data.drop_duplicates('Time')
lat_data = lat_data.drop_duplicates('Time')

out = bw_data.merge(lat_data)

#print(out)
out.plot.scatter(x='Latency', y='BW')
plt.title(sys.argv[3])
plt.xlabel(sys.argv[4])
plt.ylabel(sys.argv[5])
#plt.show()
plt.savefig(sys.argv[6])
