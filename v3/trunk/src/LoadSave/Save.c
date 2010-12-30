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



