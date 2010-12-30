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
  BOOL	hide;
  BOOL	disable;
  BOOL	inputMode;
  BOOL	phantom;
  int	weight;
  int	background;
  int	frame;
  char	key;
  char	*titleframe; 
};

SAVEDS ULONG mSet(Class *cl, Object *obj, struct opSet * msg)
{
  struct ObjData *data;
  struct TagItem *ti, *tstate;
  APTR   retval = NULL;

  data = INST_DATA(cl, obj);
  retval = DoSuperMethodA(cl, obj, (Msg)msg);
  ti = msg->ops_AttrList;
  tstate = ti;
  while(ti = NextTagItem(&tstate))
  {
    switch(ti->ti_Tag)
    {
    case MBA_Area_Hide:
      data->hide = (BOOL*)ti->ti_Data;
      break;
    case MBA_Area_Disable:
      data->disable = (BOOL*)ti->ti_Data;
      break;
    case MBA_Area_InputMode:
      data->inputmode = (ULONG*)ti->ti_Data;
      break;
    case MBA_Area_Phantom:
      data->phantom = (BOOL*)ti->ti_Data;
      break;
    case MBA_Area_Background:
      data->background = (ULONG*)ti->ti_Data;
      break;
    case MBA_Area_Frame:
      data->frame = (ULONG*)ti->ti_Data;
      break;
    case MBA_Area_Key:
      data->key = (ULONG*)ti->ti_Data;
      break;
    case MBA_Area_TitleFrame:
      data->titleframe = (ULONG*)ti->ti_Data;
      break;
    default:
      break;
    }
  }
  return((ULONG)retval);
}

SAVEDS ULONG mGet(Class *cl, Object *obj, struct opGet * msg)
{
  struct ObjData *data;
  APTR   retval = (APTR)TRUE;

  data = INST_DATA(cl, obj);
  switch(msg->opg_AttrID)
    {
    case MBA_Object_HelpBubble:
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
    }
  return((ULONG)retval);
}

ULONG mDispose(Class *cl, Object *obj, Msg msg)
{
  struct ObjData *data = INST_DATA(cl, obj);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ASM ULONG Dispatcher(REG(a0) struct IClass *cl,
			  REG(a2) Object *obj,
			  REG(a1) Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
    {
    case OM_NEW:    return(mNew(cl, obj, (struct opSet *)msg));
    case OM_SET:    return(mSet(cl, obj, (struct opSet *)msg));
    case OM_UPDATE: return(mSet(cl, obj, (struct opSet *)msg));
    case OM_GET:    return(mGet(cl, obj, (struct opGet *)msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }
  return((ULONG)retval);
}

__asm SAVEDS LONG __UserLibInit(register __a6 struct Library *mybase)
{
  LONG result = 0;
  SysBase = *((struct Library **)4);

  IntuitionBase = OpenLibrary("intuition.library",0);
  UtilityBase = OpenLibrary("utility.library",0);
  DOSBase = OpenLibrary("dos.library", 0);
  MUIMasterBase = OpenLibrary("dos.library", 0);

  if (IntuitionBase&&UtilityBase&&DOSBase&&MUIMasterBase) result = -1;

  if (ThisClass = MakeClass("MBArea", "MBBaseObject", NULL, sizeof(struct ObjData), 0))
    {
      ThisClass->cl_Dispatcher.h_Entry = (ULONG(*)())Dispatcher;
      ThisClass->cl_Dispatcher.h_SubEntry = NULL;
      AddClass(ThisClass);
    }
  else result = -1;

  if (result)
    {
      if (UtilityBase) CloseLibrary(UtilityBase);
      if (DOSBase) CloseLibrary(DOSBase);
      if (IntuitionBase) CloseLibrary(IntuitionBase);
      if (MUIMasterBase) CloseLibrary(MUIMasterBase);
    }

  return(result);
}

void __asm __UserLibCleanup(REG(a6) struct Library *mybase)
{
  RemoveClass(ThisClass);
  FreeClass(ThisClass);
  if (UtilityBase) CloseLibrary(UtilityBase);
  if (DOSBase) CloseLibrary(DOSBase);
  if (IntuitionBase) CloseLibrary(IntuitionBase);
}

struct IClass *MUIB_Class(void)
{
  return(ThisClass);
}
