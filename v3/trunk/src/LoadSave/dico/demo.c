#include "Atom.h"
#include "Dictionary.h"

AtomDictionary *globalAtomDictionary;
#define atomForString(s) (getAtomInDictionary(globalAtomDictionary,(s)))

void init()
{
    globalAtomDictionary=newAtomDictionary(0);
}

void dealloc()
{
    disposeAtomDictionary(globalAtomDictionary);
}

int main()
{
    init();
    {
        Atom un,deux,trois,enteredAtom;
        RefDictionary *aDic, *anotherDic;
        char *input="deux"; // me souvient plus des fget... pour ce soir :°)


        // initialize some dicitonaries
        aDic=newRefDictionary(3); // We suppose that the dictionary will start with about 3 associations
                                  // Of course the actual number of association can be higher, lower, or
                                  // greatly varies accross the program execution.
        anotherDic=newRefDictionary(0); // use the default

        // precalculate some widely used Atoms
        un=atomForString("un");
        deux=atomForString("deux");
        trois=atomForString("trois");
        // to compare two atomized string, just compare the atoms
        // atoms can be used to reduce memory usage, as the string is stored only once

        // fill the dictionaries in (they can be modified or consulted at any moment, in any order of course)
        setAssociationForKeyInRefDictionary(aDic,un,"One"); // anythings can be stored in a dictionary
                                                            // not only strings, the size must be those of
                                                            // void *
                                                            // the same remark applies for the keys, but
                                                            // the hashing function works well only for
                                                            // pointers
        setAssociationForKeyInRefDictionary(aDic,deux,"Too");
        setAssociationForKeyInRefDictionary(aDic,deux,"Two"); // oups, let's correct myt mistake :)
        setAssociationForKeyInRefDictionary(aDic,trois,"Three");
        setAssociationForKeyInRefDictionary(anotherDic,un,"ieden");
        setAssociationForKeyInRefDictionary(anotherDic,deux,"dwa");

        enteredAtom=atomForString(input); // this can not be preculated (we don't know what the user would
                                          //                                have typed)

        // try once
        printf("Traduction de %s : %s, %s",input,findAssociationInRefDictionary(aDic,enteredAtom),
                    findAssociationInRefDictionary(anotherDic,enteredAtom));
                    // find returns nil if no matched key is found

        // end
        disposeRefDictionary(aDic);
        disposeRefDictionary(anotherDic);
    }

    dealloc();
}

