# ProgettoAPI_2019-2020

Il progetto consiste nell’implementare un semplice editor di testi. L’editor con-
sidera un documento come una sequenza di righe, di dimensione arbitraria,
numerate a partire da uno.
L’interfaccia dell’editor è costituita da comandi testuali, terminati da un
carattere di “a capo”. I comandi possono essere seguiti da una porzione di
testo, costituita da una o più righe, terminata da un carattere . (punto) che
compare come unico carattere sulla riga successiva. I comandi sono costituiti
da un’unica lettera, preceduta opzionalmente da uno o due numeri interi.
L’interfaccia dell’editor è liberamente ispirata a quella del tradizionale editor
ed.
In alcuni comandi, gli interi che compaiono in essi rappresentano degli speci-
ficatori di indirizzo. Più precisamente, uno specificatore di indirizzo è un numero
n, espresso in decimale, che indica l’indirizzo della n-esima riga; la prima riga
del testo ha indirizzo 1.
I comandi supportati sono i seguenti, con la convenzione che ind1,ind2
indicano due specificatori di indirizzo tali che ind1  ind2 e le parentesi tonde
sono introdotte per semplicità di lettura di questo testo, ma non incluse nel
comando:
• (ind1,ind2)c
Cambia il testo presente alle righe comprese tra ind1 ed ind2 (estremi
inclusi). Il testo che segue il comando deve essere fatto di un numero di
righe pari a ind2-ind1+1. ind1 deve essere o un indirizzo e↵ettivamente
presente nel testo, oppure il primo indirizzo dopo l’ultima riga presente
nel testo (o 1 se il testo è ancora vuoto).
• (ind1,ind2)d
Cancella le righe comprese tra ind1 ed ind2 (estremi inclusi), spostando
verso l’alto le righe successive a quella di indirizzo ind2 (se ce ne sono).
La cancellazione di una riga che non esiste nel testo non ha alcun e↵etto.
• (ind1,ind2)p
Stampa le righe tra ind1 ed ind2, inclusi. Laddove non sia presente
una riga nel testo nella posizione da stampare, viene stampata una riga
contenente il solo carattere ’.’ seguito da un “a capo”.
• (numero)u
