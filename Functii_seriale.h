/* DIACONU Arina-Ioana */

/*           --- Functii_seriale.h ---
        functii de prelucrare a serialelor */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "ListeGenerice.h"
#include "Coada_lg.h"
#include "Stiva_lg.h"

#ifndef _TAB_SERIALE_
#define _TAB_SERIALE_

typedef void (*TF)(void *);           // functie eliberare un element
typedef void (*TFA)(void *, FILE *);  // functie afisare element

//structura pentru tabela de categorii de seriale
typedef struct
{
    size_t M;   //numarul de liste din tabela
    TLSI *v;    //vectorul de liste
} TH;

//structura pentru un sezon
typedef struct sezon
{
    int nr_ep;          //numar episoade sezon
    void *ep;           //coada de episoade
}Tsezon;

//structura pentru un serial
typedef struct serial
{
    int id;             //id al categoriei din care face parte
    char *nume;         //numele unic al serialului
    float rating;       //ratingul serialului
    int nr_sez;         //numarul de sezoane din serial 
    void *serial;       //coada de sezoane
    int nevizionat;     //numarul de minute nevizionate din serial
    int poz;            //pozitia in top10
}Tserial;

/* functii tabela */

TH* InitializareTabela(size_t M);       //initializare tabela
//eliberarea intregului spatiu ocupat de tabela
void DistrugeTabela(TH **ah, TF feliberare_element);
//afisarea unei categorii de seriale
void AfisareCategorie(TH *ah, int x, TFA afisareElem, FILE *f);
void Afisare_Coada(void *c, TFA f, FILE *file); //afisarea unei cozi
void Afisare_Stiva(void *s, TFA f, FILE *file); //afisarea unei stive
int InserareTabela(TH *a, void *ae, int x); //inserare ordonata in tabela
int InserareTop(TLSI *a, void *ae, TF f);   //inserarea in categoria top10
//pozitia in tabela a unui serial dupa inserare
int Pozitie(TH *a, void *ae, int x);
//adauga un sezon nou unui serial
int add_sez(void *p, int nr_ep, int *v);
//cauta un serial in tabela si daca il gaseste, 
//apeleaza functia ce adauga sezonul (add_sez)
int Ad_sezon_tab(TH *h, char *nume, int nr_ep, int *v);
//cauta un serial intr-o coada (watch_later) si adauga sezonul nou
int Ad_sezon_watch(void *c, char *nume, int nr_ep, int *v);
//cauta un serial intr-o stiva (currently_watching) si adauga sezonul nou
int Ad_sez_current(void *s, char * nume, int nr_ep, int *v);
//cauta un serial in tabela de seriale si intoarce celula de lista
TLSI cauta_serial(TH *h, char *nume, TF f);
//ultima pozitie dintr-o coada
int pozitie_coada(void *c);
//reseteaza pozitiile serialelor din top 
//si elimina serialele care au iesit din top10
void ReseteazaTop(TLSI *a, TF f);
//cauta un serial intr-o stiva pentru a-l viziona
int exista_stiva(void *s, void *h, char *nume, int min, FILE *f);
//cauta un serial intr-o coada si il insereaza in stiva primita ca parametru
int exista_coada(void *c, void *s, char *nume, int min, FILE *f);

#endif