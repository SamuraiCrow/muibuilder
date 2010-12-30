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

#include "/Object.h"

#if defined(_DCC)
#define REG(x) __ ## x
#define SAVEDS __geta4
#define ASM
#define REGARGS __regargs
#else
#if defined(__SASC)
#define REG(x) register __ ## x
#define SAVEDS __saveds
#define ASM __asm
#define REGARGS __regargs
#endif
#endif

struct Library *UtilityBase = NULL;
struct Library *IntuitionBase = NULL;
struct Library *DOSBase = NULL;
struct IClass *ThisClass;
struct Library *SysBase = NULL;
struct Library *MUIMasterBase = NULL;

struct ObjData
{
  Object *muiobj;
  char   *label;
  char   *help;
  char   *helpbubble;
};

SAVEDS ULONG mSet(Class *cl, Object *obj, struct opSet * msg)
{
  struct ObjData *data = INST_DATA(cl, obj);
  struct TagItem *ti, *tstate;
  ULONG  retval = 0;

  retval = DoSuperMethodA(cl, obj, (Msg)msg);
  ti = msg->ops_AttrList;
  tstate = ti;
  while(ti = NextTagItem(&tstate))
  {
    switch(ti->ti_Tag)
    {
    case MBA_Object_Label:
      KPrintF("Modif label\n");
      data->label = (char*)ti->ti_Data;
      retval = 1L;
      break;
    case MBA_Object_MUIObject:
      data->muiobj = (Object*)ti->ti_Data;
      retval = 1L;
      break;
    }
  }
  KPrintF("Fin mSet\n");
  return(retval);
}

SAVEDS ULONG mGet(Class *cl, Object *obj, struct opGet * msg)
{
  struct ObjData *data;
  APTR   retval = (APTR)TRUE;

  data = INST_DATA(cl, obj);
  switch(msg->opg_AttrID)
    {
    case MBA_Object_Label:
      *(msg->opg_Storage) = (ULONG) data->label;
      break;
    case MBA_Object_MUIObject:
      *(msg->opg_Storage) = (ULONG) data->muiobj;
      break;
    case MBA_Object_Help:
      *(msg->opg_Storage) = (ULONG) data->help;
      break;
    case MBA_Object_HelpBubble:
      *(msg->opg_Storage) = (ULONG) data->helpbubble;
      break;
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, (Msg) msg);
      break;
    }
  return((ULONG)retval);
}

SAVEDS ULONG mNew(Class *cl, Object *obj, struct opSet *msg)
{
  struct ObjData *data = NULL;
  APTR retval = NULL;

  if (retval = (APTR)DoSuperMethodA(cl, obj, (Msg) msg))
    {
      data = INST_DATA(cl, retval);
      data->label = "Essai_Label";
      data->muiobj = NULL;
      data->help = NULL;
      data->helpbubble = NULL;
    }
  return((ULONG)retval);
}

ULONG mDispose(Class *cl, Object *obj, Msg msg)
{
  struct ObjData *data = INST_DATA(cl, obj);

  if (data->muiobj)     MUI_DisposeObject(data->muiobj);
  if (data->help)       FreeVec(data->help);
  if (data->label)      FreeVec(data->label);
  if (data->helpbubble) FreeVec(data->helpbubble);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG Dispatcher(struct IClass *cl,
			    Object *obj,
			    Msg msg)
{
  APTR retval = NULL;

  KPrintF("Entrée Dispatcher\n");
  switch(msg->MethodID)
  {
    case OM_NEW:    return(mNew(cl, obj, (struct opSet *)msg));
    case OM_SET:    return(mSet(cl, obj, (struct opSet *)msg));
    case OM_GET:    return(mGet(cl, obj, (struct opGet *)msg));
    case OM_DISPOSE:return(mDispose(cl, obj, msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }
  KPrintF("Sortie Dispatcher\n");
  return((ULONG)retval);
}

ASM SAVEDS struct Library * __UserLibInit(REG(a6) struct Library *mybase)
{
  extern ULONG HookEntry();
  SysBase = *((struct Library **)4);

  IntuitionBase = OpenLibrary("intuition.library",0);
  UtilityBase = OpenLibrary("utility.library",0);
  DOSBase = OpenLibrary("dos.library", 0);
  MUIMasterBase = OpenLibrary("muimaster.library", 0);

  if (!(IntuitionBase&&UtilityBase&&DOSBase&&MUIMasterBase)) return(NULL);

  if (ThisClass = MakeClass("baseobject", "modelclass", NULL, sizeof(struct ObjData), 0))
    {
      ThisClass->cl_Dispatcher.h_Entry = HookEntry;
      ThisClass->cl_Dispatcher.h_SubEntry = Dispatcher;
      AddClass(ThisClass);
    }
  else return(NULL);

  return(mybase);
}

void SAVEDS ASM __UserLibCleanup(REG(a6) struct Library *mybase)
{
  RemoveClass(ThisClass);
  FreeClass(ThisClass);
  if (UtilityBase) CloseLibrary(UtilityBase);
  if (DOSBase) CloseLibrary(DOSBase);
  if (IntuitionBase) CloseLibrary(IntuitionBase);
}

SAVEDS struct IClass *MUIB_Class(void)
{
  return(ThisClass);
}
