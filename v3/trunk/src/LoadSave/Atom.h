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
