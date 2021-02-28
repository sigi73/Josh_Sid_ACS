#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <zlib.h>
#include <string.h>


int main()
{


	char a[1000] = "Hello, world!kfshdjkfhdfsfsdfsdfsdafasdfdgdsfgdfgsdfgsdfgsdfgsdfgdfgsdfgssdfbdfbdfgdfdfbdfbdfbdfbfbfbdfbsdfbdfbdbsdkfhjsdfhfjkshfksdjhf";
	char b[1000];
	
	uLong ucompSize = strlen(a)+1; // "Hello, world!" + NULL delimiter.
	uLong compSize = compressBound(ucompSize);

	compress((Bytef *)b, &compSize, (Bytef *)a, ucompSize);

	printf("uncompressed length: %ld\n", strlen(a));
	printf("compressed length: %ld\n", strlen(b));

	/* code */



	return 0;
}