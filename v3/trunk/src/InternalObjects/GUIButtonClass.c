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

#include <stdio.h>
#include <string.h>

#include "../object.h"
#include "SDI_compiler.h"
#include "SDI_hook.h"

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

DISPATCHER(DispatcherGUIButton)
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
