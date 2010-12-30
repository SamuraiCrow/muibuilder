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

struct GUITextData
{
  Object *object;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;

struct MUI_CustomClass *GUITextClass;

extern ULONG DoSuperNew(struct IClass *, Object *, ULONG, ...);

SAVEDS ULONG mNewGUIText(Class *cl, Object *obj, struct opSet *msg)
{
  APTR retval = NULL;
  struct GUITextData *data = NULL;

  if (retval = (APTR)DoSuperNew(cl, obj, TAG_MORE, msg->ops_AttrList))
    {
      data = INST_DATA(cl, retval);
      data->object = (Object*)GetTagData(MBA_GUIObject_Object, NULL, msg->ops_AttrList);
    }
  return((ULONG)retval);
}

ULONG mDisposeGUIText(Class *cl, Object *obj, Msg msg)
{
  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mGetGUIText(Class *cl, Object *obj, struct opGet * msg)
{
  struct GUITextData *data = INST_DATA(cl, obj);
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

DISPATCHER(DispatcherGUIText)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:         return(mNewGUIText(cl, obj, (struct opSet *)msg));
    case OM_DISPOSE:     return(mDisposeGUIText(cl, obj, msg));
    case OM_GET:         return(mGetGUIText(cl, obj, (struct opGet *)msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitGUITextClass(void)
{
  if (GUITextClass = MUI_CreateCustomClass(NULL, SUPERCLASS, NULL, sizeof(struct GUITextData), DispatcherGUIText)) return(0);
  return(-1);
}

void DisposeGUITextClass(void)
{
  if (GUITextClass) MUI_DeleteCustomClass(GUITextClass);
}
