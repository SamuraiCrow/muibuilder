#include "queue.h"
#include <proto/exec.h>
#include <exec/memory.h>
#include <stdlib.h>
#include <stdio.h>

void up(queue *file, int n)
{
	exchange(file,n-1,n);
}

void down(queue *file, int n)
{
	exchange(file,n,n+1);
}

void exchange( queue * file, int m, int n)
{
	chainon *aux1,*aux2;
	APTR	elt_aux;
	int i;

	if ((m<file->nb_elements)&&(m>=0)&&(n<file->nb_elements)&&(n>=0))
	{
		aux1 = file->head;
		aux2 = file->head;
		for(i=0;i<m;i++) aux1 = aux1->next;
		for(i=0;i<n;i++) aux2 = aux2->next;
		elt_aux = aux1->element;
		aux1->element = aux2->element;
		aux2->element = elt_aux;
	}
}

void add(queue * file, APTR element)
{
	chainon *chainon_aux;

	chainon_aux = ( chainon * ) AllocVec(sizeof(chainon),MEMF_PUBLIC|MEMF_CLEAR);
	if ( chainon_aux == NULL ) return;
	chainon_aux->ident = 0;
	chainon_aux->element = element;
	if ((file->nb_elements)>0)
	{
		(file->tail)->next = chainon_aux;
	}
	else
	{
		file->head = chainon_aux;
	}
	file->tail = chainon_aux;
	chainon_aux->next = NULL;
	file->nb_elements = file->nb_elements + 1;
}

APTR nth(queue * file, int n)
{
	int i;
	chainon *chainon_aux;

	if (file == NULL)
	{
		printf("INTERNAL ERROR : LIST ERROR");
		return(NULL);
	}
	chainon_aux=file->head;
	if ((n<=file->nb_elements)&&(file->nb_elements>0))
	{
		for(i=0;i<n;i++) chainon_aux=chainon_aux->next;
		return(chainon_aux->element);
	}
	else return(NULL);
}

void delete_nth(queue * file, int n)
{
	int i;
	chainon *chainon_aux, *chainon_aux2;

	if ((file->nb_elements>0)&&(n<file->nb_elements))
	{
		if (n>0)
		{
			chainon_aux=file->head;
			for(i=0;i<n-1;i++) chainon_aux=chainon_aux->next;
			chainon_aux2 = chainon_aux->next;
			chainon_aux->next = chainon_aux2->next;
			FreeVec(chainon_aux2);
			if (n==(file->nb_elements-1))
			{
				file->tail =chainon_aux;
				file->tail->next = NULL;
			}
		}
		else
		{
			chainon_aux = file->head;
			file->head = file->head->next;
			FreeVec(chainon_aux);
		}
		file->nb_elements = file->nb_elements - 1;
	}
}
	
void delete_all(queue * file)
{
	int i,n;

	n=file->nb_elements;
	if (n>0)
	{
		for (i=0;i<n;i++)
		{
			delete_nth(file,0);
		}
	}
}

queue * create( void )
{
	queue * file_aux;

	file_aux = (queue *) AllocVec(sizeof(queue),MEMF_PUBLIC|MEMF_CLEAR);
	if (file_aux)
	{
		file_aux->head = NULL;
		file_aux->tail = NULL;
		file_aux->nb_elements = 0;
	}
	return(file_aux);
}
