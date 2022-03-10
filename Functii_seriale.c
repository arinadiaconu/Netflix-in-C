/* DIACONU Arina-Ioana */

/*  Functii_seriale.c - functii prelucrare seriale */


#include "Functii_seriale.h"

TH* InitializareTabela(size_t M)
{
    //alocare tabela
    TH* h = (TH *) calloc(sizeof(TH), 1);
    if (!h)     return NULL;     //testarea alocarii
        
    //alocare vectori de pointeri la lista simplu inlantuita generica
    h->v = (TLSI *) calloc(M, sizeof(TLSI));
    if(!h->v)   //testarea alocarii
    {
        free(h);
        return NULL;
    }

    h->M = M;   //lungimea vectorului de pointeri
    return h;
}

void DistrugeTabela(TH **ah, TF feliberare_element)
{
    TLSI *p, el, aux;
    int i;
    //parcurgerea tabelei cu pointeri
    for (i = 0, p = (*ah)->v; p < (*ah)->v + (*ah)->M; p++, i++) 
    {
        //daca exista elemente corespunzatoare acestei categorii
        if(*p != NULL)
        {
            //se parcurge lista
            for(el = *p; el != NULL; ) 
            {
                //se elibereaza continutul celulei, iar apoi celula
                aux = el;
                el = el->urm;
                feliberare_element(aux->info);
                free(aux);
            }
            (*ah)->v[i] = NULL;
        }
    }
    free((*ah)->v); //eliberarea vectorului de pointeri la liste
    free(*ah);  //eliberarea tabelei
    *ah = NULL;
}

void AfisareCategorie(TH *ah, int x, TFA afisareElem, FILE *f)
{
    TLSI p = ah->v[x], el;

    fprintf(f, "[");
    if(p)   //daca lista nu este vida
    { 
        for(el = p; el != NULL; el = el->urm)   //se parcurge lista
        {
            if(el->urm == NULL)     //ultimul serial de afisat
            {
                afisareElem(el->info, f);
            }
            else    //daca nu a ajuns la ultimul serial
            {
                afisareElem(el->info, f);
                fprintf(f, ", ");   //se afiseaza virgula intre seriale
            }
        }
    }
    fprintf(f, "].\n");
}

void Afisare_Coada(void *c, TFA f, FILE *file)
{
    void *q = InitQ(sizeof(Tserial));    //coada auxiliara
    if(!q)  return;                 //testul initializarii
    void *el = malloc(DIMEC(c));      //elementul auxiliar
    if(!el)                           //testul alocarii
    {
        DistrQ(&q);
        return;
    }
    fprintf(file, "[");
    while(!VIDAC(c))    //cat timp nu e vida coada
    {
        ExtrQ(c, el);   //se extrage element
        f((Tserial *)el, file); //afisare
        //se afiseaza virgula doar daca e diferit de ultimul element
        if((SC(c) != NULL) && (IC(c) != NULL))
        {
            fprintf(file, ", ");
        }
        InsQ(q, el);    //se insereaza in q
    }
    while(!VIDAC(q))    //se reface coada initiala
    {
        ExtrQ(q, el);
        InsQ(c, el);
    }
    DistrQ(&q);     //eliberarea memoriei
    free(el);
    fprintf(file, "].\n");
}

void Afisare_Stiva(void *s, TFA f, FILE *file)
{
    void *s_aux = InitS(DIMES(s));   //stiva auxiliara
    if(!s_aux)  return;         //testul initializarii
    void *el = malloc(DIMES(s));    //elementul auxiliar
    if(!el)                         //testul alocarii
    {
        DistrS(&s_aux);
        return;
    }
    fprintf(file, "[");
    while(!VIDA(s))    //cat timp nu e vida stiva
    {
        Pop(s, el);   //se extrage element
        f((Tserial *)el, file);
        if(!VIDA(s))
        {
            //se afiseaza virgula doar daca e diferit de ultimul element
            fprintf(file, ", ");
        }
        Push(s_aux, el);    //se insereaza in s_aux
    }
    while(!VIDA(s_aux))    //se reface stiva initiala
    {
        Pop(s_aux, el);
        Push(s, el);
    }
    DistrS(&s_aux);      //eliberarea memoriei
    free(el);
    fprintf(file, "].\n");
}

//se insereaza elementul si se intoarce rezultatul inserarii
int InserareTabela(TH *a, void *ae, int x)
{
    int rez = 0;
    TLSI el = a->v[x], ant = a->v[x];
    Tserial *s = (Tserial *)ae;
    
    if(el != NULL)  //daca lista nu este vida
    {
        //se parcurge lista pentru a-l insera ordonat
        for(; el != NULL;) 
        {
            Tserial *l = (Tserial *)(el->info);
            if(s->rating > l->rating)   //rating mai mare
            {
                //inserare inainte de primul element
                if(el == a->v[x])
                {
                    rez = InserareListaInaintePrim(a->v+x, ae);
                    return rez;
                }
                else    //inserarea inainte de un element diferit de primul
                {
                    rez = InserareListaDupa(&ant, ae);
                    return rez;
                }
            }
            else if(s->rating == l->rating) //ratinguri egale
            {
                //daca numele elementului din lista este mai mic
                //din punct de vedere lexicografic
                if(strcmp(l->nume, s->nume) < 0)    
                {
                    //daca elementul este diferit de ultimul
                    if(el->urm != NULL) 
                    {
                        if((((Tserial*)(el->urm->info))->rating)!=l->rating)
                        {
                            //daca urmatorul element nu are acelasi rating
                            rez = InserareListaDupa(&el, ae);
                            return rez;
                        }
                    }
                    else    //inserarea ultimului element
                    {
                        rez = InserareListaDupa(&el, ae);
                        return rez;
                    }
                }
                else    //daca este mai mare
                {
                    if(el == a->v[x])   //daca se insereaza inainte de
                                        //primul element
                    {
                        rez = InserareListaInaintePrim(a->v+x, ae);
                        return rez;
                    }
                    else
                    {
                        rez = InserareListaDupa(&ant, ae);
                        return rez;
                    }
                }
            }
            else if(el->urm == NULL)      //inserarea se va face la final
            {
                rez = InserareListaDupa(&el, ae);
                return rez;
            }

            if(el != a->v[x]) ant = ant->urm; //avansul elementului anterior
            el = el->urm;   //avansul in lista
        }
    }
    else    //lista este vida, se insereaza un singur element
    {
        rez = InserareListaVida(a->v+x, ae);
        return rez;
    }
    return rez;
}

void ReseteazaTop(TLSI *a, TF f)
{
    TLSI p = *a, aux, q, ant = p;
    int i = 0;
    //se reseteaza pozitiile din top10
    for(i = 1; (i < 11) && (p != NULL); p = p->urm)
    {
        if(i != 1) ant = ant->urm;  //avansul elementului anterior
        ((Tserial *)p->info)->poz = i;
        i++;
    }
    ant->urm = NULL;
    aux = p;
    //se elibereaza memoria pentru toate serialele care au iesit din top
    for(; aux != NULL; )
    {
        q = aux;
        aux = aux->urm;
        f(q->info);
        free(q);
        q = NULL;
    }
}

int InserareTop(TLSI *a, void *ae, TF f)
{
    int rez = 0, i;
    TLSI el = *a;
    Tserial *s = (Tserial *)ae;
    
    if(el != NULL)  //daca lista nu este vida
    {
        if(s->poz == 1) //inserare la inceput de lista
        {
            rez = InserareListaInaintePrim(a, ae);
            ReseteazaTop(a, f);
            return rez;
        }
        //se parcurge lista pana la pozitia de inserat
        for(i = 1; (i < s->poz - 1) && (el->urm != NULL); i++,el = el->urm);
        rez = InserareListaDupa(&el, ae);
        ReseteazaTop(a, f);
        return rez;
    }
    else    //lista este vida, se insereaza un singur element
    {
        rez = InserareListaVida(a, ae);
        ReseteazaTop(a, f);
        return rez;
    }
    ReseteazaTop(a, f);
    return rez;
}

//pozitia unui serial intr-o categorie data ca parametru (x)
int Pozitie(TH *a, void *ae, int x)
{
    int poz = 1;
    TLSI el = a->v[x];
    for(; el != NULL; el = el->urm) //parcurgerea listei
    {
        //daca se gaseste serialul
        if(strcmp(((Tserial *)el->info)->nume, ((Tserial *)ae)->nume) == 0)
            return poz;
        else
            poz++;
    }
    return 0;
}

//functie ce adauga un sezon nou unui serial
int add_sez(void *p, int nr_ep, int *v)
{
    int j;
    Tserial *a = (Tserial *)p;
    a->nr_sez++;
                
    //alocarea unei structuri de tip Tsezon
	Tsezon *aux = (Tsezon *)malloc(sizeof(Tsezon));
	if(!aux) return 0;	    //testul alocarii
		
	aux->nr_ep = nr_ep;           //se retine numarul de episoade
	aux->ep = InitQ(sizeof(int)); //initializarea cozii de episoade
	if(!aux->ep) return 0;        //testul initializarii

    for(j = 0; j < nr_ep; j++)
    {
        a->nevizionat += v[j];
        InsQ(aux->ep, &v[j]);   //inserarea in coada se episoade
    }
    InsQ(a->serial, aux);   //inserarea in coada de sezoane
    free(aux);              //eliberarea memoriei
    return 1;
}

//adaugarea unui sezon la un serial din tabela
int Ad_sezon_tab(TH *h, char *nume, int nr_ep, int *v)
{
    int i;
    TLSI p;
    for(i = 1; i < h->M; i++)   //parcurgerea tabelei
    {
        for(p = h->v[i]; p != NULL; p = p->urm)  //se parcurge fiecare lista
        {
            if(strcmp(nume, ((Tserial *)p->info)->nume) == 0)
            {
                //daca se gaseste serialul
                add_sez(p->info, nr_ep, v); //se adauga sezonul
                return 1;
            }		
        }
    }
    return 0;
}

//adaugarea unui sezon la un serial din coada c (watch_later)
int Ad_sezon_watch(void *c, char *nume, int nr_ep, int *v)
{
    void *el, *q;
    int ok = 0;            //retine daca a fost adaugat sezonul
    q = InitQ(sizeof(Tserial)); //initializarea cozii auxiliare
    if(!q)  return 0;           //testul alocarii
    el = malloc(DIMEC(c));      //elementul auxiliar
    if(!el)                     //testul alocarii
    {
        DistrQ(&q);
        return 0;
    }
    while(!VIDAC(c))    //cat timp nu e vida coada
    {
        ExtrQ(c, el);   //se extrage element
        if(strcmp(nume, ((Tserial *)el)->nume) == 0)  //daca au acelasi nume
        {
            add_sez(el, nr_ep, v);   //se adauga sezonul
            ok++;                    //a fost adaugat serialul
        }
        InsQ(q, el);      //se insereaza in q
    }
    while(!VIDAC(q))      //se reface coada initiala
    {
        ExtrQ(q, el);
        InsQ(c, el);
    }
    DistrQ(&q);           //se elibereaza memoria
    free(el);
    if(ok)  return 1;     //daca a fost realizata inserarea 
    return 0;
}

//adaugarea unui sezon la un serial din stiva s (currently_watching)
int Ad_sez_current(void *s, char * nume, int nr_ep, int *v)
{
    void *el = malloc(DIMES(s));    //elementul auxiliar
    if(!el) return 0;               //testul alocarii
    int ok = 0;     //retine daca a fost adaugat sezonul
    void *s_aux = InitS(DIMES(s));  //stiva auxiliara
    if(!s_aux)                      //testul alocarii
    {
        free(el);
        return 0;
    }
    while(!VIDA(s)) //cat timp nu e vida stiva
    {
        Pop(s, el); //se extrage elementul
        //daca este serialul cautat
        if(strcmp(nume, ((Tserial *)el)->nume) == 0)
        {
            add_sez(el, nr_ep, v);  //adauga sezonul
            ok++;           //a fost adaugat sezonul
        }
        Push(s_aux, el);
    }
    while(!VIDA(s_aux)) //se reface stiva initiala
    {
        Pop(s_aux, el);
        Push(s, el);
    }
    DistrS(&s_aux);         //se elibereaza memoria
    if(el)  free(el);
    if(ok)  return 1;       //daca a fost adaugat sezonul
    return 0;
}

//cauta un serial in tabela si intoarce celula de lista
TLSI cauta_serial(TH *h, char *nume, TF f)
{
    int i;
    TLSI p, ant;
    for(i = 1; i < h->M; i++)   //parcurgerea tabelei
    {
        //parcurgerea listei
        for(p = h->v[i], ant = h->v[i]; p != NULL; p = p->urm)
        {
            if(strcmp(nume, ((Tserial *)p->info)->nume) == 0)
            {
                //daca este un singur element in lista
                if((p == h->v[i]) && (p->urm == NULL))
                    h->v[i] = NULL;
                else if(p == h->v[i])   //primul element
                    h->v[i] = p->urm;
                else    //un element diferit de primul
                    ant->urm = p->urm;
                //daca este in top serialul cautat
                if(h->M == 4)   ReseteazaTop(&h->v[i], f);
                return p;
            }
            if(p != h->v[i])    //avansul elementului anterior
                ant = ant->urm;
        }
    }
    return NULL;
}

//pozitia ultimului element inserat intr-o coada
int pozitie_coada(void *c)
{
    void *q = InitQ(sizeof(Tserial));   //coada auxiliara
    if(!q)  return 0;                   //testul alocarii
    void *el = malloc(DIMEC(c));   //elementul auxiliar
    if(!el)                         //testul alocarii
    {
        DistrQ(&q);
        return 0;
    }
    int ct = 0;
    while(!VIDAC(c))    //cat timp nu e vida coada
    {
        ExtrQ(c, el);   //se extrage element
        ct++;
        InsQ(q, el);     //se insereaza in q
    }
    while(!VIDAC(q))     //se reface coada initiala
    {
        ExtrQ(q, el);
        InsQ(c, el);
    }
    DistrQ(&q);     //se elibereaza memoria
    free(el);
    return ct;
}

//verifica daca un serial exista intr-o stiva s (currently_watching)
//daca exista, se vizioneaza numarul de minute din serial
//daca a fost vizionat integral, serialul se muta in stiva h (history)
int exista_stiva(void *s, void *h, char *nume, int min, FILE *f)
{
    void *el = malloc(DIMES(s));    //element auxiliar
    if(!el) return 0;               //testul alocarii
    void *aux = malloc(DIMES(s));   //element auxiliar
    if(!aux)                        //testul alocarii
    {
        free(el);
        return 0;
    }       
    void *s_aux = InitS(DIMES(s));  //stiva auxiliara
    if(!s_aux)                      //testul alocarii
    {
        free(el);
        free(aux);
        return 0;
    }

    int ok, k, a = 0, rez = 0;
    while(!VIDA(s))    //daca stiva e nevida
    {
        k = 0;  //retine daca un serial a fost adaugat in history
        ok = 0; //retine daca un serial a fost gaist si vizionat
        Pop(s, el);   //se extrage element
        if(strcmp(nume, ((Tserial *)el)->nume) == 0)
        {
            ok++;
            rez++;  //retine daca a fost gasit serialul cautat
            ((Tserial *)el)->nevizionat -= min;
            //daca este vizionat integral serialul
            if(((Tserial *)el)->nevizionat <= 0)
            {
                fprintf(f, "Serialul %s a fost vizionat integral.\n", nume);
                Push(h, el);    //se adauga in stiva history
                k++;
            }
            if(k == 0)  //daca nu a fost adaugat in history
            {
                memcpy(aux, el, DIMES(s));  //retin serialul
                a++;
            }
        }
        //daca nu a fost gasit serialul
        if(ok == 0)  Push(s_aux, el); //se insereaza in stiva auxiliara
    }
    while(!VIDA(s_aux))     //se reface stiva initiala
    {
        Pop(s_aux, el);
        Push(s, el);
    }
    if(a)   Push(s, aux);   //se adauga in varful stivei serialul
    DistrS(&s_aux);         //se elibereaza memoria
    free(el);
    free(aux);
    if(rez)  return 1;  //daca a fost gasit serialul cautat
    return 0;
}

//se cauta un serial in coada c si daca se gaseste, 
//se insereaza in stiva primita ca parametru s
int exista_coada(void *c, void *s, char *nume, int min, FILE *f)
{
    void *q = InitQ(sizeof(Tserial));   //coada auxiliara
    if(!q)  return 0;                   //testul alocarii
    void *el = malloc(DIMEC(c));        //elementul auxiliar
    if(!el)                             //testul alocarii
    {
        DistrQ(&q);
        return 0;
    }
    int ok = 0;         //retine daca a fost gasit serialul
    while(!VIDAC(c))    //daca nu e vida coada
    {
        ok = 0;
        ExtrQ(c, el);   //se extrage element
        if(strcmp(nume, ((Tserial *)el)->nume) == 0)
        {
            ok++;
            Push(s, el);
        }
        //daca nu a fost gasit serialul
        if(!ok)     InsQ(q, el);        //se insereaza in q
    }
    while(!VIDAC(q))    //se reface coada initiala
    {
        ExtrQ(q, el);
        InsQ(c, el);
    }
    DistrQ(&q);             //eliberarea memoriei
    free(el);
    if(ok)  return 1;   //daca a fost gasit elementul dorit
    return 0;
}