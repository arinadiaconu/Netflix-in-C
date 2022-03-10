/* DIACONU Arina-Ioana */

/* Coada_lg.h  -  coada implementata cu ajutorul listelor
                        generice, simplu inlantuite */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef _COADA_
#define _COADA_

typedef struct lcoada
{ 
	struct lcoada *urm;     //adresa urmatoarei celule
    void *info;             //adresa informatiei
} TLC, *ALC;

typedef struct coada
{
    size_t dime;    //dimensiune element
    ALC ic, sc;     //adresa primei, respectiv a ultimei celule
}TCoada, *AQ;

/*- macrodefinitii - acces campuri -*/
#define IC(a) 	(((AQ)(a))->ic)     //inceputul cozii
#define SC(a) 	(((AQ)(a))->sc)     //sfarsitul cozii
#define DIMEC(a) (((AQ)(a))->dime)  //dimensiunea unui element

/*- teste si prelucrari -*/
// verifica daca o coada este vida
#define VIDAC(a) 		(IC(a) == NULL && SC(a) == NULL)

#endif

#ifndef COADA_FUNCTII
#define COADA_FUNCTII

void* InitQ(size_t d);  //creeaza o coada vida cu elemente de dimensiune d
int InsQ(void *c, void *adr_el);   //adauga element la sfarsitul cozii 
int ExtrQ(void *c, void *adr_el);  //extrage primul element din coada 
int PrimQ(void *c, void *adr_el);  //copiaza primul element din coada
                                   //la adresa adr_el
void ResetQ(void *c);              //transforma coada in coada vida 
void DistrQ(void **c);      //elibereaza intregul spatiu ocupat de coada

#endif

