#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./headers/json-types.h"
#include "./headers/string.h"


// Load file into string
char* load_file(const char* filename) { 
  FILE* f = fopen(filename, "r");

  if(!f) { 
  	return NULL; 
  }

  // Get the size of the file
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  // Read the file's contents
  char* data = malloc(size + 1);
  fread(data, 1, size, f);
  data[size] = '\0';

  fclose(f);
  return data;
}


int main(int argc, char* argv[]) {
  if(argc != 2) {
  	fprintf(stderr, "Usage: %s </path/to/json-file>.json\n", argv[0]);
  	return EXIT_FAILURE;
  }

  const char* filename = argv[1];
  const char* file_contents = load_file(filename);
  JsonValue* json = parse_value(&file_contents);

  if(json == NULL) {
  	fprintf(stderr, "Error parsing JSON: invalid JSON file %s\n", argv[1]);
  	return 1;
  } else { 
  	printf("JSON file %s successfully parsed!\n", argv[1]);
  	return 0;
  }
}