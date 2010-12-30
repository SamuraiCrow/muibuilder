#ifndef DICTIONARY_H
#define DICTIONARY_H

typedef struct _RefDictionary RefDictionary; // A dictionary where keys are pointers
typedef struct _RefHashEntry RefHashEntry;

struct _RefDictionary {
    unsigned long size;
    unsigned long count;
    RefHashEntry **table;
};

struct _RefHashEntry {
    void *key;
    void *association;
    RefHashEntry *nextEntry;
};

RefDictionary *newRefDictionary(unsigned long initialSize); // 0 means default
void disposeRefDictionary(RefDictionary *dictionary);
void *findAssociationInRefDictionary(RefDictionary *dictionary,void *key);
int setAssociationForKeyInRefDictionary(RefDictionary *dictionary,void *key,void *association);
int removeKeyFromRefDictionary(RefDictionary *dictionary,void *key);
#endif
