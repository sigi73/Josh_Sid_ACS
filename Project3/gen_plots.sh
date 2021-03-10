#rm *.log
#rm *-job-*
#fio fio_jobs
python plot.py read-4k_bw.1.log read-4k_lat.1.log "Intel SSD 530 Read Only 4kB Latency vs BW" "Latency (usecs)" "Bandwidth (KB/sec)" read_4k.png
python plot.py read-32k_bw.2.log read-32k_lat.2.log "Intel SSD 530 Read Only 32kB Latency vs BW" "Latency (usecs)" "Bandwidth (KB/sec)" read_32k.png
python plot.py write-4k_bw.3.log write-4k_lat.3.log "Intel SSD 530 Write Only 4kB Latency vs BW" "Latency (usecs)" "Bandwidth (KB/sec)" write_4k.png
python plot.py write-32k_bw.4.log write-32k_lat.4.log "Intel SSD 530 Write Only 32kB Latency vs BW" "Latency (usecs)" "Bandwidth (KB/sec)" write_32k.png
python plot.py rw-4k_bw.5.log rw-4k_lat.5.log "Intel SSD 530 Read-Write 4kB Latency vs BW" "Latency (usecs)" "Bandwidth (KB/sec)" rw_4k.png
python plot.py rw-32k_bw.6.log rw-32k_lat.6.log "Intel SSD 530 Read-Write 32kB Latency vs BW" "Latency (usecs)" "Bandwidth (KB/sec)" rw_32k.png
