# Ορισμός του compiler
CC = gcc

# Flags μεταγλώττισης
# -Wall -Wextra: Ενεργοποίηση προειδοποιήσεων για καθαρό κώδικα
# -O3: Μέγιστη βελτιστοποίηση ταχύτητας (σημαντικό για μετρήσεις απόδοσης)
CFLAGS = -Wall -Wextra -O3

# Flags για τον linker (απαραίτητο για τα threads)
LDFLAGS = -lpthread

# Το όνομα του εκτελέσιμου αρχείου που θα παραχθεί
TARGET = poly_mult

# Τα αρχεία πηγαίου κώδικα
SRCS = main.c poly.c

# Μετατροπή των .c αρχείων σε .o (object files)
OBJS = $(SRCS:.c=.o)

# Default target (εκτελείται όταν γράφεις σκέτο 'make')
all: $(TARGET)

# Κανόνας σύνδεσης (Linking)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Γενικός κανόνας για μεταγλώττιση .c σε .o
# Το $< είναι το αρχείο .c και το $@ είναι το αρχείο .o
%.o: %.c poly.h
	$(CC) $(CFLAGS) -c $< -o $@

# Κανόνας για καθαρισμό (διαγραφή) των παραγόμενων αρχείων
clean:
	rm -f $(OBJS) $(TARGET)

# Κανόνας για να τρέξει το πρόγραμμα με ενδεικτικές τιμές
run: $(TARGET)
	./$(TARGET) 10000 8