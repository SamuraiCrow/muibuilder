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

#include <libraries/mui.h>
#include <exec/memory.h>

#include <stdio.h>
#include <string.h>

#include "/object.h"

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

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;

struct IClass *BaseClass;

struct BaseData
{
  Object *muiobj;
  char   *label;
  char   *help;
};

SAVEDS ULONG mSet(Class *cl, Object *obj, struct opSet * msg)
{
  struct BaseData *data = INST_DATA(cl, obj);
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
      if (data->label) FreeVec(data->label);
      data->label = (char*)ti->ti_Data;
      retval = 1L;
      break;
    case MBA_Object_MUIObject:
      data->muiobj = (Object*)ti->ti_Data;
      retval = 1L;
      break;
    case MBA_Object_Help:
      if (data->help) FreeVec(data->help);
      data->help = (char*)ti->ti_Data;
      retval = 1L;
      break;
    }
  }
  return(retval);
}

SAVEDS ULONG mGet(Class *cl, Object *obj, struct opGet * msg)
{
  struct BaseData *data;
  ULONG  retval = 0;

  data = INST_DATA(cl, obj);
  switch(msg->opg_AttrID)
    {
    case MBA_Object_Label:
      *(msg->opg_Storage) = (ULONG) data->label;
      retval = 1;
      break;
    case MBA_Object_MUIObject:
      *(msg->opg_Storage) = (ULONG) data->muiobj;
      retval = 1;
      break;
    case MBA_Object_Help:
      *(msg->opg_Storage) = (ULONG) data->help;
      retval = 1;
      break;
    case MBA_Object_Type:
      *(msg->opg_Storage) = (ULONG) MBV_Object_Type_Standard;
      retval = 1;
      break;
    default:
      retval = DoSuperMethodA(cl, obj, (Msg) msg);
      break;
    }
  return((ULONG)retval);
}

SAVEDS ULONG mNew(Class *cl, Object *obj, struct opSet *msg)
{
  struct BaseData *data = NULL;
  APTR retval = NULL;

  if (retval = (APTR)DoSuperMethodA(cl, obj, (Msg) msg))
    {
      data = INST_DATA(cl, retval);
      data->label = (char*)GetTagData(MBA_Object_Label, NULL, msg->ops_AttrList);
      data->muiobj = NULL;
      data->help = (char*)GetTagData(MBA_Object_Help, NULL, msg->ops_AttrList);
    }

  return((ULONG)retval);
}

ULONG mDispose(Class *cl, Object *obj, Msg msg)
{
  struct BaseData *data = INST_DATA(cl, obj);

  if (data->label) FreeVec(data->label);
  if (data->help)  FreeVec(data->help);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mAttributesEdit(Class *cl, Object *obj, struct MBP_AttributesEdit * msg)
{
  return(1);
}

SAVEDS ULONG mSaveAttributes(Class *cl, Object *obj, Msg msg)
{
  return(1);
}

SAVEDS ULONG Dispatcher(struct IClass *cl,
                            Object *obj,
                            Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:    return(mNew(cl, obj, (struct opSet *)msg));
    case OM_SET:    return(mSet(cl, obj, (struct opSet *)msg));
    case OM_GET:    return(mGet(cl, obj, (struct opGet *)msg));
    case OM_DISPOSE:return(mDispose(cl, obj, msg));
    case MBM_Object_AttributesEdit: return((ULONG)mAttributesEdit(cl, obj, (struct MBP_AttributesEdit *)msg));
    case MBM_Object_SaveAttributes: return((ULONG)mSaveAttributes(cl, obj, msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

SAVEDS void InitBaseClass(void)
{
  extern ULONG HookEntry();

  if (BaseClass = MakeClass("MBBaseClass", "modelclass", NULL, sizeof(struct BaseData), 0))
    {
      BaseClass->cl_Dispatcher.h_Entry = HookEntry;
      BaseClass->cl_Dispatcher.h_SubEntry = Dispatcher;
      AddClass(BaseClass);
    }
}

SAVEDS void DisposeBaseClass(void)
{
  RemoveClass(BaseClass);
  FreeClass(BaseClass);
}
