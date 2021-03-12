# Memory and Storage Performance Profiling
## Test Setups
### MLC
CPU Used: Intel Core i7-8700K 

Memory Specs: CORSAIR Vengeance LPX SDRAM DDR4 3200

In order to collect the data using MLC, the following process was used.

To get the loaded latency, the following command was used.

```mlc --loaded_latency ```

The default execution of this command gives 64B read only.
In order to change this to the 256B version, ```-Y``` needs to be added to the command like so

```mlc --loaded_latency -Y ```

To change the ratio of reads to writes, ```-Wn``` (where n is a number). For the following additional loaded latancy tests, ```-W2``` and ```-W6``` were used to conduct a 2:1 ratio and 100% non-temporal write tests. These commands were also combined with ```-Y``` in order to give the 256B versions of these tests. So for the 256B test for the 2:1 read to write ratio, the command imputed would be

```mlc --loaded_latency -W2 -Y ```

The combination of these commands were used to get all of the loaded latancy data documentated. More tests could be done however by changing the read to write ratio and by running the 512B versions using ```-Z``` if your CPU supports it.

### FIO
CPU Used:
Disk Specs: [Intel SSD 530 Series](https://ark.intel.com/content/www/us/en/ark/products/75331/intel-ssd-530-series-240gb-2-5in-sata-6gb-s-20nm-mlc.html)

 - Sequential read: Up to 540 MB/s
 - Sequential write: Up to 490 MB/s
 - Random read: Up to 41k IOPS
 - Random write: Up to 80k IOPS

In order to collect the latency and bandwidth data for FIO, the following process was used.

6 different tests were run, for random read, random write, and random readwrite at both 4k and 32k block sizes. Data was collected by creating jobs for FIO. For each I/O method and block size, each job has a different parameter for "iodepth". This parameter changes the amount of transfers submitted at once, therefore changing the bandwidth of the operation. The parameters for the jobs can be seen in the "fio_*" files. The data is collected using the script "gen_data_fio.sh". 

## Results
Raw data: https://docs.google.com/spreadsheets/d/1zwgUYmTq6vadqQo92bs4RTT5yU-sI8Gr8TbZ7Z6BY6A/edit?usp=sharing 

### Peak Memory Bandwidths
|Traffic| 16 byte (128 bit) load/stores| AVX 32 byte (256 bit) load/stores| Peak Cache Bandwidth|
|--|--|--|--|
| All Reads | 41786.7 | 40190.7 | 871412.4 |
| 3:1 Reads-Writes | 37251.2 | 36596.2 | 664840.5 |
| 2:1 Reads-Writes | 36420.7 | 35682.6 | 550508.1 |
| 1:1 Reads-Writes | 36011.8 | 35632.6 | 929121.0 |
| Stream-triad like | 36752.9 | 36389.4 | 121530.6 |

### Loaded Latencies for Memory - Read only, 64B
| Delay | Latency (ns) | Bandwidth (MB/sec) |
|--|--|--|
| 0 | 153.7 | 42174.7 |
| 2 | 161.87 | 41886.5 |
| 8 | 149.01 | 42117 |
| 15 | 147 | 42054.4 |
| 50 | 124.05 | 41443.7 |
| 100 | 87.5 | 38043.1 |
| 200 | 59.57 | 26639.9 |
| 300 | 53.51 | 18755 |
| 400 | 50.86 | 14745.9 |
| 500 | 48.98 | 12231 |
| 700 | 47.49 | 9230.6 |
| 1000 | 46.66 | 6905 |
| 1300 | 45.74 | 5703.8|
| 1700 | 44.80 | 4725.5 |
| 2500 | 45.68| 3656.3 |
| 3500 | 45.85 | 3002.6 |
| 5000 | 44.92 | 2556.2 |
| 9000 | 44.98 | 2052.9 |
| 20000 | 44.25 | 1740.5 |

![](Memory%20Test%20-%20Read%20Only%20Traffic%20-%2064B%20-%20Latency%20vs%20Bandwidth.png)

### Loaded Latencies for Memory - 2:1 Read:Write, 64B
| Delay    | Latency (ns) | Bandwidth (MB/sec) |
|----------|--------------|--------------------|
|        0 |       325.43 |            36164.3 |
|        2 |       318.53 |            36292.5 |
|        8 |       335.34 |            35683.3 |
|       15 |       302.86 |            36304.4 |
|       50 |       278.02 |            36980.1 |
|      100 |       297.33 |            36745.1 |
|      200 |       194.76 |            34869.3 |
|      300 |        77.52 |            26618.9 |
|      400 |        75.97 |            20468.5 |
|      500 |        61.96 |            17247.4 |
|      700 |        51.42 |            13071.5 |
|     1000 |         49.7 |             9597.8 |
|     1300 |        48.05 |             7787.9 |
|     1700 |        47.73 |             6273.7 |
|     2500 |        60.56 |             4262.4 |
|     3500 |        46.68 |               3770 |
|     5000 |        56.39 |             2793.2 |
|     9000 |        57.09 |             2034.6 |
|    20000 |        45.52 |             1830.6 |

![](Memory%20Test%20-%202_1%20Read_Write%20Traffic%20-%2064B%20-%20Latency%20vs%20Bandwidth.png)

### Loaded Latencies for Memory - 100% Non-Temporal Write. 64B
| Delay    | Latency (ns) | Bandwidth (MB/sec) |
|----------|--------------|--------------------|
|        0 |       325.43 |            36164.3 |
|        2 |       318.53 |            36292.5 |
|        8 |       335.34 |            35683.3 |
|       15 |       302.86 |            36304.4 |
|       50 |       278.02 |            36980.1 |
|      100 |       297.33 |            36745.1 |
|      200 |       194.76 |            34869.3 |
|      300 |        77.52 |            26618.9 |
|      400 |        75.97 |            20468.5 |
|      500 |        61.96 |            17247.4 |
|      700 |        51.42 |            13071.5 |
|     1000 |         49.7 |             9597.8 |
|     1300 |        48.05 |             7787.9 |
|     1700 |        47.73 |             6273.7 |
|     2500 |        60.56 |             4262.4 |
|     3500 |        46.68 |               3770 |
|     5000 |        56.39 |             2793.2 |
|     9000 |        57.09 |             2034.6 |
|    20000 |        45.52 |             1830.6 |


![](Memory%20Test%20-%20100%20Non-Temporal%20Write%20Traffic%20-%2064B%20-%20Latency%20vs%20Bandwidth.png)

### Loaded Latencies for Memory - Read only, 256B

| Delay | Latency (ns) | Bandwidth (MB/sec) |
| ----- | ------------ | ------------------ |
| 0     | 168.33       | 42263              |
| 2     | 192.19       | 41448.9            |
| 8     | 185.46       | 41833.9            |
| 15    | 162.87       | 42237.9            |
| 50    | 148.92       | 41726.2            |
| 100   | 137.41       | 39102.7            |
| 200   | 69.05        | 27186.5            |
| 300   | 63           | 19003.3            |
| 400   | 56.48        | 14871.5            |
| 500   | 52.53        | 12330.7            |
| 700   | 53.69        | 9109.9             |
| 1000  | 54.47        | 6730.4             |
| 1300  | 54.78        | 5412.2             |
| 1700  | 51.51        | 4485.4             |
| 2500  | 52.21        | 3432.5             |
| 3500  | 48.28        | 2924.4             |
| 5000  | 49.43        | 2418               |
| 9000  | 51.42        | 1864.4             |
| 20000 | 54.03        | 1464.9             |

![](Memory%20Test%20-%20Read%20Only%20Traffic%20-%20256B%20-%20Latency%20vs%20Bandwidth.png)

### Loaded Latencies for Memory - 2:1 Read:Write, 64B

| Delay | Latency (ns) | Bandwidth (MB/sec) |
| ----- | ------------ | ------------------ |
| 0     | 377.43       | 36400.7            |
| 2     | 332.22       | 36661.7            |
| 8     | 333.71       | 36582.3            |
| 15    | 369.29       | 36072.4            |
| 50    | 370.64       | 36263.5            |
| 100   | 310.92       | 36937.2            |
| 200   | 143.18       | 36188.7            |
| 300   | 71.7         | 26670.9            |
| 400   | 69.18        | 20290.8            |
| 500   | 57.78        | 17383.7            |
| 700   | 53.34        | 13003.9            |
| 1000  | 59.47        | 9275.8             |
| 1300  | 64.19        | 7227.5             |
| 1700  | 50.35        | 6196.7             |
| 2500  | 58.46        | 4381.8             |
| 3500  | 52.33        | 3534.2             |
| 5000  | 48.59        | 2973.9             |
| 9000  | 46.22        | 2320.8             |
| 20000 | 48.35        | 1736.3             |

![](Memory%20Test%20-%202_1%20Read_Write%20Traffic%20-%20256B%20-%20Latency%20vs%20Bandwidth.png)


### Loaded Latencies for Memory - 100% Non-Temporal Write. 64B

| Delay | Latency (ns) | Bandwidth (MB/sec) |
| ----- | ------------ | ------------------ |
| 0     | 527.89       | 42674.1            |
| 2     | 455.06       | 42829.7            |
| 8     | 446.83       | 42971.1            |
| 15    | 355.29       | 43454.8            |
| 50    | 65.72        | 23561.9            |
| 100   | 56.08        | 12851.5            |
| 200   | 53.05        | 7633.3             |
| 300   | 51.75        | 5631.6             |
| 400   | 49.8         | 4662.1             |
| 500   | 48.61        | 4047.8             |
| 700   | 47.03        | 3337.7             |
| 1000  | 46.81        | 2764.9             |
| 1300  | 46.41        | 2451.2             |
| 1700  | 46.1         | 2214.9             |
| 2500  | 46.18        | 1950.6             |
| 3500  | 45.64        | 1799.5             |
| 5000  | 45.51        | 1684.8             |
| 9000  | 45.7         | 1556.6             |
| 20000 | 46.3         | 1453.1             |


![](Memory%20Test%20-%20100%20Non-Temporal%20Write%20Traffic%20-%20256B%20-%20Latency%20vs%20Bandwidth.png)

### SSD Random Read - 4kB Block Size

| Read Latency (us) | Read BW (kB/s) |
| ----------------- | -------------- |
| 859.528706        | 8585.110169    |
| 904.051366        | 13016.28205    |
| 1239.719861       | 16697.37705    |
| 1838.818726       | 18027.74074    |
| 2664.052681       | 20845.92       |
| 4136.374178       | 20274.88235    |
| 6990.430364       | 20042.29412    |
| 12425.86004       | 19597.88235    |
| 22235.56495       | 19831.41177    |
| 36450.56771       | 20083.5098     |
| 62741.85295       | 19802          |

![](Disk%20Test%20-%20Read%20Only%20Traffic%20-%204kb%20Block%20Size%20-%20Latency%20vs%20Bandwidth.png)

### SSD Random Write - 4kB Block Size

| Write Latency (us) | Write BW (kB/s) |
| ------------------ | --------------- |
| 1437.784195        | 3133.520249     |
| 2442.555136        | 3710.773585     |
| 4288.97997         | 4017.348018     |
| 7876.013111        | 4401.248804     |
| 14282.93057        | 5317.518135     |
| 27718.10812        | 5525.730159     |
| 55476.41823        | 5513.415789     |
| 110344.9203        | 5618.392473     |
| 221929.6087        | 5496.863158     |
| 436942.7172        | 5627.48913      |
| 885823.5433        | 5494.746032     |

![](Disk%20Test%20-%20Write%20Only%20Traffic%20-%204kb%20Block%20Size%20-%20Latency%20vs%20Bandwidth.png)

### SSD Random Read/Write - 4kB Block Size

| Read Latency (us) | Read BW (kB/s) | Write Latency (us) | Write BW (kB/s) |
| ----------------- | -------------- | ------------------ | --------------- |
| 8284.754142       | 2214.739316    | 1167.053979        | 2202.978632     |
| 9018.882346       | 2711.769634    | 1921.979507        | 2723.732984     |
| 14390.78258       | 3282.012658    | 3196.644225        | 3279.21519      |
| 21751.93054       | 3722.131387    | 5408.640032        | 3725.50365      |
| 23325.82084       | 4134.47619     | 9386.842483        | 4141.84127      |
| 33981.57415       | 4070.20155     | 17827.23955        | 4044.713178     |
| 48180.01938       | 3981.007634    | 34654.03413        | 3969.59542      |
| 73441.34852       | 3947.427481    | 67570.36022        | 3943.038168     |
| 106740.5819       | 3946.145038    | 133701.8348        | 3970.145038     |
| 160502.0788       | 3976.122137    | 265753.6778        | 3969.40458      |
| 240284.4302       | 3911.396947    | 530481.7959        | 3926.480916     |

![](Disk%20Test%20-%20Read-Write%20Traffic%20-%204kb%20Block%20Size%20-%20Latency%20vs%20Bandwidth.png)

### SSD Random Read - 32kB Block Size

| Read Latency (us) | Read BW (kB/s) |
| ----------------- | -------------- |
| 3067.61228        | 27533.86111    |
| 3749.178018       | 32087.66667    |
| 4529.174256       | 32111.96154    |
| 4846.087944       | 39685.20833    |
| 7253.66386        | 49862.09524    |
| 10666.41204       | 44695.27273    |
| 18674.2782        | 44849.45455    |
| 35200.64959       | 43119.30435    |
| 60717.39801       | 43472.69565    |
| 104326.857        | 44794.18182    |
| 236622.2544       | 43720.72727    |

![](Disk%20Test%20-%20Read%20Only%20Traffic%20-%2032kb%20Block%20Size%20-%20Latency%20vs%20Bandwidth.png)

### SSD Random Write - 32kB Block Size

| Write Latency (us) | Write BW (kB/s) |
| ------------------ | --------------- |
| 3057.876788        | 11431.51648     |
| 5712.529452        | 11967.95238     |
| 9410.553834        | 15040.46269     |
| 16268.44685        | 17934.50877     |
| 29057.58136        | 20453.64        |
| 55896.13485        | 21678.25        |
| 114688.319         | 20925.95918     |
| 223723.0894        | 21325.33333     |
| 450705.2097        | 21048.16327     |
| 910706.5667        | 20532           |
| 1800895.297        | 20376.51064     |

![](Disk%20Test%20-%20Write%20Only%20Traffic%20-%2032kb%20Block%20Size%20-%20Latency%20vs%20Bandwidth.png)

### SSD Random Read/Write - 32kB Block Size

| Read Latency (us) | Read BW (kB/s) | Write Latency (us) | Write BW (kB/s) |
| ----------------- | -------------- | ------------------ | --------------- |
| 9097.857247       | 5525.457831    | 2321.336642        | 5554.168675     |
| 9844.622609       | 7746.060606    | 3910.334806        | 7845.166667     |
| 13428.78894       | 8638.316667    | 7079.569474        | 8794.566667     |
| 20003.73966       | 11224.68889    | 11784.66342        | 11251.68889     |
| 30445.48931       | 13727.76316    | 20160.57316        | 13576.26316     |
| 41828.91764       | 13951.76316    | 39230.2499         | 13552.55263     |
| 61999.0483        | 13231.07692    | 79418.82109        | 13287.02564     |
| 90198.94991       | 13011.69231    | 160220.1719        | 13211.89744     |
| 141055.281        | 12626.84615    | 320307.0861        | 13071.35897     |
| 227942.1255       | 13068.02564    | 634840.2099        | 12940.33333     |
| 363420.9721       | 13106.16216    | 1223884.828        | 13190.91892     |

![](Disk%20Test%20-%20Read-Write%20Traffic%20-%2032kb%20Block%20Size%20-%20Latency%20vs%20Bandwidth.png)

### SSD IOPS

The `--terse` output mode for FIO, which is used to export semicolon separated values to automate data collection and graph creation, does not output the sampled IOPS in the version of FIO we are running. Our version of FIO uses the V3 terse format, but this information is only outputted in the V5 format. As a result, to collect IOPS data, we take the IOPS for peak 32kB read and write bandwidth, and record the average IOPS.

| IO Mode    | Latency (usec) | Bandwidth (kB/s) | IOPS  |
| ---------- | -------------- | ---------------- | ----- |
| 32kB Read  | 1228.00        | 389120.00        | 12160 |
| 32kB Write | 2361.33        | 201488.00        | 6297  |

## Analysis and Conclusion

### Overview

In all cases, we observe that as we increase the data access stress to memory, the bandwidth of data transferred (written or read) increases, but so does the latency. Unsurprisingly, the memory is far faster than the SSD tested, with the bandwidths for memory measured in GB/S, while in MB/s for the disk. Similarly, the latencies are in the nanoseconds for the ram, while in the disk they are in the microsecond and millisecond ranges. 

### Comparing Block Sizes 

For the memory tests, between 64B and 256B, there is only a small difference observed in the read and write only tests. We see that for the same bandwidths, the latency is slightly higher for 256B transfers, which makes sense as slightly more time is necessary to setup the larger read/write. This difference is magnified in the combined read-write test, where for the same higher bandwidths the latency got as far as 1.4 times larger. This suggests that there is not much overhead in setting up the memory transfers.



For the disk tests we observe large differences between the block sizes. The 4kb read tests have a peak bandwidth of slightly over 20 MB/s, but the 32kb read test have a peak of almost 50MB/s. However, the latency is also higher, from about 2000us to 7000us. We see a similar difference for the write only test, where 4kb peaks at 5.8MB/s, but 32kb reaches over 20MB/s. We also observe this higher bandwidth and latency for the combined read write test. This suggests that a lot of overhead is involved in setting up the transfers, meaning the 4kb transfer has more overhead per data transferred.



Unsurprisingly, the difference between block sizes suggests that there is more overhead for writing to the disk, since the SSD needs to manage where to write the data.

### Comparing Reads and Writes

For the memory tests we observe that, for each block size, the write tests have a slightly lower peak bandwidth, and reach their peak bandwidth at a significantly lower latency (about half the latency). Therefore, it does not come as a surprise that while the peak bandwidth of the combined read-write test is similar to that of the read test, but the latency higher, similar to that of the write test.

For the disk tests, we observe that the read is again much faster than the write. This is expected, given the performance specifications of the SSD. For the 4KB block size, reads have about 4 times as much bandwidth for a given latency. For the 32KB block size, reads have about 2 times as much bandwidth for a given latency. The combined random read-write tests from FIO appear to balance bandwidth for reads and writes for both block sizes. Interestingly, for both block sizes at lower bandwidths/latencies the write has a lower latency, but higher latency for high bandwidths. 



### Comparison to Enterprise Intel SSD

As described above, we don't collect IOPS for each test due to limitations in the `--terse` output format. We observe the peak read IOPS in these tests is about 12k, far less than the rated 41k IOPS for random read. Similarly, the measured 6k IOPS for random write is far less than the rated 80k for random write. Unsurprisingly, the measurements for this disk are far below the rated specifications for the listed enterprise Intel SSD. 

