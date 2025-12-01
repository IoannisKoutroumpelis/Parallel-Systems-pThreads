# Ορισμός του compiler
CC = gcc

# Flags μεταγλώττισης
# -Wall -Wextra: Ενεργοποίηση προειδοποιήσεων για καθαρό κώδικα
# -O3: Μέγιστη βελτιστοποίηση ταχύτητας 

LDFLAGS = -lpthread

BIN_DIR = bin
SRC_DIR = src

TARGET_Q1_1 = $(BIN_DIR)/q1_1
TARGET_Q1_2 = $(BIN_DIR)/q1_2
TARGET_Q1_4 = $(BIN_DIR)/q1_4

SRCS_Q1_1 = $(SRC_DIR)/q1_1/q1_1_main.c $(SRC_DIR)/q1_1/poly.c
SRCS_Q1_2 = $(SRC_DIR)/q1_2/q1_2_main.c
SRCS_Q1_4 = $(SRC_DIR)/q1_4/q1_4_main.c

all: directories $(TARGET_Q1_1) $(TARGET_Q1_2) $(TARGET_Q1_4)

directories:
	mkdir -p $(BIN_DIR)

$(TARGET_Q1_1): $(SRCS_Q1_1)
	$(CC) $(CFLAGS) $(SRCS_Q1_1) -o $(TARGET_Q1_1) $(LDFLAGS)

$(TARGET_Q1_2): $(SRCS_Q1_2)
	$(CC) $(CFLAGS) $(SRCS_Q1_2) -o $(TARGET_Q1_2) $(LDFLAGS)

$(TARGET_Q1_4): $(SRCS_Q1_4)
	$(CC) $(CFLAGS) $(SRCS_Q1_4) -o $(TARGET_Q1_4) $(LDFLAGS)

q1_1: directories $(TARGET_Q1_1)
q1_2: directories $(TARGET_Q1_2)
q1_4: directories $(TARGET_Q1_4)

clean:
	rm -f $(BIN_DIR)/*


run_q1_1: $(TARGET_Q1_1)
	./$(TARGET_Q1_1) 10000 8

run_q1_2: $(TARGET_Q1_2)
	./$(TARGET_Q1_2) 3 8

run_q1_4: $(TARGET_Q1_4)
	./$(TARGET_Q1_4)