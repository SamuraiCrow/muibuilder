#include "Atom.h"
#include <stdlib.h>
#include <string.h>

// internal function to change the size of the dictionary, newSize must be a power of 2
int rehashAtomDictionary(AtomDictionary *dictionary,unsigned long newSize);

AtomDictionary *newAtomDictionary(unsigned long initialSize)
{
    AtomDictionary *newDictionary;
    unsigned long i;

    if (newDictionary=(AtomDictionary *)malloc(sizeof(AtomDictionary))) {
        newDictionary->size=1<<4;
        newDictionary->count=0;
        while (newDictionary->size<initialSize) {
            newDictionary->size<<=1;
        }
        if (newDictionary->table=(AtomHashEntry **)malloc(newDictionary->size*sizeof(AtomHashEntry *))) {
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

void disposeAtomDictionary(AtomDictionary *dictionary)
{
    unsigned long i;
    AtomHashEntry *nextEntry,*entry;

    for(i=0;i<dictionary->size;i++) {
        entry=dictionary->table[i];
        while (entry) {
            nextEntry=entry->nextEntry;
            free(entry->atom);
            free(entry);
            entry=nextEntry;
        }
    }
    free(dictionary->table);
    free(dictionary);
}

Atom checkAtomInDictionary(AtomDictionary *dictionary,char *key)
{
    unsigned long hashValue,hashValue2;
    AtomHashEntry *entry;

    hashValue=hashString(key);
    hashValue2=hashValue&(dictionary->size-1);

    entry=dictionary->table[hashValue2];
    while (entry) {
        if (entry->hashValue==hashValue) {
            if (!strcmp(key,entry->atom)) {
                return entry->atom;
            }
        }
        entry=entry->nextEntry;
    }
    return (void *)0L;
}

Atom getAtomInDictionary(AtomDictionary *dictionary,char *key)
{
    unsigned long hashValue,hashValue2;
    AtomHashEntry *entry,*lastEntry;

    hashValue=hashString(key);
    hashValue2=hashValue&(dictionary->size-1);

    lastEntry=0;
    entry=dictionary->table[hashValue2];
    while (entry) {
        if (entry->hashValue==hashValue) {
            if (!strcmp(key,entry->atom)) {
                return entry->atom;
            }
        }
        lastEntry=entry;
        entry=entry->nextEntry;
    }

    if (entry=((AtomHashEntry *)malloc(sizeof(AtomHashEntry)))) {
        if (entry->atom=malloc(strlen(key)+1)) {
            strcpy(entry->atom,key);
            entry->nextEntry=0;
            entry->hashValue=hashValue;
            if (lastEntry) {
                lastEntry->nextEntry=entry;
            } else {
                dictionary->table[hashValue2]=entry;
            }

            dictionary->count++;
            if (dictionary->count>(dictionary->size*2)) {
                rehashAtomDictionary(dictionary,dictionary->size*2);
            }

            return entry->atom;
        } else {
            free(entry);
        }
    }
    return (char *)0L;
}

unsigned long hashString(char *string)
{
    unsigned long hashValue=0;

    while (*string) {
        hashValue=(hashValue<<1)+*string;
        string ++;
    }
    return hashValue;
}

int rehashAtomDictionary(AtomDictionary *dictionary,unsigned long newSize)
{
    AtomHashEntry **newTable;
    AtomHashEntry *nextEntry,*entry;
    unsigned long i,hashValue;

    if (newTable=(AtomHashEntry **)malloc(newSize*sizeof(AtomHashEntry *))) {
        for(i=0;i<newSize;i++) {
            newTable[i]=0L;
        }
        for(i=0;i<dictionary->size;i++) {
            entry=dictionary->table[i];
            while (entry) {
                nextEntry=entry->nextEntry;
                hashValue=(entry->hashValue)&(newSize-1);
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
