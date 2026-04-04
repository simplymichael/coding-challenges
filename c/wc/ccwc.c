#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int count_bytes(FILE* file) { 
  int bytes;
  for(bytes = 0; getc(file) != EOF; ++bytes); // See https://stackoverflow.com/a/25613854/1743192

  return bytes;
}

int count_lines(FILE* file) { 
  // See: https://stackoverflow.com/a/70708991/1743192

  #define BUF_SIZE 65536

  int lines = 0;
  char buf[BUF_SIZE];

  while(1) {
    size_t res = fread(buf, 1, BUF_SIZE, file);
    
    if(ferror(file)) {
      return -1; 
    }

    int i;
    for(i = 0; i < res; i++) {
      if(buf[i] == '\n') { 
        lines++;
      }
    }

    if(feof(file)) { 
      break;
    }
  }

  return lines;
}


int main(int argc, char* argv[]) {
  if(argc != 3) {
  	fprintf(stderr, "Kindly specify a file (test.txt) or enter a text on the CLI.\r\n");
  	return 1;
  }

  FILE* in;

  if(!(in = fopen(argv[2], "r"))) { 
    fprintf(stderr, "File %s not found\r\n", argv[2]);
    return 1;
  }

  char * op;
  char ch;
  int result;

  while((ch = getopt(argc, argv, "cl")) != EOF) {
  	switch(ch) {
  	  case 'c': result = count_bytes(in); break;
      case 'l': result = count_lines(in); break;
  	}
  }

  argc -= optind;
  argv += optind;

  printf("  %i %s\r\n", result, argv[0]);
  fclose(in);

  return 0;
}
