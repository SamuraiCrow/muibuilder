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

HOOKPROTONHNP(SelectObject, ULONG, Object *win)
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
  MakeStaticHook(SelectObjectHook, SelectObject);

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

DISPATCHER(DispatcherGUIWindow)
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
