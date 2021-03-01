THREAD_COUNTS=( 1 2 3 4 5 6 7 8 9 10 )
OUT_FILE="10G_out_v2_ssd.csv"
#FN="/home/siddharth/Data/LargeFiles/10G_base64.txt"
FN="/home/siddharth/Development/10G_base64.txt"

TIMEFORMAT=%R

> $OUT_FILE

for THREAD in "${THREAD_COUNTS[@]}"
do
    echo -n $THREAD >> $OUT_FILE
    echo -n "," >> $OUT_FILE
done
echo "" >> $OUT_FILE

for THREAD in "${THREAD_COUNTS[@]}"
do
    COMMAND="./project2 $FN $THREAD"
    #echo $COMMAND
    TIME="$(time ( $COMMAND ) 2>&1 1>/dev/null )"
    echo -n $TIME >> $OUT_FILE
    echo -n "," >> $OUT_FILE
done
echo "" >> $OUT_FILE
