
/* Define JsonPair */
#ifndef JSON_PAIR 
#define JSON_PAIR 

typedef struct {
  char* key;
  struct JsonValue* value;
} JsonPair;

#endif

/* End define JsonPair */

/* Define JsonType */
#ifndef JSON_TYPE
#define JSON_TYPE

// Type of JSON values
typedef enum { 
  JSON_TYPE_NULL, 
  JSON_TYPE_BOOL, 
  JSON_TYPE_NUMBER, 
  JSON_TYPE_STRING, 
  JSON_TYPE_ARRAY, 
  JSON_TYPE_OBJECT 
} JsonType;

#endif

/* End define JsonType */

/* Define JsonValue */

#ifndef JSON_VALUE
#define JSON_VALUE

// JSON value structure
typedef struct JsonValue {
  JsonType type;
  union {
    char* stringValue;
    double numberValue;
    int boolValue;
    struct {
      JsonPair* pairs;
      int count;
    } objectValue;
  };
} JsonValue;

#endif

/* End define JsonValue */
