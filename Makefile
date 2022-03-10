F1=main.c
F2=Coada_lg.c
F3=Functii_seriale.c
F4=ListeGenerice.c
F5=Stiva_lg.c

build: 
	gcc -Wall -o tema2 $(F1) $(F2) $(F3) $(F4) $(F5)
clean:
	rm -rf tema2