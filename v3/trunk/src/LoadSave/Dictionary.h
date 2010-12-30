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
