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

struct PaletteData
{
  Object *window;
  Object *group;
  Object *button;
  Object *text;
  Object *gauge;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;

extern Object *app;
extern Object *muibuilder;

struct MUI_CustomClass *PaletteClass;

ULONG SAVEDS DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...)
{
	return(DoSuperMethod(cl,obj,OM_NEW,&tag1,NULL));
}

SAVEDS ULONG mNewPalette(Class *cl, Object *obj, struct opSet *msg)
{
  struct PaletteData *data;
  APTR retval = NULL;
  APTR pagegrp, objpage, aux;
  APTR btobj;

  retval = (APTR)DoSuperNew(cl, obj,
			    MUIA_Window_Title, "Palette",
			    MUIA_Window_RootObject, VGroup,
			      Child, HGroup,
			        Child, btobj = TextObject,
			          ButtonFrame,
			          MUIA_Text_Contents, "Objects",
			          MUIA_Text_PreParse, "\33c",
			          MUIA_Text_HiChar, "o",
			          MUIA_ControlChar, "o",
			          MUIA_InputMode, MUIV_InputMode_Toggle,
			          MUIA_Background, MUII_ButtonBack,
			        End,
			      End,
			      Child, MUI_MakeObject(MUIO_HBar, 2),
			      Child, pagegrp = GroupObject,
			        MUIA_Group_PageMode, TRUE,
			        Child, objpage = ColGroup(3),
			          MUIA_Group_SameWidth, TRUE,
			        End,
			      End,
			    End,
			    TAG_MORE, msg->ops_AttrList
			    );
  if (retval)
    {
      data = INST_DATA(cl, retval);

      /* Notifications */
      DoMethod(btobj, MUIM_Notify, MUIA_Selected, TRUE,
	       pagegrp, 3, MUIM_Set, MUIA_Group_ActivePage, 0);
      DoMethod(retval, MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
	       retval, 3, MUIM_Set, MUIA_Window_Open, FALSE);

      /* Window Creation */
      data->window = NewObject(NULL, "MBWindowClass", TAG_DONE);
      data->group  = NewObject(NULL, "MBGroupClass",  TAG_DONE);
      data->button = NewObject(NULL, "MBButtonClass", TAG_DONE);
      data->text   = NewObject(NULL, "MBTextClass",   TAG_DONE);
      data->gauge  = NewObject(NULL, "MBGaugeClass",  TAG_DONE);

      /* Group */
      if (aux = (APTR)DoMethod(data->group, MBM_Object_GetImage, TAG_DONE))
	{
	  set(aux, MUIA_UserData, "MBGroupClass");
	  DoMethod(objpage, OM_ADDMEMBER, aux);
	}

      /* Button */
      if (aux = (APTR)DoMethod(data->button, MBM_Object_GetImage, TAG_DONE))
	{
	  set(aux, MUIA_UserData, "MBButtonClass");
	  DoMethod(objpage, OM_ADDMEMBER, aux);
	}
      /* Text */
      if (aux = (APTR)DoMethod(data->text, MBM_Object_GetImage, TAG_DONE))
	{
	  set(aux, MUIA_UserData, "MBTextClass");
	  DoMethod(objpage, OM_ADDMEMBER, aux);
	}
      /* Gauge */
      if (aux = (APTR)DoMethod(data->gauge, MBM_Object_GetImage, TAG_DONE))
	{
	  set(aux, MUIA_UserData, "MBGaugeClass");
	  DoMethod(objpage, OM_ADDMEMBER, aux);
	}
    }

  return((ULONG)retval);
}

ULONG mDisposePalette(Class *cl, Object *obj, Msg msg)
{
  struct PaletteData *data = INST_DATA(cl, obj);

  if (data->window) DisposeObject(data->window);
  if (data->group)  DisposeObject(data->group);
  if (data->button) DisposeObject(data->button);
  return(DoSuperMethodA(cl, obj, msg));
}

DISPATCHER(DispatcherPalette)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:         return(mNewPalette(cl, obj, (struct opSet *)msg));
    case OM_DISPOSE:     return(mDisposePalette(cl, obj, msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitPaletteClass(void)
{
  if (PaletteClass = MUI_CreateCustomClass(NULL, SUPERCLASS, NULL, sizeof(struct PaletteData), DispatcherPalette)) return(0);
  return(-1);
}

void DisposePaletteClass(void)
{
  if (PaletteClass) MUI_DeleteCustomClass(PaletteClass);
}
