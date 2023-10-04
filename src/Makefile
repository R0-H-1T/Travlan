CC = gcc

# CFLAGS = -Wall -Wextra -pedantic -std=c99
LIBS = -lncurses -lmenu -lsqlite3 -l:bcrypt.a

# SOURCES = main.c database.c login.c mymenu.c recommendations.c reports.c trips.c
SOURCES = main.c database.c auth.c mymenu.c util.c

# HEADERS = database.h login.h mymenu.h recommendations.h reports.h trips.h
HEADERS = database.h auth.h mymenu.h util.h

INCLUDE_SEARCH_PATH = libbcrypt
LIBRARY_SEARCH_PATH = libbcrypt

TARGET = main

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) $(HEADERS) -I$(INCLUDE_SEARCH_PATH) -L$(LIBRARY_SEARCH_PATH) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY:all clean
