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

DISPATCHER(DispatcherHelpGroup)
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
