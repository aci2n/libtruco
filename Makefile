CC = gcc
CFLAGS = -g

OUTPUT_DIR = out

SOURCES = src/*.c src/*.h
OUTPUT = $(OUTPUT_DIR)/libtruco.o

TEST_SOURCES = test/*.c
TEST_OUTPUT = $(OUTPUT_DIR)/libtruco_test

build: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) -c -o $(OUTPUT) src/libtruco.c

test: $(TEST_OUTPUT)
	./$(TEST_OUTPUT)

$(TEST_OUTPUT): $(SOURCES) $(TEST_SOURCES)
	$(CC) $(CFLAGS) -o $(TEST_OUTPUT) test/libtruco_test.c

clean:
	rm -f $(OUTPUT_DIR)/*
