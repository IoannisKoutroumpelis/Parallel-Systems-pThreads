# Πολυνηματικός Προγραμματισμός – Εργασία Q1

Το παρόν README περιγράφει την υλοποίηση και τις βασικές προγραμματιστικές επιλογές για τα ερωτήματα **Q1.1–Q1.5**, όπως υλοποιήθηκαν στα αρχεία:

- `q1_1_main.c`, `poly.c`, `poly.h`  
- `q1_2_main.c`  
- `q1_3_main.c`  
- `q1_4_main.c`  
- `q1_5_pthread.c`, `q1_5_cond.c`, `q1_5_sense.c`  

---

## 0. Οδηγίες Μεταγλώττισης & Εκτέλεσης

Ενδεικτικά (ανάλογα με το layout των φακέλων):

```bash
# Q1.1 – Πολλαπλασιασμός πολυωνύμων
gcc -O2 -pthread q1_1_main.c poly.c -o q1_1

# Q1.2 – Mutex / RWLock / Atomic
gcc -O2 -pthread q1_2_main.c -o q1_2

# Q1.3 – 4 πίνακες / στατιστικά
gcc -O2 -pthread q1_3_main.c -o q1_3

# Q1.4 – Τραπεζικοί λογαριασμοί & στρατηγικές κλειδώματος
gcc -O2 -pthread q1_4_main.c -o q1_4

# Q1.5 – Barriers
gcc -O2 -pthread q1_5_pthread.c -o q1_5_pthread
gcc -O2 -pthread q1_5_cond.c    -o q1_5_cond
gcc -O2 -pthread q1_5_sense.c   -o q1_5_sense
