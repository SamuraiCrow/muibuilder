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
#include <libraries/gadtools.h>
#include <exec/memory.h>
/*#include <mui/Listtree_mcc.h>*/

#include <stdio.h>
#include <string.h>

#include "../object.h"
#include "../LoadSave/Save.h"
#include "SDI_compiler.h"
#include "SDI_hook.h"

#define SUPERCLASS MUIC_Window

struct MUIBMainData
{
  Object *projects; /* list */
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;
extern Object *MUIBMain;
extern Object *app;
extern Object *inspector;
extern Object *palette, *aboutmui;
extern struct MUI_CustomClass *ProjectMgrClass;
extern ULONG DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...);

struct MUI_CustomClass *MUIBMainClass;
int    windowid = 10000;

/******************************/
/* MUIBMain Class Definition */
/******************************/

HOOKPROTONH(DisplayProjects, ULONG, char **array, Object *obj)
//ULONG SAVEDS ASM DisplayProjects(REG(a2) char **array, REG(a1) Object *obj)
{
  char *txt;

  get(obj, MBA_ProjectMgr_Name, &txt);

  *array = txt;

  return(0);
}

HOOKPROTONHNP(ProjectEdit, ULONG, Object *list)
//ULONG SAVEDS ASM ProjectEdit(REG(a2) Object *list)
{
  APTR project;

  DoMethod(list, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &project);
  if (project)
    {
      set(project, MUIA_Window_Open, TRUE);
    }

  return(0);
}

HOOKPROTONHNP(AboutMUI, ULONG, Object *obj)
//ULONG SAVEDS ASM AboutMUI(REG(a2) Object *obj)
{
  if (!aboutmui)
    {
      aboutmui = AboutmuiObject,
        MUIA_Window_RefWindow, obj,
        MUIA_Aboutmui_Application, app,
      End;
    }
  else set(aboutmui, MUIA_Window_Open, TRUE);

  return(0);
}

HOOKPROTONHNP(AboutMUIBuilder, ULONG, Object *obj)
//ULONG SAVEDS ASM AboutMUIBuilder(REG(a2) Object *obj)
{
  MUI_Request(app, obj, 0, NULL, "Ok", "\033cMUIBuilder 3.0\n\nMUI Application Builder\n\nBy\nEric Totel\nand\nJérome Souquières", NULL);

  return(0);
}

HOOKPROTONHNP(LoadProject, ULONG, Object *list)
//ULONG SAVEDS ASM LoadProject(REG(a2) Object *list)
{
  Object *project;

  DoMethod(list, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &project);
  if (project) DoMethod(project, MBM_ProjectMgr_Save, TAG_DONE);
  return(0);
}

SAVEDS ULONG mGetMUIBMain(Class *cl, Object *obj, struct opGet * msg)
{
  struct MUIBMainData *data = INST_DATA(cl, obj);
  APTR   project;
  ULONG  retval = 0;

  switch(msg->opg_AttrID)
    {
    case MBA_MUIBMain_CurrentProject:
      DoMethod(data->projects, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &project);
      *(msg->opg_Storage) = (ULONG)project;
      retval = 1;
      break;
    default:
      retval = DoSuperMethodA(cl, obj, (Msg)msg);
    }

  return(retval);
}

SAVEDS ULONG mNewMUIBMain(Class *cl, Object *obj, struct opSet *msg)
{
  struct MUIBMainData *data;
  APTR retval = NULL;
  APTR projects, btedit, btdelete, btnew, mnsave, mnsaveas, mnload, mnpalette, mninspector;
  APTR mnaboutmui, mnaboutmuib, mnquit;
  MakeStaticHook(DisplayHook, DisplayProjects);
  MakeStaticHook(ProjectEditHook, ProjectEdit);
  MakeStaticHook(AboutMUIHook, AboutMUI);
  MakeStaticHook(AboutMUIBuilderHook, AboutMUIBuilder);
  MakeStaticHook(LoadProjectHook, LoadProject);

  retval = (APTR)DoSuperNew(cl, obj,
                            MUIA_Window_Title, "MUIBuilder",
                            MUIA_Window_Menustrip, MenustripObject,
                              MUIA_Family_Child, MenuitemObject,
                                MUIA_Menuitem_Title, "Project",
                                MUIA_Family_Child, mnsave = MenuitemObject,
                                  MUIA_Menuitem_Title, "Save",
                                  MUIA_Menuitem_Shortcut, "S",
                                End,
			        MUIA_Family_Child, mnsaveas = MenuitemObject,
                                  MUIA_Menuitem_Title, "Save As",
                                  MUIA_Menuitem_Shortcut, "A",
                                End,
                                MUIA_Family_Child, mnload = MenuitemObject,
                                  MUIA_Menuitem_Title, "Open",
                                  MUIA_Menuitem_Shortcut, "O",
                                End,
			        MUIA_Family_Child, MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0),
			        MUIA_Family_Child, mnaboutmuib = MenuitemObject,
                                  MUIA_Menuitem_Title, "About",
                                  MUIA_Menuitem_Shortcut, "?",
                                End,
			        MUIA_Family_Child, mnaboutmui = MenuitemObject,
                                  MUIA_Menuitem_Title, "About MUI",
                                  MUIA_Menuitem_Shortcut, "A",
                                End,
			        MUIA_Family_Child, MUI_MakeObject(MUIO_Menuitem, NM_BARLABEL, 0, 0, 0),
			        MUIA_Family_Child, mnquit = MenuitemObject,
                                  MUIA_Menuitem_Title, "Quit",
                                  MUIA_Menuitem_Shortcut, "Q",
                                End,
                              End,
			      MUIA_Family_Child, MenuitemObject,
			        MUIA_Menuitem_Title, "Windows",
                                MUIA_Family_Child, mnpalette = MenuitemObject,
                                  MUIA_Menuitem_Title, "Palette",
                                  MUIA_Menuitem_Shortcut, "P",
                                End,
			        MUIA_Family_Child, mninspector = MenuitemObject,
                                  MUIA_Menuitem_Title, "Inspector",
                                  MUIA_Menuitem_Shortcut, "I",
                                End,
			      End,
                            End,
                            MUIA_Window_RootObject, VGroup,
                              Child, projects = ListviewObject,
                                MUIA_FrameTitle, "Projects",
                                MUIA_Listview_List, ListObject,
                                  InputListFrame,
                                  MUIA_List_DisplayHook, &DisplayHook,
                                End,
                              End,
                              Child, HGroup,
			        Child, btnew = SimpleButton("_New"),
                                Child, btedit = SimpleButton("_Edit"),
                                Child, btdelete = SimpleButton("_Delete"),
                              End,
                            End,
                            TAG_MORE, msg->ops_AttrList);
  if (retval)
    {
      data = INST_DATA(cl, retval);
      data->projects = projects;

      /* Quit application when closing main window */
      DoMethod(retval, MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
	       app, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);
      /* Edit a project */
      DoMethod(projects, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
               projects, 2, MUIM_CallHook, &ProjectEditHook);
      DoMethod(btedit, MUIM_Notify, MUIA_Selected, FALSE,
               projects, 2, MUIM_CallHook, &ProjectEditHook);

      /* Menus Notifications */
      DoMethod(mnsave, MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
               retval, 1, MBM_MUIBMain_Save, TAG_DONE);
      DoMethod(mnsave, MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
               retval, 2, MUIM_CallHook, &LoadProjectHook);
      DoMethod(btnew, MUIM_Notify, MUIA_Selected, FALSE,
               retval, 1, MBM_MUIBMain_AddProject, TAG_DONE);
      DoMethod(mnpalette, MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
	       palette, 3, MUIM_Set, MUIA_Window_Open, TRUE);
      DoMethod(mninspector, MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
	       inspector, 3, MUIM_Set, MUIA_Window_Open, TRUE);
      DoMethod(mnaboutmui, MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
	       retval, 2, MUIM_CallHook, &AboutMUIHook);
      DoMethod(mnaboutmuib, MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
	       retval, 2, MUIM_CallHook, &AboutMUIBuilderHook);
      DoMethod(mnquit, MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
	       app, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);

    }

  return((ULONG)retval);
}

SAVEDS ULONG mDisposeMUIBMain(Class *cl, Object *obj, Msg msg)
{
  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mAddProject(Class *cl, Object *obj, Msg msg)
{
  struct MUIBMainData *data = INST_DATA(cl, obj);

  APTR project;

  if (project = NewObject(ProjectMgrClass->mcc_Class, NULL,
			  MUIA_Window_ID, windowid++,
                          TAG_DONE))
    {
      DoMethod(app, OM_ADDMEMBER, project);
      DoMethod(data->projects, MUIM_List_InsertSingle, project, MUIV_List_Insert_Bottom);
      set(data->projects, MUIA_List_Active, MUIV_List_Active_Bottom);
      set(project, MUIA_Window_Open, TRUE);
    }

  return(0);
}

SAVEDS ULONG mSaveMUIBMain(Class *cl, Object *obj, Msg msg)
{
  struct MUIBMainData *data = INST_DATA(cl, obj);
  Object *project;

  DoMethod(data->projects, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &project);
  if (project) DoMethod(project, MBM_ProjectMgr_Save, TAG_DONE);

  return(0);
}

DISPATCHER(DispatcherMUIBMain)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:         return(mNewMUIBMain(cl, obj, (struct opSet *)msg));
    case OM_GET:         return(mGetMUIBMain(cl, obj, (struct opGet *)msg));
    case OM_DISPOSE:     return(mDisposeMUIBMain(cl, obj, msg));
    case MBM_MUIBMain_AddProject: return(mAddProject(cl, obj, msg));
    case MBM_MUIBMain_Save: return(mSaveMUIBMain(cl, obj, msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

ULONG InitMUIBMainClass(void)
{
  if (MUIBMainClass = MUI_CreateCustomClass(NULL, SUPERCLASS, NULL, sizeof(struct MUIBMainData), DispatcherMUIBMain)) return(0);
  return(-1);
}

void DisposeMUIBMainClass(void)
{
  if (MUIBMainClass) MUI_DeleteCustomClass(MUIBMainClass);
}
