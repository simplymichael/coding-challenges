#include "./json-types.h"

JsonValue* parse_as_object(const char** text);
char* parse_as_string(const char** text);
JsonValue* parse_value(const char** text);
void skip_whitespace(const char** text);
