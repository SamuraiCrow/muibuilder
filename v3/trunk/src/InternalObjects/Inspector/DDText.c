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
struct MUI_CustomClass *DDTextClass;

extern Object *inspector;

struct DDTextData
{
  ULONG foo;
};

/**************************************/
/* Text to put destination object     */
/* for notifications                  */
/**************************************/

ULONG SAVEDS mDragDropDDText(Class *cl, Object *obj, struct MUIP_DragDrop *msg)
{
  char *classname;
  APTR muibobj;
  char *label;

  get(msg->obj, MUIA_UserData, &classname);
  if (!classname)
  {
    get(msg->obj, MBA_GUIObject_Object, &muibobj);
    if (muibobj)
      {
	get(muibobj, MBA_Object_Label, &label);
	set(obj, MUIA_Text_Contents, label);
	set(inspector, MBA_Inspector_NotifyDestination, muibobj);
      }
    return(1);
  }
  return(0);
}

ULONG mDragQueryDDText(struct IClass *cl,Object *obj, struct MUIP_DragDrop *msg)
{
  char *classname;

  get(msg->obj, MUIA_UserData, &classname);
  if (!classname) return(MUIV_DragQuery_Accept);
  else return(MUIV_DragQuery_Refuse);
}

ASM SAVEDS ULONG DispatcherDDText(REG(a0) struct IClass *cl,
                             REG(a2) Object *obj,
                             REG(a1) Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case MUIM_DragDrop:   return(mDragDropDDText(cl, obj, (struct MUIP_DragDrop *)msg));
    case MUIM_DragQuery:  return(mDragQueryDDText(cl, obj, (struct MUIP_DragDrop *)msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitDDTextClass(void)
{
  if (DDTextClass = MUI_CreateCustomClass(NULL, MUIC_Text, NULL, sizeof(struct DDTextData), DispatcherDDText)) return(0);
  return(-1);
}

void DisposeDDTextClass(void)
{
  if (DDTextClass) MUI_DeleteCustomClass(DDTextClass);
}

