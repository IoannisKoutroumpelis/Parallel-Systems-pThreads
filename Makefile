# Ορισμός του compiler
CC = gcc

# Flags μεταγλώττισης
# -Wall -Wextra: Ενεργοποίηση προειδοποιήσεων για καθαρό κώδικα
# -O3: Μέγιστη βελτιστοποίηση ταχύτητας (σημαντικό για μετρήσεις απόδοσης)
CFLAGS = -Wall -Wextra -O3 -Iinclude

# Flags για τον linker (απαραίτητο για τα threads)
LDFLAGS = -lpthread

# Φάκελοι
BIN_DIR = bin
SRC_DIR = src

# Στόχοι (Targets)
TARGET_Q1_1 = $(BIN_DIR)/q1_1
TARGET_Q1_2 = $(BIN_DIR)/q1_2
TARGET_Q1_4 = $(BIN_DIR)/q1_4

# Αρχεία πηγαίου κώδικα
SRCS_Q1_1 = $(SRC_DIR)/q1_1/q1_1_main.c $(SRC_DIR)/q1_1/poly.c
SRCS_Q1_2 = $(SRC_DIR)/q1_2/q1_2_main.c
SRCS_Q1_4 = $(SRC_DIR)/q1_4/q1_4_main.c

# Default target (εκτελείται όταν γράφεις σκέτο 'make')
all: directories $(TARGET_Q1_1) $(TARGET_Q1_2) $(TARGET_Q1_4)

# Δημιουργία φακέλου bin αν δεν υπάρχει
directories:
	mkdir -p $(BIN_DIR)

# Κανόνας για q1_1
$(TARGET_Q1_1): $(SRCS_Q1_1)
	$(CC) $(CFLAGS) $(SRCS_Q1_1) -o $(TARGET_Q1_1) $(LDFLAGS)

# Κανόνας για q1_2
$(TARGET_Q1_2): $(SRCS_Q1_2)
	$(CC) $(CFLAGS) $(SRCS_Q1_2) -o $(TARGET_Q1_2) $(LDFLAGS)

# Κανόνας για q1_4
$(TARGET_Q1_4): $(SRCS_Q1_4)
	$(CC) $(CFLAGS) $(SRCS_Q1_4) -o $(TARGET_Q1_4) $(LDFLAGS)

# Κανόνες για μεμονωμένη μεταγλώττιση
q1_1: directories $(TARGET_Q1_1)
q1_2: directories $(TARGET_Q1_2)
q1_4: directories $(TARGET_Q1_4)

# Κανόνας για καθαρισμό (διαγραφή) των παραγόμενων αρχείων
clean:
	rm -f $(BIN_DIR)/*

# Κανόνες για να τρέξουν τα προγράμματα με ενδεικτικές τιμές
run_q1_1: $(TARGET_Q1_1)
	./$(TARGET_Q1_1) 10000 8

run_q1_2: $(TARGET_Q1_2)
	./$(TARGET_Q1_2) 3 8

run_q1_4: $(TARGET_Q1_4)
	./$(TARGET_Q1_4)