COMPILE_FLAGS = -Wall -Wextra -I ./includes/

all: string.o json-parser

string.o: ./headers/json-types.h ./headers/string.h ./includes/string.c
	gcc ${COMPILE_FLAGS} ./includes/string.c -c -o ./bin/string.o

json-parser: ./headers/json-types.h ./headers/string.h ./bin/string.o ./json-parser.c 
	gcc ${COMPILE_FLAGS} ./json-parser.c ./bin/string.o -o ./bin/json-parser

clean:
	rm -f ./bin/*.o ./bin/json-parser
