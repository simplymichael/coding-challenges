#include <stdlib.h>

#include "../headers/json-types.h"
#include "../headers/string.h"

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
  }

  if(**text == '}') { 
    // Skip the closing '}'
    (*text)++; 
  }

  return obj;
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
