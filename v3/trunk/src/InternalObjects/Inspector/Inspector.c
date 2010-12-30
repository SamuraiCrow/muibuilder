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

#define SUPERCLASS MUIC_Window

#define MBV_Inspector_Attributes  0
#define MBV_Inspector_Connections 1
#define MBV_Inspector_Menu        2
#define MBV_Inspector_Help        3

struct InspectorData
{
  Object *current;
  Object *pagegrp;
  Object *cycle;
  Object *attrgrp;
  Object *helpgrp;
  Object *notifygrp;
  Object *STR_label;
};

struct UpdateDisplayArgs
{
  Object *obj;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;
extern Object *inspector;

extern Object *app;
extern ULONG DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...);

struct MUI_CustomClass *InspectorClass;
extern struct MUI_CustomClass *DDGroupClass;
extern struct MUI_CustomClass *HelpGroupClass;
extern struct MUI_CustomClass *NotifyGroupClass;

extern ULONG InitHelpGroupClass(void);
extern void  DisposeHelpGroupClass(void);
extern ULONG InitDDGroupClass(void);
extern void  DisposeDDGroupClass(void);
extern ULONG InitNotifyGroupClass(void);
extern void  DisposeNotifyGroupClass(void);

/******************************/
/* Inspector Class Definition */
/******************************/

Object * EmptyGroup(void)
{
  return(GroupObject,
         Child, HVSpace,
         Child, CLabel("No Entry"),
         Child, HVSpace,
         End);
}

SAVEDS void UpdateAttributes(Class *cl, Object *obj)
{
  struct InspectorData *data = INST_DATA(cl, obj);
  struct List *list;
  Object *current;
  static char *registertitles[10];
  int i;

  /* Dispose Current group content */
  DoMethod(data->attrgrp, MUIM_Group_InitChange, TAG_DONE);
  get(data->attrgrp, MUIA_Group_ChildList, &list);
  if (current = NextObject(&(list->lh_Head)))
    {
      DoMethod(data->attrgrp, OM_REMMEMBER, current);
      MUI_DisposeObject(current);
    }
  /* Put the new content inside the group */
  if (data->current)
    {
      for(i=0;i<10;i++) registertitles[i] = NULL;
      if (current = RegisterGroup(registertitles), End)
        DoMethod(data->current, MBM_Object_AttributesEdit, current, 0L, registertitles);
    }
  else
    current = EmptyGroup();
  if (current) DoMethod(data->attrgrp, OM_ADDMEMBER, current);
  DoMethod(data->attrgrp, MUIM_Group_ExitChange, TAG_DONE);
}

SAVEDS void UpdateDisplay(Object *obj)
{
  struct InspectorData *data = INST_DATA(InspectorClass->mcc_Class, obj);
  ULONG  cycleval;
  char   *label, *txt;

  if (!data->current) return;

  get(data->cycle, MUIA_Cycle_Active, &cycleval);

  get(data->current, MBA_Object_Label, &label);
  if (label) set(data->STR_label, MUIA_String_Contents, label);
  else set(data->STR_label, MUIA_String_Contents, "");

  switch(cycleval)
    {
    case MBV_Inspector_Attributes:
      UpdateAttributes(InspectorClass->mcc_Class, obj);
      break;
    case MBV_Inspector_Help:
      get(data->current, MBA_Object_Help, &txt);
      if (txt) set(data->helpgrp, TEXTFIELD_Text, txt);
      else set(data->helpgrp, TEXTFIELD_Text, "");
      break;
    }
  set(data->pagegrp, MUIA_Group_ActivePage, cycleval);
}

/* Apply attributes change on object */
SAVEDS ASM ULONG ApplyFunction(REG(a2) Object *obj)
{
  APTR muiobj, muibobj, parent;
  struct InspectorData * data;
  ULONG cycleval, size, type;
  char *txt, *newtxt;

  data = INST_DATA(InspectorClass->mcc_Class, obj);

  if (!data->current) return(0);
  get(data->cycle, MUIA_Cycle_Active, &cycleval);

  get(data->STR_label, MUIA_String_Contents, &txt);
  if (newtxt = AllocVec((size = strlen(txt))+1, MEMF_PUBLIC|MEMF_CLEAR))
    {
      memcpy(newtxt, txt, size);
      set(data->current, MBA_Object_Label, newtxt);
    }

  switch(cycleval)
    {
    case MBV_Inspector_Attributes:
      /* Update attributes values */
      DoMethod(data->current, MBM_Object_AttributesApply, TAG_DONE);
      get(data->current, MBA_Object_Type, &type);
      /* Get the mui object */
      get(data->current, MBA_Object_MUIObject, &muiobj);
      if (!type)
        {
          /* We know the parent is a group (at least the root group) */
          if (muiobj)
            {
              get(muiobj, MUIA_Parent, &parent);
              if (parent)
                {
                  /* If it is not the root group, then */
                  /* Update parent group content */
                  DoMethod(parent, MBM_Group_UpdateObject, muiobj);
                }
              else
                {
                  /* We have to rebuild the whole window */
                  get(muiobj, MUIA_WindowObject, &parent);
                  get(parent, MBA_GUIObject_Object, &muibobj);
                  DoMethod(app, OM_REMMEMBER, parent);
                  MUI_DisposeObject(parent);
                  if (parent = (APTR)DoMethod(muibobj, MBM_Object_CreateEdition, TAG_DONE))
                    {
                      DoMethod(app, OM_ADDMEMBER, parent);
                      set(parent, MUIA_Window_Open, TRUE);
                    }
                }
            }
        }
      else
        {
          /* It is a window : we have to rebuild it */
          get(muiobj, MBA_GUIObject_Object, &muibobj);
          DoMethod(app, OM_REMMEMBER, muiobj);
          MUI_DisposeObject(muiobj);
          if (muiobj = (APTR)DoMethod(muibobj, MBM_Object_CreateEdition, TAG_DONE))
            {
              DoMethod(app, OM_ADDMEMBER, muiobj);
              set(muiobj, MUIA_Window_Open, TRUE);
            }
        }
      break;
    case MBV_Inspector_Help:
      set(data->helpgrp, TEXTFIELD_ReadOnly, TRUE);
      get(data->helpgrp, TEXTFIELD_Text, &txt);
      get(data->helpgrp, TEXTFIELD_Size, &size);
      if (newtxt = AllocVec(size+1, MEMF_PUBLIC|MEMF_CLEAR))
        {
          memcpy(newtxt, txt, size);
          set(data->current, MBA_Object_Help, newtxt);
        }
      set(data->helpgrp, TEXTFIELD_ReadOnly, FALSE);
      break;
    }
  return(1);
}

SAVEDS ULONG mNewInspector(Class *cl, Object *obj, struct opSet *msg)
{
  struct InspectorData *data;
  APTR retval = NULL;
  APTR cycle, grp, attrgrp, helpgrp, btapply, btrevert, strlabel;
  static const struct Hook ApplyFunctionHook = { { NULL,NULL },(VOID *)ApplyFunction,NULL,NULL };
  static const struct Hook UpdateDisplayHook = { { NULL,NULL },(VOID *)UpdateDisplay,NULL,NULL };
  static char *GroupNames[] =
    {
      "Attributes",
      "Connections",
      "Menu",
      "Help",
      NULL
    };

  retval = (APTR)DoSuperNew(cl, obj,
                            MUIA_Window_Title, "Inspector",
                            MUIA_Window_RootObject, NewObject(DDGroupClass->mcc_Class, NULL,
                              Child, HGroup,
                                Child, Label("_Object"),
                                Child, strlabel = MUI_MakeObject(MUIO_String, "", 40),
                              End,
                              Child, cycle = Cycle(GroupNames),
                              Child, grp = PageGroup,
                                GroupFrame,
                                /* Attributes Page */
                                Child, attrgrp = VGroup,
                                  Child, EmptyGroup(),
                                End,
                                Child, NewObject(NotifyGroupClass->mcc_Class, NULL, TAG_DONE),
                                Child, VGroup,
                                  Child, EmptyGroup(),
                                End,
                                /* Help Page */
                                Child, helpgrp = NewObject(HelpGroupClass->mcc_Class, NULL, TAG_DONE),
                              End,
                              Child, HGroup,
                                Child, btapply = SimpleButton("_Apply"),
                                Child, btrevert= SimpleButton("_Revert"),
                              End, TAG_DONE),
                            TAG_MORE, msg->ops_AttrList);
  if (retval)
    {
      data = INST_DATA(cl, retval);

      data->cycle = cycle;
      DoMethod(cycle, MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime,
               retval, 3, MUIM_CallHook, &UpdateDisplayHook, retval);
      DoMethod(btapply, MUIM_Notify, MUIA_Selected, FALSE,
               retval, 2, MUIM_CallHook, &ApplyFunctionHook);
      DoMethod(btrevert, MUIM_Notify, MUIA_Selected, FALSE,
               retval, 3, MUIM_CallHook, &UpdateDisplayHook, retval);
      DoMethod(retval, MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
	       retval, 3, MUIM_Set, MUIA_Window_Open, FALSE);
      data->pagegrp = grp;
      data->attrgrp = attrgrp;
      data->helpgrp = helpgrp;
      data->STR_label = strlabel;
      data->current = (APTR)GetTagData(MBA_Inspector_CurrentObject,
                                       NULL,
                                       msg->ops_AttrList);
      if (data->current) UpdateDisplay(retval);
    }

  return((ULONG)retval);
}

ULONG mDisposeInspector(Class *cl, Object *obj, Msg msg)
{
  return(DoSuperMethodA(cl, obj, msg));
}

ASM SAVEDS ULONG mSetInspector(REG(a0) Class *cl,
                               REG(a2) Object *obj,
                               REG(a1) struct opSet *msg)
{
  struct InspectorData *data = INST_DATA(cl, obj);
  struct TagItem *ti, *tstate;
  ULONG  retval = 0;

  retval = DoSuperMethodA(cl, obj, (Msg)msg);
  ti = msg->ops_AttrList;
  tstate = ti;
  while(ti = NextTagItem(&tstate))
  {
    switch(ti->ti_Tag)
    {
    case MBA_Inspector_CurrentObject:
      if (data->current != (Object*)ti->ti_Data)
        {
          data->current = (Object*)ti->ti_Data;
          UpdateDisplay(obj);
	  set(obj, MUIA_Window_Open, TRUE);
        }
      retval = 1L;
      break;
    case MBA_Inspector_NotifyDestination:
      set(data->notifygrp, MBA_NotifyGroup_Destination, (Object*)ti->ti_Data);
      retval = 1L;
      break;
    }
  }
  return(retval);
}

ASM SAVEDS ULONG DispatcherInspector(REG(a0) struct IClass *cl,
                                     REG(a2) Object *obj,
                                     REG(a1) Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:         return(mNewInspector(cl, obj, (struct opSet *)msg));
    case OM_DISPOSE:     return(mDisposeInspector(cl, obj, msg));
    case OM_SET:         return(mSetInspector(cl, obj, (struct opSet *)msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitInspectorClass(void)
{
  InitDDGroupClass();
  InitHelpGroupClass();
  InitNotifyGroupClass();
  if (InspectorClass = MUI_CreateCustomClass(NULL, SUPERCLASS, NULL, sizeof(struct InspectorData), DispatcherInspector)) return(0);
  return(-1);
}

void DisposeInspectorClass(void)
{
  if (InspectorClass)   MUI_DeleteCustomClass(InspectorClass);
  if (DDGroupClass)     MUI_DeleteCustomClass(DDGroupClass);
  if (HelpGroupClass)   MUI_DeleteCustomClass(HelpGroupClass);
  if (NotifyGroupClass) MUI_DeleteCustomClass(NotifyGroupClass);
}
