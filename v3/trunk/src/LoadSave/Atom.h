#ifndef ATOM_H
#define ATOM_H

typedef struct _AtomDictionary AtomDictionary; //A dictionary where keys are atoms
typedef struct _AtomHashEntry AtomHashEntry;
typedef const char * Atom;

struct _AtomDictionary {
    unsigned long size;
    unsigned long count;
    AtomHashEntry **table;
};

struct _AtomHashEntry {
    unsigned long hashValue;
    AtomHashEntry *nextEntry;
    char *atom;
};

AtomDictionary *newAtomDictionary(unsigned long initialSize); // 0 means default
void disposeAtomDictionary(AtomDictionary *dictionary);
Atom checkAtomInDictionary(AtomDictionary *dictionary,char *key);
Atom getAtomInDictionary(AtomDictionary *dictionary,char *key);
unsigned long hashString(char *string);
#define stringForAtom(a) ((const char *)a)

#endif
