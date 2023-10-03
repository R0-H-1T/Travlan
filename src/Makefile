CC = gcc

# CFLAGS = -Wall -Wextra -pedantic -std=c99
LIBS = -lncurses -lmenu -lsqlite3 -lbcrypt

# SOURCES = main.c database.c login.c recommendations.c reports.c trips.c
SOURCES = main.c

# HEADERS = database.h login.h recommendations.h reports.h trips.h
HEADERS = database.h login.h

INCLUDE_SEARCH_PATH = libbcrypt
LIBRARY_SEARCH_PATH = libbcrypt

TARGET = main

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) -I$(INCLUDE_SEARCH_PATH) -L$(LIBRARY_SEARCH_PATH) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY:all clean

