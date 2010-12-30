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
#include "NotifyGroup.h"

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

extern struct MUI_CustomClass *DDTextClass;
struct MUI_CustomClass *NotifyGroupClass;

extern ULONG InitDDTextClass(void);
extern void  DisposeDDTextClass(void);

extern Object *inspector;

struct NotifyGroupData
{
  Object *text;
  APTR   destination;
};

/**************************************/
/* Help Group of the inspector window */
/**************************************/

SAVEDS ULONG mNewNotifyGroup(Class *cl, Object *obj, struct opSet *msg)
{
  APTR retval = NULL;
  struct NotifyGroupData *data;
  Object *text, *events, *actions;

  retval = (APTR)DoSuperNew(cl, obj,
                            Child, HGroup,
			      Child, Label("_Destination"),
			      Child, text = NewObject(DDTextClass->mcc_Class, NULL,
			        TextFrame,
			        MUIA_Text_Contents, "",
				TAG_DONE),
			    End,
			    Child, HGroup,
			      Child, ListviewObject,
			        MUIA_Listview_List, events = ListObject,
			          InputListFrame,
			        End,
			      End,
			      Child, ListviewObject,
			        MUIA_Listview_List, actions = ListObject,
			          InputListFrame,
			        End,
			      End,
			    End,
                            TAG_DONE);
  if (retval)
    {
      data = INST_DATA(cl, retval);

      data->text = text;

    }

  return((ULONG)retval);
}

SAVEDS ULONG mSetNotifyGroup(Class *cl, Object *obj, struct opSet *msg)
{
  struct NotifyGroupData *data = INST_DATA(cl, obj);
  struct TagItem *ti, *tstate;
  ULONG  retval = 0;

  ti = msg->ops_AttrList;
  tstate = ti;
  while(ti = NextTagItem(&tstate))
  {
    switch(ti->ti_Tag)
    {
    case MBA_NotifyGroup_Destination:
      data->destination = (Object*)ti->ti_Data;
      retval = 1L;
      break;
    }
  }

  return(retval);
}

SAVEDS ULONG mGetNotifyGroup(Class *cl, Object *obj, Msg msg)
{
  struct NotifyGroupData *data = INST_DATA(cl, obj);
  ULONG retval = 0;

  return(retval);
}

ASM SAVEDS ULONG DispatcherNotifyGroup(REG(a0) struct IClass *cl,
                             REG(a2) Object *obj,
                             REG(a1) Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:  return(mNewNotifyGroup(cl, obj, (struct opSet *)msg));
    case OM_SET:  return(mSetNotifyGroup(cl, obj, (struct opSet *)msg));
    case OM_GET:  return(mGetNotifyGroup(cl, obj, msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitNotifyGroupClass(void)
{
  InitDDTextClass();
  if (NotifyGroupClass = MUI_CreateCustomClass(NULL, MUIC_Group, NULL, sizeof(struct NotifyGroupData), DispatcherNotifyGroup)) return(0);
  return(-1);
}

void DisposeNotifyGroupClass(void)
{
  if (DDTextClass)      DisposeDDTextClass();
  if (NotifyGroupClass) MUI_DeleteCustomClass(NotifyGroupClass);
}
