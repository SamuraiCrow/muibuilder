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

#define WINDOW_TYPE "MBWindowClass"

struct WindowData
{
  char   *title;
  Object *rootgroup;
  ULONG  id; /* ObjectID (MUI window creation) */
  ULONG  activate;
  ULONG  appwindow;
  ULONG  backdrop;
  ULONG  borderless;
  ULONG  closegadget;
  ULONG  depthgadget;
  ULONG  dragbar;
  ULONG  fancydrawing;
  ULONG  needsmouseobject;
  ULONG  nomenus;
  ULONG  sizegadget;
  ULONG  sizeright;
  APTR   CH_activate;
  APTR   CH_appwindow;
  APTR   CH_backdrop;
  APTR   CH_borderless;
  APTR   CH_closegadget;
  APTR   CH_depthgadget;
  APTR   CH_dragbar;
  APTR   CH_needsmouseobject;
  APTR   CH_nomenus;
  APTR   CH_sizegadget;
  APTR   CH_sizeright;
  APTR   STR_title;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;

struct IClass *WindowClass;
extern struct MUI_CustomClass *GUIWindowClass;

extern Object *inspector;

ULONG numWindow = 0;

SAVEDS ULONG mSetWindow(Class *cl, Object *obj, struct opSet * msg)
{
  struct WindowData *data = INST_DATA(cl, obj);
  struct TagItem *ti, *tstate;
  ULONG  retval = 0;

  retval = DoSuperMethodA(cl, obj, (Msg)msg);
  ti = msg->ops_AttrList;
  tstate = ti;
  while(ti = NextTagItem(&tstate))
  {
    switch(ti->ti_Tag)
    {
    case MBA_Window_RootGroup:
      data->rootgroup = (Object*)ti->ti_Data;
      retval = 1L;
      break;
    }
  }
  return(retval);
}

SAVEDS ULONG mGetWindow(Class *cl, Object *obj, struct opGet * msg)
{
  struct WindowData *data = INST_DATA(cl, obj);
  ULONG   retval = 0;
  char   *txt;

  switch(msg->opg_AttrID)
    {
    case MBA_Window_Title:
      *(msg->opg_Storage) = (ULONG)data->title;
      retval = 1;
      break;
    case MBA_Window_RootGroup:
      *(msg->opg_Storage) = (ULONG)data->rootgroup;
      retval = 1;
      break;
    case MBA_Ressource_Name:
      get(obj, MBA_Object_Label, &txt);
      *(msg->opg_Storage) = (ULONG)txt;
      retval = 1;
      break;
    case MBA_Ressource_Type:
      *(msg->opg_Storage) = (ULONG)"Window";
      break;
    case MBA_Object_Type:
      *(msg->opg_Storage) = (ULONG) MBV_Object_Type_Window;
      retval = 1;
      break;
    default:
      retval = DoSuperMethodA(cl, obj, (Msg)msg);
    }

  return(retval);
}

SAVEDS ULONG mNewWindow(Class *cl, Object *obj, struct opSet *msg)
{
  struct WindowData *data = NULL;
  APTR retval = NULL;
  char *aux;

  if (retval = (APTR)DoSuperMethodA(cl, obj, (Msg) msg))
    {
      data = INST_DATA(cl, retval);
      if (data->title = AllocVec(10, MEMF_PUBLIC|MEMF_CLEAR))
        sprintf(data->title, "Window%d", numWindow);
      if (aux = AllocVec(12, MEMF_PUBLIC|MEMF_CLEAR))
	sprintf(aux, "wiWindow%d", numWindow);
      set(retval, MBA_Object_Label, aux);
      data->id = 1000+numWindow++;
      data->activate = TRUE;
      data->appwindow = FALSE;
      data->backdrop = FALSE;
      data->borderless = FALSE;
      data->closegadget = TRUE;
      data->depthgadget = TRUE;
      data->dragbar = TRUE;
      data->fancydrawing = FALSE;
      data->needsmouseobject = FALSE;
      data->nomenus = FALSE;
      data->sizegadget = TRUE;
      data->sizeright = FALSE;
      data->rootgroup = NewObject(NULL, "MBGroupClass", TAG_DONE);
    }
  return((ULONG)retval);
}

ULONG mDisposeWindow(Class *cl, Object *obj, Msg msg)
{
  struct WindowData *data = INST_DATA(cl, obj);

  if (data->title) FreeVec(data->title);

  if (data->rootgroup) DisposeObject(data->rootgroup);
  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS APTR mCreateTestWindow(Class *cl, Object *obj, Msg msg)
{
  struct WindowData *data = INST_DATA(cl, obj);
  APTR retval;
  char *aux;
  APTR grp;

  get(obj, MBA_Window_Title, &aux);
  retval = WindowObject,
    MUIA_Window_Title, aux,
    MUIA_Window_RootObject, grp = (APTR)DoMethod(data->rootgroup, MBM_Object_CreateTest, TAG_DONE),
    End;

  if (grp) set(grp, MUIA_Draggable, FALSE);

  return(retval);
}

SAVEDS APTR mCreateEditionWindow(Class *cl, Object *obj, Msg msg)
{
  struct WindowData *data = INST_DATA(cl, obj);
  APTR retval, grp;

  grp = (APTR)DoMethod(data->rootgroup,
		 MBM_Object_CreateEdition,
		 TAG_DONE
		 );

  retval = NewObject(GUIWindowClass->mcc_Class, NULL,
		     MBA_GUIObject_Object, obj,
		     MUIA_Window_ID, data->id,
		     MUIA_Window_Title, data->title,
		     MUIA_Window_Activate, data->activate,
		     MUIA_Window_Backdrop, data->backdrop,
		     MUIA_Window_Borderless, data->borderless,
		     MUIA_Window_CloseGadget, data->closegadget,
		     MUIA_Window_DepthGadget, data->depthgadget,
		     MUIA_Window_DragBar, data->dragbar,
		     MUIA_Window_SizeGadget, data->sizegadget,
		     MUIA_Window_SizeRight, data->sizeright,
		     MUIA_Window_RootObject, grp, TAG_DONE
		     );

  set(obj, MBA_Object_MUIObject, retval);

  return(retval);
}

SAVEDS APTR mGetImageWindow(Class *cl, Object *obj, Msg msg)
{
  return(SimpleButton("Window"));
}

SAVEDS ULONG mEditWindowRessource(Class *cl, Object *obj)
{
  APTR win;

  get(obj, MBA_Object_MUIObject, &win);

  set(win, MUIA_Window_Open, TRUE);
  set(inspector, MBA_Inspector_CurrentObject, obj);
  
  return(0);
}

SAVEDS ULONG mAttributesEditWindow(Class *cl, Object *obj, struct MBP_AttributesEdit *msg)
{
  struct WindowData *data = INST_DATA(cl, obj);
  APTR   aux, CH_activate, CH_appwindow, CH_backdrop, CH_borderless;
  APTR   CH_closegadget, CH_depthgadget, CH_dragbar, CH_needsmouseobject;
  APTR   CH_nomenus, CH_sizegadget, CH_sizeright, STR_title;
  char   **registertitles = msg->titles;

  registertitles[msg->number] = "Window";

  aux = VGroup,
    Child, HVSpace,
    Child, ColGroup(4),
      Child, Label("_Activate"),
      Child, CH_activate = KeyCheckMark(data->activate, 'a'),
      Child, Label("A_ppWindow"),
      Child, CH_appwindow = KeyCheckMark(data->appwindow, 'p'),
      Child, Label("_Backdrop"),
      Child, CH_backdrop = KeyCheckMark(data->backdrop, 'b'),
      Child, Label("B_orderless"),
      Child, CH_borderless = KeyCheckMark(data->borderless, 'o'),
      Child, Label("_Close Gadget"),
      Child, CH_closegadget = KeyCheckMark(data->closegadget, 'c'),
      Child, Label("_Depth Gadget"),
      Child, CH_depthgadget = KeyCheckMark(data->depthgadget, 'd'),
      Child, Label("Dr_ag Bar"),
      Child, CH_dragbar = KeyCheckMark(data->dragbar, 'a'),
      Child, Label("_Needs Mouse"),
      Child, CH_needsmouseobject = KeyCheckMark(data->needsmouseobject, 'n'),
      Child, Label("No _Menus"),
      Child, CH_nomenus = KeyCheckMark(data->nomenus, 'm'),
      Child, Label("Size _Gadget"),
      Child, CH_sizegadget = KeyCheckMark(data->sizegadget, 'g'),
      Child, Label("_Size Right"),
      Child, CH_sizeright = KeyCheckMark(data->sizeright, 's'),
      Child, HVSpace,
      Child, HVSpace,
    End,
    Child, HGroup,
      Child, Label("_Title"),
      Child, STR_title = KeyString(data->title, 80, 't'),
    End,
    Child, HVSpace,
  End;

  if (aux)
    {
      data->CH_activate = CH_activate;
      data->CH_appwindow = CH_appwindow;
      data->CH_backdrop = CH_backdrop;
      data->CH_borderless = CH_borderless;
      data->CH_closegadget = CH_closegadget;
      data->CH_depthgadget = CH_depthgadget;
      data->CH_dragbar = CH_dragbar;
      data->CH_needsmouseobject = CH_needsmouseobject;
      data->CH_nomenus = CH_nomenus;
      data->CH_sizegadget = CH_sizegadget;
      data->CH_sizeright = CH_sizeright;
      data->STR_title = STR_title;

      DoMethod(msg->group, OM_ADDMEMBER, aux);
      DoSuperMethod(cl, obj, MBM_Object_AttributesEdit,
                    msg->group,
                    msg->number+1,
                    registertitles);
    }
  else return(0);

  return(1);
}

SAVEDS ULONG mAttributesApplyWindow(Class *cl, Object *obj, Msg msg)
{
  struct WindowData *data = INST_DATA(cl, obj);
  char * straux;

  /* Record Window Title */
  get(data->STR_title, MUIA_String_Contents, &straux);

  if (data->title) FreeVec(data->title);
  if (data->title = AllocVec(strlen(straux)+1, MEMF_PUBLIC|MEMF_CLEAR))
    strcpy(data->title, straux);

  get(data->CH_activate, MUIA_Selected, &data->activate);
  get(data->CH_appwindow, MUIA_Selected, &data->appwindow);
  get(data->CH_backdrop, MUIA_Selected, &data->backdrop);
  get(data->CH_borderless, MUIA_Selected, &data->borderless);
  get(data->CH_closegadget, MUIA_Selected, &data->closegadget);
  get(data->CH_depthgadget, MUIA_Selected, &data->depthgadget);
  get(data->CH_dragbar, MUIA_Selected, &data->dragbar);
  get(data->CH_nomenus, MUIA_Selected, &data->nomenus);
  get(data->CH_sizegadget, MUIA_Selected, &data->sizegadget);
  get(data->CH_sizeright, MUIA_Selected, &data->sizeright);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mSaveWindow(Class *cl, Object *obj, Msg msg)
{
  struct WindowData *data = INST_DATA(cl, obj);
  char *aux;

  /* Save Root group first */
  DoMethod(data->rootgroup, MBM_Object_Save, TAG_DONE);
  /* Then save window object */
  get(obj, MBA_Object_Label, &aux);
  MB_BeginSaveObject(WINDOW_TYPE, aux);
  DoMethod(obj, MBM_Object_SaveAttributes, TAG_DONE);
  MB_EndSaveObject();

  return(0);
}

SAVEDS ULONG mSaveAttributesWindow(Class *cl, Object *obj, Msg msg)
{
  struct WindowData *data = INST_DATA(cl, obj);
  char   *aux;

  MB_SaveAttribute("MUIA_Window_Activate",         TYPE_BOOL,   data->activate);
  MB_SaveAttribute("MUIA_Window_AppWindow",        TYPE_BOOL,   data->appwindow);
  MB_SaveAttribute("MUIA_Window_Backdrop",         TYPE_BOOL,   data->backdrop);
  MB_SaveAttribute("MUIA_Window_CloseGadget",      TYPE_BOOL,   data->closegadget);
  MB_SaveAttribute("MUIA_Window_DepthGadget",      TYPE_BOOL,   data->depthgadget);
  MB_SaveAttribute("MUIA_Window_DragBar",          TYPE_BOOL,   data->dragbar);
  MB_SaveAttribute("MUIA_Window_NeedsMouseObject", TYPE_BOOL,   data->needsmouseobject);
  MB_SaveAttribute("MUIA_Window_NoMenus",          TYPE_BOOL,   data->nomenus);
  MB_SaveAttribute("MUIA_Window_SizeGadget",       TYPE_BOOL,   data->sizegadget);
  MB_SaveAttribute("MUIA_Window_SizeRight",        TYPE_BOOL,   data->sizeright);
  MB_SaveAttribute("MUIA_Window_Title",            TYPE_STRING, (ULONG)data->title);
  get(data->rootgroup, MBA_Object_Label, &aux);
  MB_SaveAttribute("MUIA_Window_RootObject",       TYPE_OBJ,    (ULONG)aux);

  return(0);
}

SAVEDS ULONG DispatcherWindow(struct IClass *cl,
			    Object *obj,
			    Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
  case OM_NEW:                    return(mNewWindow(cl, obj, (struct opSet *)msg));
  case OM_SET:                    return(mSetWindow(cl, obj, (struct opSet *)msg));
  case OM_GET:                    return(mGetWindow(cl, obj, (struct opGet *)msg));
  case OM_DISPOSE:                return(mDisposeWindow(cl, obj, msg));
  case MBM_Object_CreateTest:     return((ULONG)mCreateTestWindow(cl, obj, msg));
  case MBM_Object_CreateEdition:  return((ULONG)mCreateEditionWindow(cl, obj, msg));
  case MBM_Object_GetImage:       return((ULONG)mGetImageWindow(cl, obj, msg));
  case MBM_Ressource_Edit:        return(mEditWindowRessource(cl, obj));
  case MBM_Object_AttributesEdit: return((ULONG)mAttributesEditWindow(cl, obj, (struct MBP_AttributesEdit *)msg));
  case MBM_Object_AttributesApply:return(mAttributesApplyWindow(cl, obj, msg));
  case MBM_Object_SaveAttributes: return(mSaveAttributesWindow(cl, obj, msg));
  case MBM_Object_Save:           return(mSaveWindow(cl, obj, msg));
  case MBM_Ressource_Save:        return(mSaveWindow(cl, obj, msg));
  default:
    retval = (APTR)DoSuperMethodA(cl, obj, msg);
    break;
  }

  return((ULONG)retval);
}

void InitWindowClass(void)
{
  extern ULONG HookEntry();
  extern ULONG InitGUIWindowClass(void);

  if (WindowClass = MakeClass("MBWindowClass", "MBBaseClass", NULL, sizeof(struct WindowData), 0))
    {
      WindowClass->cl_Dispatcher.h_Entry = HookEntry;
      WindowClass->cl_Dispatcher.h_SubEntry = DispatcherWindow;
      AddClass(WindowClass);
    }
  InitGUIWindowClass();
}

void DisposeWindowClass(void)
{
  extern void DisposeGUIWindowClass(void);

  RemoveClass(WindowClass);
  FreeClass(WindowClass);
  DisposeGUIWindowClass();
}
