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

#include <stdio.h>
#include <exec/types.h>

#include "AttributeTypes.h"

FILE *SaveFile = NULL;
int  tablevel;

void WriteTabs(void)
{
  int i;

  if (SaveFile)
    for(i=0;i<tablevel;i++) fprintf(SaveFile, "  ");
}

int MB_OpenSaveFile(char *filename)
{
  if (SaveFile = fopen(filename, "w+"))
    {
      tablevel = 0;
      fprintf(SaveFile, "MUIBUILDER_FILE\n");
      return(1);
    }

  return(0);
}

void MB_CloseSaveFile(void)
{
  if (SaveFile) fclose(SaveFile);
}

void MB_BeginSaveObject(char *type, char *name)
{
  if (!SaveFile) return;

  WriteTabs();
  fprintf(SaveFile, "Object %s %s\n", type, name);
  WriteTabs();
  fprintf(SaveFile, "{\n");
  tablevel++;
}

void MB_EndSaveObject(void)
{

  if (!SaveFile) return;

  tablevel--;
  WriteTabs();

  fprintf(SaveFile, "}\n\n");
}

void MB_SaveAttribute(char *attribute, ULONG type, ULONG value)
{
  if (!SaveFile) return;

  WriteTabs();
  fprintf(SaveFile, "%s = ", attribute);


  switch(type)
    {
    case TYPE_INT:
      fprintf(SaveFile, "%d;\n", value);
      break;
    case TYPE_BOOL:
      if (value) fprintf(SaveFile, "TRUE;\n");
      else       fprintf(SaveFile, "FALSE;\n");
      break;
    case TYPE_CHAR:
      if (value) fprintf(SaveFile, "%c;\n", (char)value);
      break;
    case TYPE_STRING:
      if (value) fprintf(SaveFile, "\"%s\";\n", (char*)value);
      else       fprintf(SaveFile, "\"\";\n", (char*)value);
      break;
    case TYPE_OBJ:
      if (value) fprintf(SaveFile, "%s;\n", (char*)value);
      break;
    }
}



