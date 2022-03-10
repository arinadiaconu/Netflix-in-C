/* DIACONU Arina-Ioana */

/* Stiva_lg.c  -  operatii cu stive implementate cu ajutorul
                    listelor generice simplu inlantuite */

#include "Stiva_lg.h"

void* InitS(size_t d)   //initializarea unei stive
{
    AST s;
    s = (AST)malloc(sizeof(TStiva));
    if(!s)  return NULL;    //testul alocarii

    s->dime = d;    //dimensiunea elementelor din stiva
    s->vf = NULL;

    return (void *)s;
}

int Push(void *s, void *adr_el) //inserarea in varful stivei
{
    ALSt aux;
    aux = (ALSt)malloc(sizeof(TLSt));
    if(!aux)    return 0;   //testul alocarii
    //se aloca si se copiaza elementul in stiva
    aux->info = malloc(((AST)s)->dime);
    memcpy(aux->info, adr_el, ((AST)s)->dime);

    aux->urm = ((AST)s)->vf;
    ((AST)s)->vf = aux;

    return 1;
}

int Pop(void *s, void *adr_el)  //extragerea unui element din varful stivei
{
    ALSt aux = VF(s);
    if(aux == NULL) return 0;
    //se copiaza elementul la adresa primita ca parametru
    memcpy(adr_el, aux->info, ((AST)s)->dime);
    VF(s) = aux->urm;

    free(aux->info);    //eliberarea memoriei
    free(aux);

    return 1;
}

int Top(void *s, void *adr_el)  //primul element din stiva
{
    if(VIDA(s))     return 0;
    memcpy(adr_el, VF(s)->info, DIMES(s));
    return 1;
}

void ResetS(void *s)    //transforma stiva in stiva vida
{
    ALSt aux = VF(s), p;
    while(aux)
    {
        p = aux;
        aux = aux->urm;
        free(p->info);  //eliberarea memoriei
        free(p);
    }
    VF(s) = NULL;
}

void DistrS(void **s)   //eliberarea intregului spatiu ocupat de stiva
{
    ResetS(*s);
    free(*s);
    *s = NULL;
}