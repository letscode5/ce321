
Εδώ είναι η υλοποίηση των mythreads. Ο φάκελος περιέχει ένα makefile
το οποίο φτιάχνει το εκτελέσιμο με όνομα "4_2", το οποίο εκτελείται
"./4_2" χωρίς να θέλει κάποιο όρισμα. Το πρόγραμμα αυτό τρέχει κάποια
δοκιματσικά threads για να δούμε αν λειτουργούν καλά τα threads.


Αρχεία που περιέχονται:
- 4_2_mythreads.c : υλοποίηση των συναρτήσεων των mythreads και του
                    tuplespace
- 4_2_mythreads.h : header file της βιβλιοθήκης των threads
- 4_1_coro.c :      υλοποίηση των κορουτινών
- 4_2_coro.h :      header file της βιβλιοθήκης των κορουτινών
- Makefile 
- README


Το body από κάθε thread που δημιουργείται πρέπει να ξεκινά με την 
εντολή mythreads_start() και να τελειώνει με την εντολή mythreads_finish().


Η κλήση των συναρτήσεων mythreads_tuple_in και mythreads_tuple_out
γίνονται στο πνεύμα της printf και scanf. Π.χ. αν θέλω προσθέσω
την πλειάδα <"hello", 17> τότε πρέπει να καλέσω:
mythreads_tuple_out("%s %d", "hello", 17);

Τύποι δεδομένων και πώς καλούνται στο format της πλειάδας:
    %d  : integer
    %c  : character
    %lf : double
    %s  : string
Όπως ακριβώς και στην printf.

Στην συνάρτηση mythreads_tuple_in όταν θέλουμε να ζητήσουμε την 
τιμή μίας πλειάδας, στο format βάζουμε το σύμβολο '&' και μετά
τον τύπο δεδομένων που θέλουμε. Αν θέλουμε καρφωτή τιμή βάζουμε
το σύμβολο '%' όπως και παραπάνω

Τύποι δεδομένων στο format της mythreads_tuple_in:
    &t  : integer
    &h  : character
    &o  : double
    &t  : string

Π.χ. αν θέλουμε να κάνουμε in την πλειάδα <&str, 35.28, &integ>
πρέπει να καλέσουμε:
mythreads_tuple_in("&t %lf &t", &str, 35.28, &integ)