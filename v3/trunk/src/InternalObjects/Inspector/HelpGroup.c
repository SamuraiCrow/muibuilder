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

extern ULONG DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...);

struct MUI_CustomClass *HelpGroupClass;
Class *TextFieldClass;

extern Object *inspector;

struct HelpGroupData
{
  Object *text;
};

/**************************************/
/* Help Group of the inspector window */
/**************************************/

SAVEDS ULONG mNewHelpGroup(Class *cl, Object *obj, struct opSet *msg)
{
  APTR retval = NULL;
  struct HelpGroupData *data;
  Object *text, *scroll;

  retval = (APTR)DoSuperNew(cl, obj,
                            MUIA_Group_HorizSpacing, 0,
                            MUIA_Group_Horiz, TRUE,
                            Child, text = BoopsiObject,
                             InputListFrame,
                             MUIA_Background, MUII_BACKGROUND,
                             MUIA_Boopsi_Class, TextFieldClass,
                             MUIA_Boopsi_Smart, TRUE,
                             MUIA_Boopsi_MinWidth, 20,
                             MUIA_Boopsi_MinHeight, 20,
                             ICA_TARGET, ICTARGET_IDCMP,
                             TEXTFIELD_Text,(ULONG)"",
                            End,
                            Child, scroll = ScrollbarObject, End,
                            TAG_DONE);
  if (retval)
    {
      data = INST_DATA(cl, retval);

      data->text = text;

      DoMethod(text, MUIM_Notify, TEXTFIELD_Lines, MUIV_EveryTime,
           scroll, 3, MUIM_Set, MUIA_Prop_Entries, MUIV_TriggerValue);

      DoMethod(text, MUIM_Notify, TEXTFIELD_Visible, MUIV_EveryTime,
           scroll, 3, MUIM_Set, MUIA_Prop_Visible, MUIV_TriggerValue);

      DoMethod(text, MUIM_Notify, TEXTFIELD_Top, MUIV_EveryTime,
           scroll, 3, MUIM_NoNotifySet, MUIA_Prop_First, MUIV_TriggerValue);

      DoMethod(scroll, MUIM_Notify, MUIA_Prop_First, MUIV_EveryTime,
           text, 3, MUIM_Set, TEXTFIELD_Top, MUIV_TriggerValue);
    }

  return((ULONG)retval);
}

SAVEDS ULONG mSetHelpGroup(Class *cl, Object *obj, Msg msg)
{
  struct HelpGroupData *data = INST_DATA(cl, obj);
  ULONG retval;

  retval = DoMethodA(data->text, msg);

  return(retval);
}

SAVEDS ULONG mGetHelpGroup(Class *cl, Object *obj, Msg msg)
{
  struct HelpGroupData *data = INST_DATA(cl, obj);
  ULONG retval;

  retval = DoMethodA(data->text, msg);

  return(retval);
}

ASM SAVEDS ULONG DispatcherHelpGroup(REG(a0) struct IClass *cl,
                             REG(a2) Object *obj,
                             REG(a1) Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:  return(mNewHelpGroup(cl, obj, (struct opSet *)msg));
    case OM_SET:  return(mSetHelpGroup(cl, obj, msg));
    case OM_GET:  return(mGetHelpGroup(cl, obj, msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitHelpGroupClass(void)
{
  TextFieldClass = TEXTFIELD_GetClass();
  if (HelpGroupClass = MUI_CreateCustomClass(NULL, MUIC_Group, NULL, sizeof(struct HelpGroupData), DispatcherHelpGroup)) return(0);
  return(-1);
}

void DisposeHelpGroupClass(void)
{
  if (HelpGroupClass) MUI_DeleteCustomClass(HelpGroupClass);
}
