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

#define SUPERCLASS MUIC_Text

struct GUITextData
{
  Object *object;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;

struct MUI_CustomClass *GUITextClass;

extern ULONG DoSuperNew(struct IClass *, Object *, ULONG, ...);

SAVEDS ULONG mNewGUIText(Class *cl, Object *obj, struct opSet *msg)
{
  APTR retval = NULL;
  struct GUITextData *data = NULL;

  if (retval = (APTR)DoSuperNew(cl, obj, TAG_MORE, msg->ops_AttrList))
    {
      data = INST_DATA(cl, retval);
      data->object = (Object*)GetTagData(MBA_GUIObject_Object, NULL, msg->ops_AttrList);
    }
  return((ULONG)retval);
}

ULONG mDisposeGUIText(Class *cl, Object *obj, Msg msg)
{
  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mGetGUIText(Class *cl, Object *obj, struct opGet * msg)
{
  struct GUITextData *data = INST_DATA(cl, obj);
  ULONG   retval = 0;

 switch(msg->opg_AttrID)
    {
    case MBA_GUIObject_Object:
      *(msg->opg_Storage) = (ULONG) data->object;
      retval = 1L;
      break;
    }

  if (!retval)  retval = DoSuperMethodA(cl, obj, (Msg)msg);

  return((ULONG)retval);
}

ASM SAVEDS ULONG DispatcherGUIText(REG(a0) struct IClass *cl,
                             REG(a2) Object *obj,
                             REG(a1) Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:         return(mNewGUIText(cl, obj, (struct opSet *)msg));
    case OM_DISPOSE:     return(mDisposeGUIText(cl, obj, msg));
    case OM_GET:         return(mGetGUIText(cl, obj, (struct opGet *)msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitGUITextClass(void)
{
  if (GUITextClass = MUI_CreateCustomClass(NULL, SUPERCLASS, NULL, sizeof(struct GUITextData), DispatcherGUIText)) return(0);
  return(-1);
}

void DisposeGUITextClass(void)
{
  if (GUITextClass) MUI_DeleteCustomClass(GUITextClass);
}
