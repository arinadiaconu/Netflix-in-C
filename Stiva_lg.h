/* DIACONU Arina-Ioana */

/* Stiva_lg.h  -  stiva implementata cu ajutorul listelor 
                        generice, simplu inlantuite */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef _STIVA_
#define _STIVA_

typedef struct lstiva
{ 
	struct lstiva *urm;     //adresa urmatoarei celule
    void *info;             //adresa informatiei
} TLSt, *ALSt;

typedef struct stiva
{
    size_t dime;            //dimensiune element
    ALSt vf;                //adresa celulei din varf
}TStiva, *AST;


/*- macrodefinitii - acces campuri */
#define DIMES(a) (((AST)(a))->dime) //dimensiunea unui element din stiva
#define VF(a) 	(((AST)(a))->vf)    //varful stivei

/*- teste -*/
/* verifica daca o stiva este vida */
#define VIDA(a)  	(VF(a) == NULL)

#endif

#ifndef STIVA_FUNCTII
#define STIVA_FUNCII

void* InitS(size_t d);            //initializare stiva vida
int Push(void *s, void *adr_el);  // insereaza element in varful stivei
int Pop(void *s, void *adr_el);   // extrage elementul din varful stivei
int Top (void *s, void *adr_el);  // copiaza elementul din varful stivei 
                                  // la adresa adr_el
void ResetS(void *s);   // transforma stiva in stiva vida
void DistrS(void **s);  // elibereaza intregul spatiu ocupat de stiva

#endif