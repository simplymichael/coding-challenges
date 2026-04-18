#include <stdlib.h>
#include <string.h>

#include "../headers/json-types.h"
#include "../headers/string.h"
#include "../headers/validator.h"

// Read object like { "key": value, ... }
// @param {pointer to string } text.
// The string itself is `text` (char* text). 
// The pointer to the string is `*text` (char** text).
JsonValue* parse_as_object(const char** text) { 
  JsonValue* obj = (JsonValue*)malloc(sizeof(JsonValue));
  obj->type = JSON_TYPE_OBJECT;
  obj->objectValue.pairs = malloc(sizeof(JsonPair) * 20);
  obj->objectValue.count = 0;

  // Skip the opening '{'
  (*text)++; 
  skip_whitespace(text);

  while(**text != '}' && **text != '\0') { 
    skip_whitespace(text);

    char* key = parse_as_string(text);

    skip_whitespace(text);
        
    if(**text == ':') { 
      (*text)++; 
    }

    skip_whitespace(text);

    JsonValue* value = parse_value(text);

    obj->objectValue.pairs[obj->objectValue.count].key = key;
    obj->objectValue.pairs[obj->objectValue.count].value = value;
    obj->objectValue.count++;

    skip_whitespace(text);

    if(**text == ',') {
      (*text)++;
      skip_whitespace(text);
    }
  }

  if(**text == '}') { 
    // Skip the closing '}'
    (*text)++; 
  }

  return obj;
}

// Read string value such as "name"
// @param {pointer to string } text
// The string itself is `text` (char* text). 
// The pointer to the string is `*text` (char** text).
char* parse_as_string(const char** text) {
  if (**text != '"') { 
    return NULL; 
  }

  (*text)++;  // Skip first quote

  const char* start = *text;

  while (**text != '"' && **text != '\0') {
    (*text)++;
  }

  int length = *text - start;
  char* result = (char*)malloc(length + 1);

  strncpy(result, start, length);
  result[length] = '\0';

  if (**text == '"') { 
    (*text)++;  // Skip closing quote
  }

  return result;
}

// Read any value (string, number, bool, null, object)
// @param {pointer to string } text
// The string itself is `text` (char* text). 
// The pointer to the string is `*text` (char** text).
JsonValue* parse_value(const char** text) { 
  skip_whitespace(text);

  if(**text == '{') { 
    return parse_as_object(text);
  }

  return NULL;
}

// Skip spaces and tabs
// @param {pointer to string} text
// The string itself is `text` (char* text). 
// The pointer to the string is `*text` (char** text).
void skip_whitespace(const char** text) { 
  // Read the character at the current cursor/pointer location.
  // If the character is a whitespace character...
  while(**text == ' ' || **text == '\n' || **text == '\t') { 
    (*text)++; // ...move the cursor/pointer forward
  }
}
