COUNTS_MEAN=1000
COUNTS_STDEV=0
NUM_MEAN=( 100 1000 10000 100000 )
NUM_STDEV=0

OUT_FILE="encode_out_single_threaded.csv"

TIMEFORMAT=%R
#> $OUT_FILE
#echo "Number of rows, Time to encode" >> $OUT_FILE

#for NUM in "${NUM_MEAN[@]}"
#do
    #python gen_input_file.py words_alpha.txt out.txt $COUNTS_MEAN $COUNTS_STDEV $NUM $NUM_STDEV
    #echo -n `wc -l <out.txt` >> $OUT_FILE
    #TIME="$(time ( ./a.out out.txt 1 ) 2>&1 1>/dev/null )"
    #echo -n "," >> $OUT_FILE
    #echo $TIME >> $OUT_FILE
    #rm out.txt
    #echo "Finished $NUM"
#done

OUT_FILE="encode_out_threads.csv"
> $OUT_FILE

NUM_MEAN=100000
NUM_THREADS=( 1 2 4 8 16 )
python gen_input_file.py words_alpha.txt out.txt $COUNTS_MEAN $COUNTS_STDEV $NUM_MEAN $NUM_STDEV
for THREADS in "${NUM_THREADS[@]}"
do
    echo -n $THREADS >> $OUT_FILE
    TIME="$(time ( ./a.out out.txt $THREADS ) 2>&1 1>/dev/null )"
    echo -n "," >> $OUT_FILE
    echo $TIME >> $OUT_FILE
    echo "Finished $THREADS"
done