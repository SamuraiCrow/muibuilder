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

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;

struct IClass *BaseClass;

struct BaseData
{
  Object *muiobj;
  char   *label;
  char   *help;
};

SAVEDS ULONG mSet(Class *cl, Object *obj, struct opSet * msg)
{
  struct BaseData *data = INST_DATA(cl, obj);
  struct TagItem *ti, *tstate;
  ULONG  retval = 0;

  retval = DoSuperMethodA(cl, obj, (Msg)msg);
  ti = msg->ops_AttrList;
  tstate = ti;
  while(ti = NextTagItem(&tstate))
  {
    switch(ti->ti_Tag)
    {
    case MBA_Object_Label:
      if (data->label) FreeVec(data->label);
      data->label = (char*)ti->ti_Data;
      retval = 1L;
      break;
    case MBA_Object_MUIObject:
      data->muiobj = (Object*)ti->ti_Data;
      retval = 1L;
      break;
    case MBA_Object_Help:
      if (data->help) FreeVec(data->help);
      data->help = (char*)ti->ti_Data;
      retval = 1L;
      break;
    }
  }
  return(retval);
}

SAVEDS ULONG mGet(Class *cl, Object *obj, struct opGet * msg)
{
  struct BaseData *data;
  ULONG  retval = 0;

  data = INST_DATA(cl, obj);
  switch(msg->opg_AttrID)
    {
    case MBA_Object_Label:
      *(msg->opg_Storage) = (ULONG) data->label;
      retval = 1;
      break;
    case MBA_Object_MUIObject:
      *(msg->opg_Storage) = (ULONG) data->muiobj;
      retval = 1;
      break;
    case MBA_Object_Help:
      *(msg->opg_Storage) = (ULONG) data->help;
      retval = 1;
      break;
    case MBA_Object_Type:
      *(msg->opg_Storage) = (ULONG) MBV_Object_Type_Standard;
      retval = 1;
      break;
    default:
      retval = DoSuperMethodA(cl, obj, (Msg) msg);
      break;
    }
  return((ULONG)retval);
}

SAVEDS ULONG mNew(Class *cl, Object *obj, struct opSet *msg)
{
  struct BaseData *data = NULL;
  APTR retval = NULL;

  if (retval = (APTR)DoSuperMethodA(cl, obj, (Msg) msg))
    {
      data = INST_DATA(cl, retval);
      data->label = (char*)GetTagData(MBA_Object_Label, NULL, msg->ops_AttrList);
      data->muiobj = NULL;
      data->help = (char*)GetTagData(MBA_Object_Help, NULL, msg->ops_AttrList);
    }

  return((ULONG)retval);
}

ULONG mDispose(Class *cl, Object *obj, Msg msg)
{
  struct BaseData *data = INST_DATA(cl, obj);

  if (data->label) FreeVec(data->label);
  if (data->help)  FreeVec(data->help);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mAttributesEdit(Class *cl, Object *obj, struct MBP_AttributesEdit * msg)
{
  return(1);
}

SAVEDS ULONG mSaveAttributes(Class *cl, Object *obj, Msg msg)
{
  return(1);
}

SAVEDS ULONG Dispatcher(struct IClass *cl,
                            Object *obj,
                            Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:    return(mNew(cl, obj, (struct opSet *)msg));
    case OM_SET:    return(mSet(cl, obj, (struct opSet *)msg));
    case OM_GET:    return(mGet(cl, obj, (struct opGet *)msg));
    case OM_DISPOSE:return(mDispose(cl, obj, msg));
    case MBM_Object_AttributesEdit: return((ULONG)mAttributesEdit(cl, obj, (struct MBP_AttributesEdit *)msg));
    case MBM_Object_SaveAttributes: return((ULONG)mSaveAttributes(cl, obj, msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

SAVEDS void InitBaseClass(void)
{
  if (BaseClass = MakeClass("MBBaseClass", "modelclass", NULL, sizeof(struct BaseData), 0))
    {
      BaseClass->cl_Dispatcher.h_Entry = HookEntry;
      BaseClass->cl_Dispatcher.h_SubEntry = Dispatcher;
      AddClass(BaseClass);
    }
}

SAVEDS void DisposeBaseClass(void)
{
  RemoveClass(BaseClass);
  FreeClass(BaseClass);
}
