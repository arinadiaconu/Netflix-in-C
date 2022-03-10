/* DIACONU Arina-Ioana */

/*              --- ListeGenerice.c ---
      operatii lista generica simplu inlantuita */

#include "ListeGenerice.h"

//inserarea primului element
int InserareListaVida(TLSI *a, void *x)  
{
	//se aloca o celula de lista
    TLSI aux = (TLSI)malloc(sizeof(TCelula));
	if(!aux) return 0;      //testarea alocarii
	
    //atribuirea pointerului la continutul listei
    aux->info = x;

    aux->urm = NULL;

    //se retine primul element din lista
    *a = aux;

    return 1;
}

//inserare inainte de primul element din lista
int InserareListaInaintePrim(TLSI *a, void *x) 
{
    //se aloca o celula de lista
    TLSI aux = (TLSI)malloc(sizeof(TCelula));
	if(!aux) return 0;      //testarea alocarii

    //atribuirea pointerului la continutul listei
    aux->info = x;
    
    //se refac legaturile
    aux->urm = *a;
    
    //se retine adresa noului prim element din lista
    *a = aux;

    return 1;
}

//inserare inainte de un element diferit de primul
int InserareListaDupa(TLSI *a, void *x)
{
    //se aloca o celula de lista
    TLSI aux = (TLSI)malloc(sizeof(TCelula));
	if(!aux) return 0;      //testarea alocarii

    //atribuirea pointerului la continutul listei
    aux->info = x;

    //se refac legaturile
    aux->urm = (*a)->urm;
    (*a)->urm = aux;

    return 1;
}

// transforma lista in lista vida
void DistrugeLista(TLSI *s)
{
    TLSI p = *s, aux;
	while(p != NULL)   //parcurgerea listei
	{
		aux = p;    
		p = p->urm;
		free(aux->info);    //eliberarea informatiei
		free(aux);          //eliberarea celulei
	}
    *s = NULL;
}