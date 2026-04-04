#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <unistd.h>
#include <wchar.h>

long count_bytes(FILE* file) { 
  // See https://stackoverflow.com/a/25613854/1743192

  long count;
  for(count = 0; getc(file) != EOF; ++count);

  return count;
}

long count_chars(FILE* file) { 
  long count = 0;

  // If current locale supports multibyte sequences 
  // handle UTF-8 correctly, in a locale-aware way.
  if(setlocale(LC_ALL, "") != NULL) {
    wint_t wc; 

    // Read wide characters until EOF
    while((wc = fgetwc(file)) != WEOF) {
      count++;
    }
  } else { 
    char ch;

    while((ch = fgetc(file)) != EOF) {
      count++; 
    }

    // Alternatively, we can reuse the count_bytes function and just do 
    // return count_bytes(file);
  }

  return count;
}

long count_lines(FILE* file) { 
  // See: https://stackoverflow.com/a/70708991/1743192

  #define BUF_SIZE 65536

  long count = 0;
  char buf[BUF_SIZE];

  while(1) {
    size_t res = fread(buf, 1, BUF_SIZE, file);
    
    if(ferror(file)) {
      return -1; 
    }

    int i;
    for(i = 0; i < res; i++) {
      if(buf[i] == '\n') { 
        count++;
      }
    }

    if(feof(file)) { 
      break;
    }
  }

  return count;
}

long count_words(FILE* file) {
  // See: https://stackoverflow.com/a/28892601/1743192

  char ch;
  long count = 0;
  int PreviousWasSpace = 1;
   
  while((ch = fgetc(file)) != EOF) {
    if(isspace(ch)) {
      PreviousWasSpace = 1;
    } else {
      if(PreviousWasSpace) { 
        count++; 
      }

      PreviousWasSpace = 0;
    }
  }
   
  return count;
}


int main(int argc, char* argv[]) {
  if(argc < 2) {
  	fprintf(stderr, "Kindly specify a file (test.txt) or enter a text on the CLI.\r\n");
  	return 1;
  }

  FILE* in;
  int file_pos = (argc == 2 ? 1 : 2);

  if(!(in = fopen(argv[file_pos], "r"))) { 
    fprintf(stderr, "File %s not found\r\n", argv[file_pos]);
    return 1;
  }

  char * op;
  char ch;
  long result;

  while((ch = getopt(argc, argv, "clmw")) != EOF) {
  	switch(ch) {
  	  case 'c': result = count_bytes(in); break;
      case 'l': result = count_lines(in); break;
      case 'm': result = count_chars(in); break;
      case 'w': result = count_words(in); break;
  	}
  }

  argc -= optind;
  argv += optind;

  if(result) {
    printf("  %li %s\r\n", result, argv[0]);
  } else {
    long words = count_words(in);
    fclose(in);

    in = fopen(argv[0], "r");

    long lines = count_lines(in);
    fclose(in);

    in = fopen(argv[0], "r");

    long bytes = count_bytes(in);

    printf(
      "  %li %li %li %s\r\n", 
      lines,
      words,  
      bytes,
      argv[0]
    );
  }
  
  fclose(in);

  return 0;
}
