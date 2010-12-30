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

#define SUPERCLASS MUIC_Group

struct GUIGroupData
{
  Object *object;
  BOOL   isempty;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;
extern Object *inspector;

struct MUI_CustomClass *GUIGroupClass;

ULONG xdrop, ydrop;

SAVEDS ULONG mNewGUIGroup(Class *cl, Object *obj, struct opSet *msg)
{
  APTR retval = NULL;
  struct GUIGroupData *data = NULL;

  if (retval = (APTR)DoSuperMethodA(cl, obj, (Msg) msg))
    {
      data = INST_DATA(cl, retval);
      data->object = (Object*)GetTagData(MBA_GUIObject_Object, NULL, msg->ops_AttrList);
      data->isempty  = TRUE;
    }
  return((ULONG)retval);
}

ULONG mDisposeGUIGroup(Class *cl, Object *obj, Msg msg)
{
  return(DoSuperMethodA(cl, obj, msg));
}

APTR InsertObjectPosition(Class *cl, Object *obj, Object *insertobj)
{
  struct GUIGroupData * data = INST_DATA(cl, obj);
  struct List *list;
  APTR   objstate = NULL;
  APTR   child = NULL;
  APTR   retval;
  ULONG  horiz, x1, x2, aux;

  get(obj, MUIA_Group_ChildList, &list);
  get(data->object, MBA_Group_Horiz, &horiz);

  if (list) objstate = list->lh_Head;

  get(data->object, MBA_Group_IsEmpty, &aux);
  if (aux)
    {
      /* If the group is empty */
      /* we delete the Space   */
      child = NextObject(&objstate);
      if (child)
	{
	  DoMethod(obj, OM_REMMEMBER, child);
	  MUI_DisposeObject(child);
	}
    }

  while(child = NextObject(&objstate))
    {
      if (horiz)
        {
          get(child, MUIA_LeftEdge, &x1);
          get(child, MUIA_RightEdge, &x2);
          if (xdrop<=(ULONG)((x1+x2)/2)) break;
        }
      else
        {
          get(child, MUIA_TopEdge, &x1);
          get(child, MUIA_BottomEdge, &x2);
          if (ydrop<=(ULONG)((x1+x2)/2)) break;
        }
    }

  DoMethod(obj, OM_ADDMEMBER, insertobj);

  retval = child;

  while((child) && (child != insertobj))
    {
      DoMethod(obj, OM_REMMEMBER, child);
      DoMethod(obj, OM_ADDMEMBER, child);
      child = NextObject(&objstate);
    }

  return(retval);
}

ULONG SAVEDS mDragDropGUIGroup(Class *cl, Object *obj, struct MUIP_DragDrop *msg)
{
  struct GUIGroupData * data = INST_DATA(cl, obj);
  char *classname;
  APTR muibobj = NULL;
  APTR guiobj = NULL;
  APTR pos, win, parentgrp, muibgrp;

  if (msg->obj == obj) return(0);

  DoMethod(obj, MUIM_Group_InitChange, TAG_DONE);
  get(msg->obj, MUIA_UserData, &classname);
  get(obj, MUIA_WindowObject, &win);
  /* Does the dropped object come from palette or GUI builded ? */
  if (classname)
    {
      /* Comes from palette */
      /* Create object      */
      if (muibobj = NewObject(NULL, classname, TAG_DONE))
        guiobj = (APTR)DoMethod(muibobj, MBM_Object_CreateEdition, TAG_DONE);
    }
  else
    {
      /* Comes from gui builded */
      /* Move Object            */
      get(msg->obj, MUIA_Parent, &parentgrp);
      if (parentgrp)
	{

	  get(parentgrp, MBA_GUIObject_Object, &muibgrp);
	}
      else muibgrp = NULL;
      guiobj = msg->obj;
      get(guiobj, MBA_GUIObject_Object, &muibobj);
      if (muibgrp&&muibobj) DoMethod(muibgrp, MBM_Group_RemObject, muibobj);
      if (parentgrp)
	{
	  DoMethod(parentgrp, MUIM_Group_InitChange, TAG_DONE);
	  DoMethod(parentgrp, OM_REMMEMBER, msg->obj);
	  DoMethod(parentgrp, MUIM_Group_ExitChange, TAG_DONE);
	}
    }
  if (guiobj&&muibobj)
    {
      set(guiobj, MUIA_UserData, 0L);
      if (pos = InsertObjectPosition(cl, obj, guiobj))
        get(pos, MBA_GUIObject_Object, &pos);
      DoMethod(data->object, MBM_Group_AddObject, muibobj, pos);
    }
  DoMethod(obj, MUIM_Group_ExitChange, TAG_DONE);
  return(1);
}

ULONG mDragQueryGUIGroup(struct IClass *cl,Object *obj, struct MUIP_DragDrop *msg)
{
  if (msg->obj == obj) return(MUIV_DragQuery_Refuse);
  return(MUIV_DragQuery_Accept);
}

ULONG mDragReportGUIGroup(struct IClass *cl,Object *obj, struct MUIP_DragReport *msg)
{
  xdrop = msg->x;
  ydrop = msg->y;
  return(1);
}

ULONG mObjectUpdate(Class *cl, Object *obj, struct MBP_AddObj *msg)
{
  struct List *list;
  APTR   objstate = NULL;
  APTR   child = NULL;
  APTR   retval, muibobj;

  DoMethod(obj, MUIM_Group_InitChange, TAG_DONE);

  get(obj, MUIA_Group_ChildList, &list);
  get(msg->source, MBA_GUIObject_Object, &muibobj);

  if (list) objstate = list->lh_Head;
  else return(0);

  while( child = NextObject(&objstate))
    {
      if (child == msg->source) break;
    }

  if (child && (retval = (APTR)DoMethod(muibobj, MBM_Object_CreateEdition, TAG_DONE)))
    {
      DoMethod(obj, OM_REMMEMBER, msg->source);
      MUI_DisposeObject(msg->source);
      set(retval, MUIA_UserData, 0L);
      DoMethod(obj, OM_ADDMEMBER, retval);
      child = NextObject(&objstate);
    }

  if (retval)
    {
      while((child) && (child != retval))
        {
          DoMethod(obj, OM_REMMEMBER, child);
          DoMethod(obj, OM_ADDMEMBER, child);
          child = NextObject(&objstate);
        }

      DoMethod(obj, MUIM_Group_ExitChange, TAG_DONE);
    }

  return((ULONG)retval);
}

SAVEDS ULONG mGetGUIGroup(Class *cl, Object *obj, struct opGet * msg)
{
  struct GUIGroupData *data = INST_DATA(cl, obj);
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

DISPATCHER(DispatcherGUIGroup)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:          return(mNewGUIGroup(cl, obj, (struct opSet *)msg));
    case OM_DISPOSE:      return(mDisposeGUIGroup(cl, obj, msg));
    case OM_GET:          return(mGetGUIGroup(cl, obj, (struct opGet *)msg));
    case MUIM_DragDrop:   return(mDragDropGUIGroup(cl, obj, (struct MUIP_DragDrop *)msg));
    case MUIM_DragQuery:  return(mDragQueryGUIGroup(cl, obj, (struct MUIP_DragDrop *)msg));
    case MUIM_DragReport: return(mDragReportGUIGroup(cl, obj, (struct MUIP_DragReport *)msg));
    case MBM_Group_UpdateObject: return(mObjectUpdate(cl, obj, (struct MBP_AddObj *)msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitGUIGroupClass(void)
{
  if (GUIGroupClass = MUI_CreateCustomClass(NULL, SUPERCLASS, NULL, sizeof(struct GUIGroupData), DispatcherGUIGroup)) return(0);
  return(-1);
}

void DisposeGUIGroupClass(void)
{
  if (GUIGroupClass) MUI_DeleteCustomClass(GUIGroupClass);
}
