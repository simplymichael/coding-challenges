#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
  if(argc != 3) {
  	fprintf(stderr, "Kindly specify a file (test.txt) or enter a text on the CLI.\r\n");
  	return 1;
  }

  FILE* in;
  char ch;
  char * op;

  while((ch = getopt(argc, argv, "c")) != EOF) {
  	switch(ch) {
  	  case 'c': 
  	  	op = "bytes"; 
  	  	break;
  	}
  }

  argc -= optind;
  argv += optind;

  if(!(in = fopen(argv[0], "r"))) { 
  	fprintf(stderr, "File %s not found\r\n", argv[0]);
  	return 1;
  }

  if(op == "bytes") { 
  	int bytes;
    for(bytes = 0; getc(in) != EOF; ++bytes); // See https://stackoverflow.com/a/25613854/1743192

    printf("  %i %s\r\n", bytes, argv[0]);
  }


  fclose(in);
  return 0;
}
