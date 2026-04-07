#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
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

void count_all(FILE* in, long *words, long *lines, long *bytes, long *chars) {
  *words = *lines = *bytes = *chars = 0;

  int c;
  int previous_was_space = 1;

  while((c = fgetc(in)) != EOF) {
    (*bytes)++;

    if(c == '\n') {
      (*lines)++;
    }

    if(isspace(c)) {
      previous_was_space = 1;
    } else {
      if(previous_was_space) {
        (*words)++;
      }

      previous_was_space = 0;
    }

    // UTF-8 character counting:
    // A new character starts whenever we see a byte that is not a continuation (0b10xxxxxx).
    /*if((c & 0xC0) != 0x80) {
      (*chars)++;
    }*/

    // Character counting (UTF-8 friendly):
    // Continuation bytes are 128–191. If not in that range, it's a new character.
    if(c < 128 || c > 191) {
      (*chars)++;
    }
  }
}


int main(int argc, char* argv[]) {
  FILE* in;

  if(argc == 1) {
    in = stdin;
  } else if(argc == 2) { 
    in = fopen(argv[1], "r");

    if(!in) {
      in = stdin;
    }

    if(!in) { 
      fprintf(stderr, "Error: cannot open file %s\n", argv[1]);
      return EXIT_FAILURE;
    }
  } else if(argc == 3) {
    in = fopen(argv[2], "r"); 

    if(!in) { 
      fprintf(stderr, "Error: cannot open file %s\n", argv[2]);
      return EXIT_FAILURE;
    }
  }

  char * op;
  char ch;
  long result = 0;

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
    (argv[0] 
      ? printf("  %li %s\n", result, argv[0])
      : printf("  %li\n", result)
    );
  } else { 
    long words;
    long lines;
    long bytes;
    long chars;

    count_all(in, &words, &lines, &bytes, &chars);

    (argv[0] 
      ? printf("  %li %li %li %s\n", lines, words, bytes, argv[0])
      : printf("  %li %li %li\n", lines, words, bytes)
    );
  }
  
  if(in != stdin) { 
    fclose(in);
  }

  return 0;
}
