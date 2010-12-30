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

struct GUIButtonData
{
  Object *object;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;

struct MUI_CustomClass *GUIButtonClass;

extern ULONG DoSuperNew(struct IClass *, Object *, ULONG, ...);

SAVEDS ULONG mNewGUIButton(Class *cl, Object *obj, struct opSet *msg)
{
  APTR retval = NULL;
  struct GUIButtonData *data = NULL;

  if (retval = (APTR)DoSuperNew(cl, obj,
                                MUIA_Text_PreParse, "\033c",
                                TAG_MORE, msg->ops_AttrList))
    {
      data = INST_DATA(cl, retval);
      data->object = (Object*)GetTagData(MBA_GUIObject_Object, NULL, msg->ops_AttrList);
    }
  return((ULONG)retval);
}

ULONG mDisposeGUIButton(Class *cl, Object *obj, Msg msg)
{
  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mGetGUIButton(Class *cl, Object *obj, struct opGet * msg)
{
  struct GUIButtonData *data = INST_DATA(cl, obj);
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

ASM SAVEDS ULONG DispatcherGUIButton(REG(a0) struct IClass *cl,
                             REG(a2) Object *obj,
                             REG(a1) Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:         return(mNewGUIButton(cl, obj, (struct opSet *)msg));
    case OM_DISPOSE:     return(mDisposeGUIButton(cl, obj, msg));
    case OM_GET:         return(mGetGUIButton(cl, obj, (struct opGet *)msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitGUIButtonClass(void)
{
  if (GUIButtonClass = MUI_CreateCustomClass(NULL, SUPERCLASS, NULL, sizeof(struct GUIButtonData), DispatcherGUIButton)) return(0);
  return(-1);
}

void DisposeGUIButtonClass(void)
{
  if (GUIButtonClass) MUI_DeleteCustomClass(GUIButtonClass);
}
