#include <dos/dos.h>
#include <clib/alib_protos.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/intuition_protos.h>
#include <clib/utility_protos.h>
#include <clib/muimaster_protos.h>

#include <pragmas/exec_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/intuition_pragmas.h>
#include <pragmas/utility_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include <exec/memory.h>

#include <stdio.h>
#include <string.h>

#include "object.h"

struct Library *IntuitionBase = NULL;

int main(void)
{
  struct Library * BaseObject;
  APTR   obj;
  char   *mystring = NULL;

  IntuitionBase = OpenLibrary("intuition.library", 0);
  BaseObject = OpenLibrary("Object.boopsi", 0);

  if (BaseObject) printf("Library opened.\n");

  if(obj = NewObject(NULL, "baseobject", TAG_DONE))
  {
    printf("Object created \n");
  }

  mystring = AllocVec(strlen("Essai")+1, MEMF_PUBLIC|MEMF_CLEAR);
  strcpy(mystring, "Essai");

  SetAttrs(obj, MBA_Object_Label, mystring, TAG_DONE);
  printf("Set réalisé\n");
  GetAttr(MBA_Object_Label, obj, (ULONG*) &mystring);

  printf("Label = %s\n", mystring);

  if (obj) DisposeObject(obj);
  if (BaseObject) CloseLibrary(BaseObject);
  if (IntuitionBase) CloseLibrary(IntuitionBase);
  return(0);
}
