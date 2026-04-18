#include <regex.h>
#include <stdio.h>

#include "../headers/validator.h"

int has_trailing_comma(const char *json_str) {
  regex_t regex;
  int reti = regcomp(&regex, ",[[:space:]]*[}\\]]", REG_EXTENDED);

  reti = regexec(&regex, json_str, 0, NULL, 0);
  regfree(&regex);
    
  return (reti == 0); // Returns 1 if a trailing comma is found
}

int validate(const char* json_str) {
  int is_trailing_comma_error = has_trailing_comma(json_str);

  if(is_trailing_comma_error != 0) {
    fprintf(stderr, "Invalid JSON: trailing comma found.");
    return EXIT_FAILURE;
  }

  return 0;
}
