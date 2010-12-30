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

#define SUPERCLASS MUIC_Window

struct GUIWindowData
{
  Object *object;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;

extern Object *inspector;

struct MUI_CustomClass *GUIWindowClass;

ULONG SAVEDS ASM SelectObject(REG(a2) Object *win)
{
  APTR obj, guiobj;

  get(win, MUIA_Window_ActiveObject, &guiobj);
  if (guiobj)
    {
      get(guiobj, MBA_GUIObject_Object, &obj);
      if (obj)
	DoMethod(inspector, MUIM_Set, MBA_Inspector_CurrentObject, obj);
    }
  return(1);
}

SAVEDS ULONG mNewGUIWindow(Class *cl, Object *obj, struct opSet *msg)
{
  APTR retval = NULL;
  struct GUIWindowData *data = NULL;
  static const struct Hook SelectObjectHook = { { NULL,NULL },(VOID *)SelectObject,NULL,NULL };

  if (retval = (APTR)DoSuperMethodA(cl, obj, (Msg)msg))
    {
      data = INST_DATA(cl, retval);
      data->object = (Object*)GetTagData(MBA_GUIObject_Object, NULL, msg->ops_AttrList);
      DoMethod(retval, MUIM_Notify, MUIA_Window_ActiveObject, MUIV_EveryTime,
               retval, 3, MUIM_CallHook, &SelectObjectHook, MUIV_TriggerValue);
      DoMethod(retval, MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
	       retval, 3, MUIM_Set, MUIA_Window_Open, FALSE);
    }
  return((ULONG)retval);
}

ULONG mDisposeGUIWindow(Class *cl, Object *obj, Msg msg)
{
  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mGetGUIWindow(Class *cl, Object *obj, struct opGet * msg)
{
  struct GUIWindowData *data = INST_DATA(cl, obj);
  ULONG   retval = 0;

 switch(msg->opg_AttrID)
    {
    case MBA_GUIObject_Object:
      *(msg->opg_Storage) = (ULONG) data->object;
      retval = 1L;
      break;
    }
   if (!retval) retval = DoSuperMethodA(cl, obj, (Msg)msg);
  return((ULONG)retval);
}

ASM SAVEDS ULONG DispatcherGUIWindow(REG(a0) struct IClass *cl,
                             REG(a2) Object *obj,
                             REG(a1) Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:     return(mNewGUIWindow(cl, obj, (struct opSet *)msg));
    case OM_DISPOSE: return(mDisposeGUIWindow(cl, obj, msg));
    case OM_GET:     return(mGetGUIWindow(cl, obj, (struct opGet *)msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitGUIWindowClass(void)
{
  if (GUIWindowClass = MUI_CreateCustomClass(NULL, SUPERCLASS, NULL, sizeof(struct GUIWindowData), DispatcherGUIWindow)) return(0);
  return(-1);
}

void DisposeGUIWindowClass(void)
{
  if (GUIWindowClass) MUI_DeleteCustomClass(GUIWindowClass);
}
