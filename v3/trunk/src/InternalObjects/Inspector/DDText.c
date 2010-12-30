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
#include "SDI_compiler.h"
#include "SDI_hook.h"

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

DISPATCHER(DispatcherDDText)
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

