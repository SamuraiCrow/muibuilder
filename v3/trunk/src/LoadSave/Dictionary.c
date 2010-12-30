/***************************************************************************

  MUIBuilder - MUI interface builder
  Copyright (C) 1990-2009 by Eric Totel
  Copyright (C) 2010-2011 by MUIBuilder Open Source Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  MUIBuilder Support Site: http://sourceforge.net/projects/muibuilder/

  $Id$

***************************************************************************/

#include "Dictionary.h"
#include <stdlib.h>

// internal function to change the size of the dictionary, newSize must be a power of 2
int rehashRefDictionary(RefDictionary *dictionary,unsigned long newSize);

RefDictionary *newRefDictionary(unsigned long initialSize)
{
	RefDictionary *newDictionary;
	unsigned long i;

	if (newDictionary=(RefDictionary *)malloc(sizeof(RefDictionary))) {
		newDictionary->size=1<<4;
		newDictionary->count=0;
		while (newDictionary->size<initialSize) {
			newDictionary->size<<=1;
		}
		if (newDictionary->table=(RefHashEntry **)malloc(newDictionary->size*sizeof(RefHashEntry *))) {
			for(i=0;i<newDictionary->size;i++) {
				newDictionary->table[i]=0L;
			}
		} else {
			free(newDictionary);
			newDictionary=0L;
		}
	}

	return newDictionary;
}

void disposeRefDictionary(RefDictionary *dictionary)
{
	unsigned long i;
	RefHashEntry *nextEntry,*entry;

	for(i=0;i<dictionary->size;i++) {
		entry=dictionary->table[i];
		while (entry) {
			nextEntry=entry->nextEntry;
			free(entry);
			entry=nextEntry;
		}
	}
	free(dictionary->table);
	free(dictionary);
}

void *findAssociationInRefDictionary(RefDictionary *dictionary,void *key)
{
	unsigned long hashValue;
	RefHashEntry *entry;

	hashValue=(((unsigned long)(key))>>3)&(dictionary->size-1);
	entry=dictionary->table[hashValue];
	while (entry) {
		if (entry->key==key) return entry->association;
		entry=entry->nextEntry;
	}
	return (void *)0L;
}

int setAssociationForKeyInRefDictionary(RefDictionary *dictionary,void *key,void *association)
{
	unsigned long hashValue;
	RefHashEntry *nextEntry,*entry,*newEntry;

	hashValue=(((unsigned long)(key))>>3)&(dictionary->size-1);
	nextEntry=dictionary->table[hashValue];

	while (nextEntry) {
		entry=nextEntry;
		if (entry->key==key) {
			entry->association=association;
			return 1;
		}
		nextEntry=entry->nextEntry;
	}

	if (newEntry=((RefHashEntry *)malloc(sizeof(RefHashEntry)))) {
		newEntry->key=key;
		newEntry->association=association;
		newEntry->nextEntry=dictionary->table[hashValue];
		nextEntry=dictionary->table[hashValue]=newEntry;

		dictionary->count++;
		if (dictionary->count>(dictionary->size*2)) {
			rehashRefDictionary(dictionary,dictionary->size*2);
		}

		return 1;
	}
	return 0;
}

int removeKeyFromRefDictionary(RefDictionary *dictionary,void *key)
{
	unsigned long hashValue;
	RefHashEntry *nextEntry,*entry;

	hashValue=(((unsigned long)(key))>>3)&(dictionary->size-1);
	entry=dictionary->table[hashValue];
	if (entry) {
		nextEntry=entry->nextEntry;
		if (entry->key==key) {
			dictionary->table[hashValue]=nextEntry;
			free(entry);
			return 1;
		}

		while (nextEntry) {
			if (nextEntry->key==key) {
				entry->nextEntry=nextEntry->nextEntry;
				free(nextEntry);

				dictionary->count--;
				if (dictionary->count<(dictionary->size/2)) {
					rehashRefDictionary(dictionary,dictionary->size/2);
				}

				return 1;
			}
			entry=nextEntry;
			nextEntry=entry->nextEntry;

		}
	}
	return 0;
}

int rehashRefDictionary(RefDictionary *dictionary,unsigned long newSize)
{
	RefHashEntry **newTable;
	RefHashEntry *nextEntry,*entry;
	unsigned long i,hashValue;

	if (newTable=(RefHashEntry **)malloc(newSize*sizeof(RefHashEntry *))) {
		for(i=0;i<newSize;i++) {
			newTable[i]=0L;
		}
		for(i=0;i<dictionary->size;i++) {
			entry=dictionary->table[i];
			while (entry) {
				nextEntry=entry->nextEntry;
				hashValue=(((unsigned long)(entry->key))>>3)&(newSize-1);
				entry->nextEntry=newTable[hashValue];
				newTable[hashValue]=entry;
				entry=nextEntry;
			}
		}
		free (dictionary->table);
		dictionary->table=newTable;
		dictionary->size=newSize;
		return 1;
	} else {
		return 0;
	}
}

