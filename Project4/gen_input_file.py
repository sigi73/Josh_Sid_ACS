import argparse
import random
import sys
import time

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="TODO")
    parser.add_argument('dictionary_file', type=str, help='Filename of dictionary file to read from')
    parser.add_argument('out_file', type=str, help='Filename to write output to')
    parser.add_argument('mean_words', type=int, help='Mean number of unique words to use')
    parser.add_argument('std_words', type=int, help='Standard deviation of number of unique words to use')
    parser.add_argument('mean_num', type=int, help='Mean number of times to repeat each word')
    parser.add_argument('std_num', type=int, help='Standard deviation of number of times to repeat each word')

    args = parser.parse_args()

    f_in = open(args.dictionary_file, 'r')
    if f_in is None:
        print("Error reading", args.dictionary_file)
        sys.exit(1)

    words = f_in.readlines()
    f_in.close()

    random.seed(time.time())
    num_words = int(random.gauss(args.mean_words, args.std_words))

    out_list = []
    for i in range(num_words):
        word = random.choice(words)
        num = int(random.gauss(args.mean_num, args.std_num))
        new = [word] * num
        out_list.extend(new)
    
    random.shuffle(out_list)

    f_out = open(args.out_file, 'w')
    if f_out is None:
        print("Error writing", args.out_file)
        sys.exit(2)
    
    for item in out_list:
        f_out.write(item)