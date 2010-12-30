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
/*#include <mui/Listtree_mcc.h>*/

#include <stdio.h>
#include <string.h>

#include "/object.h"
#include "/LoadSave/Save.h"

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

enum
{
  RESSOURCE_WINDOW
};

struct ProjectMgrData
{
  Object *ressources; /* list */
  Object *CY_choice;  /* ressources choice */
  char   *savefile;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;
extern Object *ProjectMgr;

extern Object *app;
extern ULONG DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...);

struct MUI_CustomClass *ProjectMgrClass;

/*******************************/
/* ProjectMgr Class Definition */
/*******************************/

ULONG SAVEDS ASM DisplayRessource(REG(a2) char **array, REG(a1) Object *obj)
{
  static char *txt;

  get(obj, MBA_Ressource_Name, &txt);
  *array++ = txt;

  get(obj, MBA_Ressource_Type, &txt);
  *array = txt;

  return(0);
}

ULONG SAVEDS ASM RessourceEdit(REG(a2) Object *list)
{
  APTR res;

  DoMethod(list, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &res);
  if (res)
    {
      DoMethod(res, MBM_Ressource_Edit, TAG_DONE);
    }

  return(0);
}

ULONG SAVEDS ASM NewRessource(REG(a2) Object *prjmgr)
{
  struct ProjectMgrData *data;
  int    restype;
  APTR   ressource, muiwin;

  data = INST_DATA(ProjectMgrClass->mcc_Class, prjmgr);

  get(data->CY_choice, MUIA_Cycle_Active, &restype);
  switch(restype)
    {
    case RESSOURCE_WINDOW:
      if (ressource = NewObject(NULL, "MBWindowClass",
			MUIA_Window_Height, MUIV_Window_Height_Visible(100),
			MUIA_Window_Width, MUIV_Window_Width_Visible(100),
			TAG_DONE))
	{
	  if (muiwin = (APTR)DoMethod(ressource, MBM_Object_CreateEdition, TAG_DONE))
	  {
	    DoMethod(app, OM_ADDMEMBER, muiwin);
	    set(muiwin, MUIA_Window_Open, TRUE);
	    DoMethod(data->ressources, MUIM_List_InsertSingle, ressource, MUIV_List_Insert_Bottom);
	  }
	}
      break;
    }
  return(0);
}

SAVEDS ULONG mNewProjectMgr(Class *cl, Object *obj, struct opSet *msg)
{
  struct ProjectMgrData *data;
  APTR retval = NULL;
  APTR ressources, cychoice, btedit, btdelete, btnew, mnsave, mnload;
  static char *choices[] =
    {
      "Window",
      "AmigaGuide",
      NULL
    };
  static const struct Hook DisplayHook = {{NULL, NULL}, (VOID*)DisplayRessource, NULL, NULL};
  static const struct Hook RessourceEditHook = {{NULL, NULL}, (VOID*)RessourceEdit, NULL, NULL};
  static const struct Hook NewRessourceHook = {{NULL, NULL}, (VOID*)NewRessource, NULL, NULL};

  retval = (APTR)DoSuperNew(cl, obj,
                            MUIA_Window_Title, "ProjectMgr",
			    MUIA_Window_Menustrip, MenustripObject,
			      MUIA_Family_Child, MenuitemObject,
			        MUIA_Menuitem_Title, "Project",
			        MUIA_Family_Child, mnsave = MenuitemObject,
			          MUIA_Menuitem_Title, "Save",
			          MUIA_Menuitem_Shortcut, "S",
			        End,
			        MUIA_Family_Child, mnload = MenuitemObject,
			          MUIA_Menuitem_Title, "Load",
			          MUIA_Menuitem_Shortcut, "L",
			        End,
			      End,
			    End,
                            MUIA_Window_RootObject, VGroup,
			      Child, HGroup,
			        Child, Label("_Ressource"),
			        Child, cychoice = KeyCycle(choices, 'r'),
			        Child, btnew = TextObject,
			          ButtonFrame,
			          MUIA_Weight, 0,
			          MUIA_Text_Contents, "New",
			          MUIA_Text_PreParse, "\33c",
			          MUIA_Text_HiChar  , 'n',
			          MUIA_ControlChar  , 'n',
			          MUIA_InputMode    , MUIV_InputMode_RelVerify,
			          MUIA_Background   , MUII_ButtonBack,
			        End,
			      End,
                              Child, ressources = ListviewObject,
			        MUIA_Listview_List, ListObject,
			          InputListFrame,
			          MUIA_List_DisplayHook, &DisplayHook,
			          MUIA_List_Format, "COL=0 P=\033r, COL=1 P=\033l",
			        End,
                              End,
                              Child, HGroup,
                                Child, btedit = SimpleButton("_Edit"),
                                Child, btdelete = SimpleButton("_Delete"),
                              End,
                            End,
                            TAG_MORE, msg->ops_AttrList);
  if (retval)
    {
      data = INST_DATA(cl, retval);
      data->ressources = ressources;
      data->CY_choice  = cychoice;
      data->savefile = "MUIBEssai";

      DoMethod(ressources, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
	       ressources, 2, MUIM_CallHook, &RessourceEditHook);
      DoMethod(btedit, MUIM_Notify, MUIA_Selected, FALSE,
	       ressources, 2, MUIM_CallHook, &RessourceEditHook);
      DoMethod(mnsave, MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
	       retval, 1, MBM_ProjectMgr_Save, TAG_DONE);
      DoMethod(btnew, MUIM_Notify, MUIA_Selected, FALSE,
	       retval, 2, MUIM_CallHook, &NewRessourceHook);
      DoMethod(retval, MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
	       retval, 3, MUIM_Set, MUIA_Window_Open, FALSE);
    }

  return((ULONG)retval);
}

SAVEDS ULONG mDisposeProjectMgr(Class *cl, Object *obj, Msg msg)
{
  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mAddRessource(Class *cl, Object *obj, struct MBP_AddRessource *msg)
{
  struct ProjectMgrData *data = INST_DATA(cl, obj);

  DoMethod(data->ressources, MUIM_List_InsertSingle,
	   msg->obj, MUIV_List_Insert_Bottom);
  return(0);
}

SAVEDS ULONG mGetProjectMgr(Class *cl, Object *obj, struct opGet * msg)
{
  struct ProjectMgrData *data = INST_DATA(cl, obj);
  ULONG  retval = 0;

  switch(msg->opg_AttrID)
    {
    case MBA_ProjectMgr_Name:
      *(msg->opg_Storage) = (ULONG)data->savefile;
      retval = 1;
      break;
    default:
      retval = DoSuperMethodA(cl, obj, (Msg)msg);
    }

  return(retval);
}

ASM SAVEDS ULONG mSetProjectMgr(REG(a0) Class *cl,
                               REG(a2) Object *obj,
                               REG(a1) struct opSet *msg)
{
  struct ProjectMgrData *data = INST_DATA(cl, obj);
  struct TagItem *ti, *tstate;
  ULONG  retval = 0;

  retval = DoSuperMethodA(cl, obj, (Msg)msg);
  ti = msg->ops_AttrList;
  tstate = ti;
  while(ti = NextTagItem(&tstate))
  {
    switch(ti->ti_Tag)
    {
    case MBA_ProjectMgr_CurrentObject:
      retval = 1L;
      break;
    }
  }
  return(retval);
}

SAVEDS ULONG mSaveProjectMgr(Class *cl, Object *obj, Msg msg)
{
  int i, n;
  struct ProjectMgrData *data = INST_DATA(cl, obj);
  Object *ressource;

  MB_OpenSaveFile("Essai");

  get(data->ressources, MUIA_List_Entries, &n);
  for(i=0;i<n;i++)
    {
      DoMethod(data->ressources, MUIM_List_GetEntry, i, &ressource);
      DoMethod(ressource, MBM_Ressource_Save, TAG_DONE);
    }

  MB_CloseSaveFile();

  return(DoSuperMethodA(cl, obj, msg));
}

ASM SAVEDS ULONG DispatcherProjectMgr(REG(a0) struct IClass *cl,
                                     REG(a2) Object *obj,
                                     REG(a1) Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:         return(mNewProjectMgr(cl, obj, (struct opSet *)msg));
    case OM_GET:         return(mGetProjectMgr(cl, obj, (struct opGet *)msg));
    case OM_DISPOSE:     return(mDisposeProjectMgr(cl, obj, msg));
    case OM_SET:         return(mSetProjectMgr(cl, obj, (struct opSet *)msg));
    case MBM_ProjectMgr_AddRessource: return(mAddRessource(cl, obj, (struct MBP_AddRessource *)msg));
    case MBM_ProjectMgr_Save: return(mSaveProjectMgr(cl, obj, msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitProjectMgrClass(void)
{
  if (ProjectMgrClass = MUI_CreateCustomClass(NULL, SUPERCLASS, NULL, sizeof(struct ProjectMgrData), DispatcherProjectMgr)) return(0);
  return(-1);
}

void DisposeProjectMgrClass(void)
{
  if (ProjectMgrClass) MUI_DeleteCustomClass(ProjectMgrClass);
}
