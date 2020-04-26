#------------------------------------------------------------------------
#  CPP-SDL2 sketches
#  Copyright (c) 2019-2020 - Trung Truong
# 
#  File name: Makefile
#  Description: Create executables
#------------------------------------------------------------------------

OBJECTS  = $(SOURCES:.c=.o)

#  Compiler Flags
CC 			= gcc
C++			= g++
CFLAGS      = -g -std=c99 -pedantic -Wall -Werror -Wextra \
              -Wno-overlength-strings -Wfatal-errors -pedantic
C++FLAGS    = -g -std=c++11 -pedantic -Wall -Werror -Wextra \
              -Wno-overlength-strings -Wfatal-errors -pedantic
LDFLAGS     = -lSDL2
CPPFLAGS    = -I.

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS) $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(C++) -c -o $@ $< $(CPPFLAGS) $(C++FLAGS)

# Clean up executables
clean: 
	rm -rf bin/* src/*.o tests/*.o core* *~ tests/test-*

# Make all executables
all:
	make barnsley
	make raycast

# Individual executables
barnsley: src/barnsley-fern.o
	$(CC) $(LDFLAGS) -o bin/barnsley-fern src/barnsley-fern.o

raycast: src/ray-casting.o
	$(C++) $(LDFLAGS) -o bin/raycast src/ray-casting.o
