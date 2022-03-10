/* DIACONU Arina-Ioana */

/*   main.c   */

#include <stdio.h>
#include <stdlib.h>
#include "ListeGenerice.h"
#include "Functii_seriale.h"
#include "Coada_lg.h"
#include "Stiva_lg.h"
#include <string.h>

#define l 35		//lungime nume serial
#define P 1000

//afisarea unui serial si a ratingului sau
void afisare(void *element, FILE *f)
{
	Tserial *s = (Tserial *)element;
	fprintf(f, "(%s, %.1f)", s->nume, s->rating);
}

//eliberarea memoriei pentru o structura de tip Tserial
void f_eliberare(void *a)
{
	if(a != NULL)	//daca elementul primit nu este vid
	{
		Tserial *s = (Tserial *)a;
		Tsezon *sez = (Tsezon *)malloc(sizeof(Tsezon));
		if(!sez)	return;		//testul alocarii
		
		while(!VIDAC(s->serial))	
		{
			ExtrQ(s->serial, sez);
			DistrQ(&sez->ep);	//eliberarea cozii de episoade
		}
		DistrQ(&s->serial);		//eliberarea cozii de sezone
		if(s->nume)
			free(s->nume);
		if(s)
			free(s);
		s = NULL;
		free(sez);				//eliberarea memoriei
	}
}

TH* GenerareTCategorii(size_t M)	//generarea tabelei de categorii
{
    TH *h = NULL;
	
	h = (TH *) InitializareTabela(M);	//initializare tabela
	if(h == NULL)					 	//testarea initializarii
		return NULL;

	return h;
}

//constructia cozii de sezoane, ce contine coada de episoade
Tserial* RetineCozi(void *a)
{
	Tserial *s = (Tserial *)a;
	if(s == NULL)	return NULL;
	//se aloca memorie pentru un sezon
	Tsezon *aux = (Tsezon *)malloc(sizeof(Tsezon));
	if(!aux)		//testul alocarii
	{
		f_eliberare(s);
		return NULL;
	}
	//se initializeaza coada de sezoane
	s->serial = InitQ(sizeof(Tsezon));
	if(!s->serial)		//testul alocarii
	{
		f_eliberare(s);
		return NULL;
	}
	int i, j;
	for(i = 0; i < s->nr_sez; i++)
	{
		char *nr_ep = strtok(NULL, " ");
		aux->nr_ep = atoi(nr_ep);
		//se initializeaza coada de episoade
		aux->ep = InitQ(sizeof(int));
		if(!aux->ep)		//testul alocarii
		{
			f_eliberare(s);
			return NULL;
		}
		//se retine durata episoadelor
		for(j = 0; j < aux->nr_ep; j++)
		{
			char *ep = strtok(NULL, " ");
			if (ep[strlen(ep) - 1] == '\n')
				ep[strlen(ep) - 1] = '\0';
			int episod = atoi(ep);

			s->nevizionat += episod;
			InsQ(aux->ep, &episod);	//inserarea in coada de episoade
		}
		InsQ(s->serial, aux);	//inserarea in coada de sezoane
	}
	free(aux);					//eliberarea memoriei
	return s;
}

Tserial* RetineDate(void *a)	//constructia unei structuri de tip Tserial
{
	char *lista = strtok(a, " ");
	char *nume = strtok(a, " ");
	char *rating = strtok(a, " ");
	char *nr_sez = strtok(a, " ");

	Tserial *s = (Tserial *)malloc(sizeof(Tserial));
	if(s == NULL)	return NULL;	//testul alocarii

	s->id = atoi(lista);
	s->nume = (char *)malloc(l);
	if(!s->nume)					//testul alocarii
	{
		free(s);
		return NULL;
	}
	strcpy(s->nume, nume);	

	s->rating = atof(rating);
	s->nevizionat = 0;
	s->poz = 0;
	s->nr_sez = atoi(nr_sez);
	s = RetineCozi((void *)s);
	return s;
}

Tserial* RetineDateTop(void *a)		//retinerea datelor din top10
{
	char *poz = strtok(a, " ");
	char *nume = strtok(a, " ");
	char *rating = strtok(a, " ");
	char *nr_sez = strtok(a, " ");

	Tserial *s = (Tserial *)malloc(sizeof(Tserial));
	if(s == NULL)	return NULL;	//testul alocarii

	s->id = 4;
	s->nume = (char *)malloc(l);
	if(!s->nume)					//testul alocarii
	{
		free(s);
		return NULL;
	}
	strcpy(s->nume, nume);	

	s->rating = atof(rating);
	s->nevizionat = 0;
	s->poz = atoi(poz);				//pozitia in top
	s->nr_sez = atoi(nr_sez);
	s = RetineCozi((void *)s);
	return s;
}

int main(int argc, char **argv)
{    
    size_t m = 5, len = 0;		//m - numarul de categorii de seriale
	TH *tabela = NULL;
	tabela = GenerareTCategorii(m);		  //se genereaza tabela
	if (tabela == NULL)		 return 1;	  //testul generarii tabelei

	FILE *file_in, *file_out;
	char *line = NULL, *numeFisier_in = argv[1], *numeFisier_out = argv[2];

	//se deschide fisierul ce contine datele de intrare
	file_in = fopen(numeFisier_in, "rt");
	if (file_in == NULL)	//testul deschiderii
	{
		DistrugeTabela(&tabela, f_eliberare);
		return 1;
	}

	//se deschide fisierul ce contine datele de iesire
	file_out = fopen(numeFisier_out, "w");
	if (file_out == NULL)	//testul deschiderii
	{
		DistrugeTabela(&tabela, f_eliberare);
		fclose(file_in);
		return 1;
	}

	void *watch_later = InitQ(sizeof(Tserial));			//coada watch_later
	void *currently_watching = InitS(sizeof(Tserial));	//stiva
	void *history = InitS(sizeof(Tserial));				//stiva

	TLSI *p, *q;	//vor retine serialele mutate pentru a elibera memoria
	int cp = 0, i, cq = 0;
	p = calloc(P, sizeof(TLSI));
	if(!p)		//testul alocarii
	{
		DistrugeTabela(&tabela, f_eliberare);
		fclose(file_in);
		fclose(file_out);
		return 1;
	}
	q = calloc(P, sizeof(TLSI));
	if(!q)		//testul alocarii
	{
		DistrugeTabela(&tabela, f_eliberare);
		fclose(file_in);
		fclose(file_out);
		free(p);
		return 1;
	}

	//se citeste linie cu linie din fisierul cu datele de intrare
	while ( getline(&line, &len, file_in) != -1 )
	{
		/* se retine primul cuvant, ce da si comanda 
		pentru prelucrarea serialelor */
		char *comanda = strtok(line, " ");		
		if(strcmp(comanda, "add") == 0)		//se adauga un serial
		{
			//se retin datele despre serial
			Tserial *s = RetineDate(NULL);
			int rez = InserareTabela(tabela, (void *)s, s->id);
			if(!rez)	//daca inserarea a esuat, se elibereaza memoria
			{
				f_eliberare((void *)s);
				return 1;
			}
			int poz = Pozitie(tabela, (void *)s, s->id);
			fprintf(file_out, "Serialul %s a fost adaugat la pozitia %d.\n",
			s->nume, poz);
		}
		else if(strcmp(comanda, "show") == 0)	//afisarea unei categorii
		{
			char *x = strtok(NULL, " ");
			if(x[strlen(x) - 1] == '\n') 	x[strlen(x) - 1] = '\0';
			
			if((atoi(x) == 1) || (atoi(x) == 2) || (atoi(x) == 3))
			{
				fprintf(file_out, "Categoria %d: ", atoi(x));
				AfisareCategorie(tabela, atoi(x), afisare, file_out);
			}
			else if(strcmp(x, "top10") == 0)
			{
				fprintf(file_out, "Categoria %s: ", x);
				AfisareCategorie(tabela, 4, afisare, file_out);
			}
			else if(strcmp(x, "later") == 0)
			{
				fprintf(file_out, "Categoria %s: ", x);
				Afisare_Coada(watch_later, afisare, file_out);
			}
			else if(strcmp(x, "watching") == 0)
			{
				fprintf(file_out, "Categoria %s: ", x);
				Afisare_Stiva(currently_watching, afisare, file_out);
			}
			else if(strcmp(x, "history") == 0)
			{
				fprintf(file_out, "Categoria %s: ", x);
				Afisare_Stiva(history, afisare, file_out);
			}
		}
		else if(strcmp(comanda, "add_sez") == 0)	//adaugare sezon nou
		{
			int nr_ep, *v, i;
			char *nume = strtok(NULL, " ");
			char *nr = strtok(NULL, " ");
			nr_ep = atoi(nr);
			//se aloca vectorul in care se retin episoadele la citire
			v = malloc(nr_ep * sizeof(int));
			if(!v)	return 1;	//testul alocarii
			for(i = 0; i < nr_ep; i++)
			{
				char *x = strtok(NULL, " ");
				if(x[strlen(x) - 1] == '\n')	x[strlen(x) - 1] = '\0';
				v[i] = atoi(x);
			}
			//se incearca gasirea serialului in tabela
			int rez = Ad_sezon_tab(tabela, nume, nr_ep, v);
			if(rez)		//daca a fost gasit
				fprintf(file_out, "Serialul %s are un sezon nou.\n", nume);
			else	//se incearca gasirea serialului in coada watch later
			{
				rez = Ad_sezon_watch(watch_later, nume, nr_ep, v);
				if(rez)		//daca a fost gasit
				{
					fprintf(file_out, "Serialul %s are un sezon nou.\n",
					nume);
				}
				else	//se incearca gasirea serialului in stiva currently
				{
					rez = Ad_sez_current(currently_watching, nume, nr_ep, v);
					if(rez)		//daca a fost gasit
					{
						fprintf(file_out, 
						"Serialul %s are un sezon nou.\n", nume);
					}
				}
			}
			free(v);	//eliberarea memoriei
		}
		//mutarea unui serial in coada watch_later
		else if(strcmp(comanda, "later") == 0)	
		{
			char *nume = strtok(NULL, " ");
			if(nume[strlen(nume) - 1]=='\n')  nume[strlen(nume) - 1] = '\0';
			//se retine serialul ce trebuie mutat
			p[cp] = cauta_serial(tabela, nume, f_eliberare);
			InsQ(watch_later, p[cp++]->info);		//inserarea in coada
			int poz = pozitie_coada(watch_later);
			fprintf(file_out,
			"Serialul %s se afla in coada de asteptare pe pozitia %d.\n",
			nume, poz);
		}
		else if(strcmp(comanda, "watch") == 0)	//vizionarea unui serial
		{
			char *nume = strtok(NULL, " ");
			char *durata = strtok(NULL, " ");
			if(durata[strlen(durata) - 1] == '\n')
				durata[strlen(durata) - 1] = '\0';
			int min = atoi(durata);
			//se cauta serialul in stiva currently_watching
			int rez = exista_stiva(currently_watching, history, nume,
			min, file_out);
			if(!rez)	//daca serialul de vizionat nu a fost gasit in stiva
			{
				//se cauta serialul in coada watch_later
				rez = exista_coada(watch_later, currently_watching,
				nume, min, file_out);
				//se vizioneaza serialul mutat in stiva currently_watching
				rez = exista_stiva(currently_watching, history, nume,
				min, file_out);
			}
			if(!rez)	//daca serialul de vizionat nu a fost gasit in coada
			{
				//se cauta serialul in tabela
				q[cq] = cauta_serial(tabela, nume, f_eliberare);
				Push(currently_watching, q[cq++]->info);	//inserarea in stiva
				//se vizioneaza serialul mutat in stiva currently_watching
				rez = exista_stiva(currently_watching, history, nume,
				min, file_out);
			}
		}
		else if(strcmp(comanda, "add_top") == 0) //se adauga un serial in top
		{
			//se retin datele despre serial
			Tserial *s = RetineDateTop(NULL);
			int rez = InserareTop(&tabela->v[4], (void *)s, f_eliberare);
			if(!rez)	//daca inserarea a esuat, se elibereaza memoria
				f_eliberare((void *)s);
			
			fprintf(file_out, "Categoria top10: ");
			AfisareCategorie(tabela, 4, afisare, file_out);
		}
	}
	
	free(line);		//eliberarea memoriei
	DistrugeTabela(&tabela, f_eliberare); //eliberarea memoriei pentru tabela
	DistrQ(&watch_later);
	DistrS(&currently_watching);
	DistrS(&history);

	for(i = 0; i < cp; i++)
	{
		if(p[i])
		{
			if(p[i]->info)	f_eliberare(p[i]->info);
			free(p[i]);
		}
	}
	free(p);
	for(i = 0; i < cq; i++)
	{
		if(q[i])
		{
			if(q[i]->info)	f_eliberare(q[i]->info);
			free(q[i]);
		}
	}
	free(q);

	fclose(file_in);	//inchiderea fisierelor
	fclose(file_out);	
    return 0;
}
