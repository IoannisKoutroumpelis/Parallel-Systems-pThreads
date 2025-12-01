CC = gcc
CFLAGS = -Wall -Wextra -O3 -Iinclude
LDFLAGS = -lpthread

BIN_DIR = bin
SRC_DIR = src

TARGET_Q1_1 = $(BIN_DIR)/q1_1
TARGET_Q1_2 = $(BIN_DIR)/q1_2
TARGET_Q1_3 = $(BIN_DIR)/q1_3
TARGET_Q1_4 = $(BIN_DIR)/q1_4
TARGET_Q1_5_PTHREAD = $(BIN_DIR)/q1_5_pthread
TARGET_Q1_5_COND = $(BIN_DIR)/q1_5_cond
TARGET_Q1_5_SENSE = $(BIN_DIR)/q1_5_sense

SRCS_Q1_1 = $(SRC_DIR)/q1_1/q1_1_main.c $(SRC_DIR)/q1_1/poly.c
SRCS_Q1_2 = $(SRC_DIR)/q1_2/q1_2_main.c
SRCS_Q1_3 = $(SRC_DIR)/q1_3/q1_3_main.c
SRCS_Q1_4 = $(SRC_DIR)/q1_4/q1_4_main.c
SRCS_Q1_5_PTHREAD = $(SRC_DIR)/q1_5/q1_5_pthread.c
SRCS_Q1_5_COND = $(SRC_DIR)/q1_5/q1_5_cond.c
SRCS_Q1_5_SENSE = $(SRC_DIR)/q1_5/q1_5_sense.c

.PHONY: all directories clean run_q1_1 run_q1_2 run_q1_3 run_q1_4 run_q1_5 test clean_test

all: directories $(TARGET_Q1_1) $(TARGET_Q1_2) $(TARGET_Q1_3) $(TARGET_Q1_4) $(TARGET_Q1_5_PTHREAD) $(TARGET_Q1_5_COND) $(TARGET_Q1_5_SENSE)

directories:
	mkdir -p $(BIN_DIR)

$(TARGET_Q1_1): $(SRCS_Q1_1)
	$(CC) $(CFLAGS) $(SRCS_Q1_1) -o $(TARGET_Q1_1) $(LDFLAGS)

$(TARGET_Q1_2): $(SRCS_Q1_2)
	$(CC) $(CFLAGS) $(SRCS_Q1_2) -o $(TARGET_Q1_2) $(LDFLAGS)

$(TARGET_Q1_3): $(SRCS_Q1_3)
	$(CC) $(CFLAGS) $(SRCS_Q1_3) -o $(TARGET_Q1_3) $(LDFLAGS)

$(TARGET_Q1_4): $(SRCS_Q1_4)
	$(CC) $(CFLAGS) $(SRCS_Q1_4) -o $(TARGET_Q1_4) $(LDFLAGS)

# Κανόνες για q1_5
$(TARGET_Q1_5_PTHREAD): $(SRCS_Q1_5_PTHREAD)
	$(CC) $(CFLAGS) $(SRCS_Q1_5_PTHREAD) -o $(TARGET_Q1_5_PTHREAD) $(LDFLAGS)

$(TARGET_Q1_5_COND): $(SRCS_Q1_5_COND)
	$(CC) $(CFLAGS) $(SRCS_Q1_5_COND) -o $(TARGET_Q1_5_COND) $(LDFLAGS)

$(TARGET_Q1_5_SENSE): $(SRCS_Q1_5_SENSE)
	$(CC) $(CFLAGS) $(SRCS_Q1_5_SENSE) -o $(TARGET_Q1_5_SENSE) $(LDFLAGS)

q1_1: directories $(TARGET_Q1_1)
q1_2: directories $(TARGET_Q1_2)
q1_3: directories $(TARGET_Q1_3)
q1_4: directories $(TARGET_Q1_4)
q1_5: directories $(TARGET_Q1_5_PTHREAD) $(TARGET_Q1_5_COND) $(TARGET_Q1_5_SENSE)

clean:
	rm -f $(BIN_DIR)/*

run_q1_1: $(TARGET_Q1_1)
	./$(TARGET_Q1_1) 10000 8

run_q1_2: $(TARGET_Q1_2)
	./$(TARGET_Q1_2) 3 8

run_q1_3: $(TARGET_Q1_3)
	./$(TARGET_Q1_3) 1000000

run_q1_4: $(TARGET_Q1_4)
	./$(TARGET_Q1_4) 100 100 70 2 4

run_q1_5: $(TARGET_Q1_5_PTHREAD) $(TARGET_Q1_5_COND) $(TARGET_Q1_5_SENSE)
	./$(TARGET_Q1_5_PTHREAD) 4 100000
	./$(TARGET_Q1_5_COND) 4 100000
	./$(TARGET_Q1_5_SENSE) 4 100000
	./$(TARGET_Q1_5_COND) 4 100000
	./$(TARGET_Q1_5_SENSE) 4 100000

test:
	chmod +x test.sh
	./test.sh

make clean_test: 
	rm -f test/fig/*
	rm -f test/results/*