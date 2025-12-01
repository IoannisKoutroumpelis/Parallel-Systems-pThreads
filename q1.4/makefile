CC = gcc
CFLAGS = -pthread -Wall -O3
TARGET = bank_sim
SRC = bank_simulation.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) 1000 10000 90 3 4