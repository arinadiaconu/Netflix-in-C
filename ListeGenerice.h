/* DIACONU Arina-Ioana */

/*-- ListeGenerice.h -- Lista generica simplu inalntuita
              si info = pointer la element --*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifndef _LISTA_SIMPLU_INLANTUITA_
#define _LISTA_SIMPLU_INLANTUITA_

typedef struct celula
{
  void *info;               // adresa element extern
  struct celula *urm;       // legatura spre celula urmatoare
} TCelula, *TLSI;

//inserarea primului element in lista vida
int InserareListaVida(TLSI *a, void *x);

//inserarea inainte de inceputul listei
int InserareListaInaintePrim(TLSI *a, void *x);

//inserare dupa un element
int InserareListaDupa(TLSI *a, void *x);

//distrugere lista
void DistrugeLista(TLSI *s);

#endif
