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

/* TextField */
#include <proto/textfield.h>
#include <gadgets/textfield.h>
#include <intuition/icclass.h>
#include <intuition/classes.h>
#include <intuition/gadgetclass.h>

#include <stdio.h>
#include <string.h>

#include "//object.h"


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

extern struct Library *TextFieldBase;
struct MUI_CustomClass *DDGroupClass;

extern Object *inspector;

struct DDGroupData
{
  ULONG foo;
};

/**************************************/
/* Main Group of the inspector window */
/* (enables Drag&Drop)                */
/**************************************/

ULONG SAVEDS mDragDropDDGroup(Class *cl, Object *obj, struct MUIP_DragDrop *msg)
{
  char *classname;
  APTR muibobj;

  get(msg->obj, MUIA_UserData, &classname);
  if (!classname)
  {
    get(msg->obj, MBA_GUIObject_Object, &muibobj);
    if (muibobj) set(inspector, MBA_Inspector_CurrentObject, muibobj);
    return(1);
  }
  return(0);
}

ULONG mDragQueryDDGroup(struct IClass *cl,Object *obj, struct MUIP_DragDrop *msg)
{
  char *classname;

  get(msg->obj, MUIA_UserData, &classname);
  if (!classname) return(MUIV_DragQuery_Accept);
  else return(MUIV_DragQuery_Refuse);
}

ASM SAVEDS ULONG DispatcherDDGroup(REG(a0) struct IClass *cl,
                             REG(a2) Object *obj,
                             REG(a1) Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case MUIM_DragDrop:   return(mDragDropDDGroup(cl, obj, (struct MUIP_DragDrop *)msg));
    case MUIM_DragQuery:  return(mDragQueryDDGroup(cl, obj, (struct MUIP_DragDrop *)msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitDDGroupClass(void)
{
  if (DDGroupClass = MUI_CreateCustomClass(NULL, MUIC_Group, NULL, sizeof(struct DDGroupData), DispatcherDDGroup)) return(0);
  return(-1);
}

void DisposeDDGroupClass(void)
{
  if (DDGroupClass) MUI_DeleteCustomClass(DDGroupClass);
}

