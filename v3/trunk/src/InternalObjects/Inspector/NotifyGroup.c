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
#include <proto/alib.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/utility.h>
#include <proto/muimaster.h>

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

#include "../../object.h"
#include "NotifyGroup.h"
#include "SDI_compiler.h"
#include "SDI_hook.h"

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

DISPATCHER(DispatcherNotifyGroup)
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
