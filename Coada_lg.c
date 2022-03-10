/* DIACONU Arina-Ioana */

/* Coada_lg.c  -  operatii cu cozi implementate cu ajutorul
                    listelor generice simplu inlantuite */

#include "Coada_lg.h"

//initializarea unei cozi cu elemente de dimensiune d
void* InitQ(size_t d)   
{
    AQ c;
    c = (AQ)malloc(sizeof(TCoada));
    if(!c)  return NULL;    //testul alocarii

    c->dime = d;
    c->ic = NULL;
    c->sc = NULL;

    return (void *)c;
}

//inserarea unui element la sfarsitul cozii
int InsQ(void *c, void *adr_el)
{
    ALC aux;
    aux = (ALC)malloc(sizeof(TLC));
    if(!aux)    return 0;   //testul alocarii
    //se aloca memorie si se copiaza elementul
    aux->info = malloc(DIMEC(c));
    if(!aux->info)          //testul alocarii
    {
        free(aux);
        return 0;
    }
    
    memcpy(aux->info, adr_el, DIMEC(c));
    aux->urm = NULL;    
    if(VIDAC(c))    //coada vida
    {
        IC(c) = aux;
        SC(c) = aux;
    }
    else
    {
        SC(c)->urm = aux;
        SC(c) = aux;
    }
    return 1;
}

int PrimQ(void *c, void *adr_el)    //primul element din coada
{
    if(VIDAC(c))    return 0;
    memcpy(adr_el, IC(c)->info, DIMEC(c));
    return 1;
}

int ExtrQ(void *c, void *adr_el)    //extragerea primului element din coada
{
    if(VIDAC(c)) return 0;
    ALC aux = IC(c);
    IC(c) = aux->urm;
    
    //se copiaza elementul la adresa primita ca parametru
    memcpy(adr_el, aux->info, DIMEC(c));
    free(aux->info);    //eliberarea memoriei
    free(aux);
    if(IC(c) ==  NULL)  //daca dupa extragere coada este vida
    {
        SC(c) = NULL;
    }

    return 1;
}

void ResetQ(void *c)    //transforma coada intr-o coada vida
{
    ALC aux = IC(c), p;
    while(aux)
    {
        p = aux;
        aux = aux->urm;
        free(p->info);      //eliberarea memoriei
        free(p);
    }
    IC(c) = NULL;
    SC(c) = NULL;
}

void DistrQ(void **c)   //elibereaza tot spatiul ocupat de coada
{
    ResetQ(*c);
    free(*c);
    *c = NULL;
}