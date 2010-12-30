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

#define AREA_TYPE "AreaClass"

struct AreaData
{
  ULONG background;
  ULONG frame;
  ULONG fonts;
  ULONG disabled;
  ULONG show;
  ULONG horizdisappear;
  ULONG vertdisappear;
  ULONG phantomhoriz;
  char  *frametitle;
  ULONG selected;
  ULONG inputmode;
  ULONG showselstate;
  ULONG draggable;
  ULONG droppable;
  ULONG cyclechain;
  char  controlchar;
  ULONG horizweight;
  ULONG vertweight;
  char  *shorthelp;
  APTR  CY_background;
  APTR  CY_frame;
  APTR  CY_fonts;
  APTR  CH_disabled;
  APTR  CH_show;
  APTR  CH_horizdisappear;
  APTR  CH_vertdisappear;
  APTR  CH_phantomhoriz;
  APTR  STR_frametitle;
  APTR  CY_selected;
  APTR  CY_inputmode;
  APTR  CH_showselstate;
  APTR  CH_draggable;
  APTR  CH_droppable;
  APTR  SL_cyclechain;
  APTR  STR_controlchar;
  APTR  SL_horizweight;
  APTR  SL_vertweight;
  APTR  GR_shorthelp;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;
extern struct MUI_CustomClass *HelpGroupClass;

struct IClass *AreaClass;

ULONG BackgroundsValues[] =
  {
    MUII_WindowBack,
    MUII_RequesterBack,
    MUII_ButtonBack,
    MUII_ListBack,
    MUII_TextBack,
    MUII_PropBack,
    MUII_SelectedBack,
    MUII_PopupBack,
    MUII_RegisterBack,
    MUII_GroupBack,
    MUII_SliderBack,
    MUII_PageBack,
    MUII_ReadListBack
  };

ULONG FramesValues[] =
  {
    MUIV_Frame_None,
    MUIV_Frame_Button,
    MUIV_Frame_ImageButton,
    MUIV_Frame_Text,
    MUIV_Frame_String,
    MUIV_Frame_ReadList,
    MUIV_Frame_InputList,
    MUIV_Frame_Prop,
    MUIV_Frame_Slider,
    MUIV_Frame_Gauge,
    MUIV_Frame_Virtual,
    MUIV_Frame_Group
  };

ULONG FontsValues[] =
{
  MUIV_Font_Inherit,
  MUIV_Font_Normal,
  MUIV_Font_List,
  MUIV_Font_Tiny,
  MUIV_Font_Fixed,
  MUIV_Font_Title,
  MUIV_Font_Big,
  MUIV_Font_Button
};

SAVEDS ULONG mSetArea(Class *cl, Object *obj, struct opSet * msg)
{
  ULONG  retval = 0;

  retval = DoSuperMethodA(cl, obj, (Msg)msg);
  return(retval);
}

SAVEDS ULONG mGetArea(Class *cl, Object *obj, struct opGet * msg)
{
  struct AreaData *data = INST_DATA(cl, obj);
  APTR   retval = (APTR)TRUE;

  switch(msg->opg_AttrID)
    {
    case MBA_Area_Show:
      *(msg->opg_Storage) = (ULONG) data->show;
      break;
    case MBA_Area_Disabled:
      *(msg->opg_Storage) = (ULONG) data->disabled;
      break;
    case MBA_Area_InputMode:
      *(msg->opg_Storage) = (ULONG) data->inputmode;
      break;
    case MBA_Area_PhantomHoriz:
      *(msg->opg_Storage) = (ULONG) data->phantomhoriz;
      break;
    case MBA_Area_Background:
      *(msg->opg_Storage) = (ULONG) data->background;
      break;
    case MBA_Area_Frame:
      *(msg->opg_Storage) = (ULONG) data->frame;
      break;
    case MBA_Area_FrameTitle:
      *(msg->opg_Storage) = (ULONG) data->frametitle;
      break;
    case MBA_Area_Font:
      *(msg->opg_Storage) = (ULONG) data->fonts;
      break;
    case MBA_Area_Selected:
      *(msg->opg_Storage) = (ULONG) data->selected;
      break;
    case MBA_Area_ShowSelState:
      *(msg->opg_Storage) = (ULONG) data->showselstate;
      break;
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, (Msg) msg);
      break;
    }
  return((ULONG)retval);
}

SAVEDS ULONG mNewArea(Class *cl, Object *obj, struct opSet *msg)
{
  struct AreaData *data = NULL;
  APTR retval = NULL;

  if (retval = (APTR)DoSuperMethodA(cl, obj, (Msg) msg))
    {
      data = INST_DATA(cl, retval);
      data->inputmode     = (ULONG) GetTagData(MBA_Area_InputMode, 0, msg->ops_AttrList);
      data->background    = (ULONG) GetTagData(MBA_Area_Background, 0, msg->ops_AttrList);
      data->frame         = (ULONG) GetTagData(MBA_Area_Frame, 0, msg->ops_AttrList);
      data->selected      = (ULONG) GetTagData(MBA_Area_Selected, 0, msg->ops_AttrList);
      data->showselstate  = (ULONG) GetTagData(MBA_Area_ShowSelState, 0, msg->ops_AttrList);
      data->shorthelp     = NULL;
      data->frametitle    = NULL;
      data->fonts         = 0;
      data->disabled      = FALSE;
      data->show          = TRUE;
      data->horizdisappear= FALSE;
      data->vertdisappear = FALSE;
      data->phantomhoriz  = FALSE;
      data->draggable     = FALSE;
      data->droppable     = TRUE;
      data->cyclechain    = 1L;
      data->controlchar   = 0;
      data->horizweight   = 100;
      data->vertweight    = 100;
    }
  return((ULONG)retval);
}

ULONG mDisposeArea(Class *cl, Object *obj, Msg msg)
{
  struct AreaData *data = INST_DATA(cl, obj);

  if (data->frametitle) FreeVec(data->frametitle);

  if (data->shorthelp) FreeVec(data->shorthelp);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mAttributesEditArea(Class *cl, Object *obj, struct MBP_AttributesEdit *msg)
{
  struct AreaData *data = INST_DATA(cl, obj);
  APTR   aux;
  char   **registertitles = msg->titles;
  static char *AreaRegister[] =
  {
    "Visual",
    "Control",
    "Size",
    "ShortHelp",
    NULL
  };
  static char *Backgrounds[] =
  {
    "Window",
    "Requester",
    "Button",
    "List",
    "Text",
    "Proportionnal",
    "Selected",
    "Popup",
    "Register",
    "Group",
    "Slider",
    "Page",
    "ReadListBack",
    NULL
  };
  static char *Frames[] =
  {
    "None",
    "Button",
    "Image Button",
    "Text",
    "String",
    "ReadList",
    "InputList",
    "Prop",
    "Slider",
    "Gauge",
    "Virtual",
    "Group",
    NULL
  };
  static char *Fonts[] =
  {
    "Inherit",
    "Normal",
    "List",
    "Tiny",
    "Fixed",
    "Title",
    "Big",
    "Button",
    NULL
  };
  static char *SelectedModes[] =
  {
    "RelVerify",
    "Immediate",
    "Toggle",
    NULL
  };
  static char *InputModes[] =
  {
    "None",
    "RelVerify",
    "Immediate",
    "Toggle",
    NULL
  };

  registertitles[msg->number] = "Area";

  aux = RegisterGroup(AreaRegister),
    Child, VGroup,
      Child, ColGroup(2),
        Child, Label("_Background"), Child, data->CY_background = KeyCycle(Backgrounds, 'b'),
        Child, Label("_Frame"), Child, data->CY_frame = KeyCycle(Frames, 'f'),
        Child, Label("F_ont"), Child, data->CY_fonts = KeyCycle(Fonts, 'o'),        
      End,
      Child, ColGroup(4),
        Child, Label("_Disabled"), Child, data->CH_disabled = KeyCheckMark(data->disabled, 'd'),
        Child, Label("_Show me"), Child, data->CH_show = KeyCheckMark(data->show, 's'),
        Child, Label("_HorizDisappear"), Child, data->CH_horizdisappear = KeyCheckMark(data->horizdisappear, 'h'),
        Child, Label("_VertDisappear"), Child, data->CH_vertdisappear = KeyCheckMark(data->vertdisappear, 'v'),
        Child, Label("_PhantomHoriz"), Child, data->CH_phantomhoriz = KeyCheckMark(data->phantomhoriz, 'p'),        
      End,
      Child, HGroup,
        Child, Label("Frame _Title"),
        Child, data->STR_frametitle = KeyString("", 80, 't'),
      End,
    End,
    Child, ColGroup(2),
      Child, Label("S_elected"), Child, data->CY_selected = KeyCycle(SelectedModes, 'e'),
      Child, Label("_InputMode"), Child, data->CY_inputmode = KeyCycle(InputModes, 'i'),
      Child, Label("Sho_wSelState"), Child, HGroup, Child, data->CH_showselstate = KeyCheckMark(data->showselstate, 'w'), Child, HVSpace, End,
      Child, Label("_Draggable"), Child, HGroup, Child, data->CH_draggable = KeyCheckMark(data->draggable, 'd'), Child, HVSpace, End,
      Child, Label("Dro_ppable"), Child, HGroup, Child, data->CH_droppable = KeyCheckMark(data->droppable, 'p'), Child, HVSpace, End,
      Child, Label("C_ycleChain"), Child, data->SL_cyclechain = KeySlider(0, 100, data->cyclechain,'y'),
      Child, Label("_ControlChar"), Child, data->STR_controlchar = KeyString("", 2, 'c'),
    End,
    Child, ColGroup(2),
      Child, Label("_HorizWeight"), Child, data->SL_horizweight = KeySlider(0, 100, data->horizweight, 'h'),
      Child, Label("_VertWeight"), Child, data->SL_vertweight = KeySlider(0, 100, data->vertweight, 'v'),
    End,
    Child, data->GR_shorthelp = NewObject(HelpGroupClass->mcc_Class, NULL, TAG_DONE),
  End;

  if (aux)
    {
      set(data->CY_background, MUIA_Cycle_Active, data->background);
      set(data->CY_frame, MUIA_Cycle_Active, data->frame);
      set(data->CY_fonts, MUIA_Cycle_Active, data->fonts);
      if (data->frametitle) set(data->STR_frametitle, MUIA_String_Contents, data->frametitle);
      if (data->shorthelp)  set(data->GR_shorthelp, TEXTFIELD_Text, data->shorthelp);
      set(data->CY_selected, MUIA_Cycle_Active, data->selected);
      set(data->CY_inputmode, MUIA_Cycle_Active, data->inputmode);
      if (data->controlchar) set(data->STR_controlchar, MUIA_String_Contents, &data->controlchar);

      DoMethod(msg->group, OM_ADDMEMBER, aux);
      DoSuperMethod(cl, obj, MBM_Object_AttributesEdit,
                    msg->group,
                    msg->number+1,
                    registertitles);
    }
  else return(0);

  return(1);
}

SAVEDS APTR mCreateEditionArea(Class *cl, Object *obj, struct MBP_CreateEdition *msg)
{
  struct AreaData *data = INST_DATA(cl, obj);

  return(NewObject(msg->cl, NULL,
		   MUIA_Background, BackgroundsValues[data->background],
		   MUIA_Frame, FramesValues[data->frame],
		   MUIA_Font, FontsValues[data->fonts],
		   MUIA_Disabled, data->disabled,
		   MUIA_ShowMe, data->show,
		   MUIA_HorizDisappear, data->horizdisappear,
		   MUIA_VertDisappear, data->vertdisappear,
		   MUIA_Selected, data->selected,
		   MUIA_InputMode, data->inputmode,
		   MUIA_ShowSelState, data->showselstate,
		   MUIA_CycleChain, data->cyclechain,
		   MUIA_HorizWeight, data->horizweight,
		   MUIA_VertWeight, data->vertweight,
		   MUIA_FrameTitle, data->frametitle,
		   MUIA_ControlChar, data->controlchar,
		   /* Fixed attributes for edition */
		   MUIA_Draggable, TRUE,
		   MUIA_ShortHelp, data->shorthelp,
		   /*MUIA_Dropable, FALSE,*/
		   TAG_MORE, &msg->tags
		   )
	 );
}

ULONG mAttributesApplyArea(Class *cl, Object *obj, Msg msg)
{
  struct AreaData *data = INST_DATA(cl, obj);
  char *txt, *newtxt;
  int size;

  get(data->CY_background, MUIA_Cycle_Active, &data->background);
  get(data->CY_frame, MUIA_Cycle_Active, &data->frame);
  get(data->CY_fonts, MUIA_Cycle_Active, &data->fonts);
  get(data->CH_disabled, MUIA_Selected, &data->disabled);
  get(data->CH_show, MUIA_Selected, &data->show);
  get(data->CH_horizdisappear, MUIA_Selected, &data->horizdisappear);
  get(data->CH_vertdisappear, MUIA_Selected, &data->vertdisappear);
  get(data->CH_phantomhoriz, MUIA_Selected, &data->phantomhoriz);
  get(data->CY_selected, MUIA_Cycle_Active, &data->selected);
  get(data->CY_inputmode, MUIA_Cycle_Active, &data->inputmode);
  get(data->CH_showselstate, MUIA_Cycle_Active, &data->showselstate);
  get(data->CH_draggable, MUIA_Selected, &data->draggable);
  get(data->CH_droppable, MUIA_Selected, &data->droppable);
  get(data->SL_cyclechain, MUIA_Numeric_Value, &data->cyclechain);
  get(data->SL_horizweight, MUIA_Numeric_Value, &data->horizweight);
  get(data->SL_vertweight, MUIA_Numeric_Value, &data->vertweight);

  get(data->STR_controlchar, MUIA_String_Contents, &txt);
  if (txt) data->controlchar = txt[0];

  get(data->STR_frametitle, MUIA_String_Contents, &txt);
  if (data->frametitle) FreeVec(data->frametitle);
  size = strlen(txt);
  if (size>0)
    {
      if (newtxt = AllocVec(size+1, MEMF_PUBLIC|MEMF_CLEAR))
	{
	  memcpy(newtxt, txt, size);
	}
    }
  else newtxt = NULL;
  data->frametitle = newtxt;

  if (data->shorthelp) FreeVec(data->shorthelp);

  set(data->GR_shorthelp, TEXTFIELD_ReadOnly, TRUE);
  get(data->GR_shorthelp, TEXTFIELD_Text, &txt);
  get(data->GR_shorthelp, TEXTFIELD_Size, &size);
  if (newtxt = AllocVec(size+1, MEMF_PUBLIC|MEMF_CLEAR))
    {
      memcpy(newtxt, txt, size);
    }
  set(data->GR_shorthelp, TEXTFIELD_ReadOnly, FALSE);

  data->shorthelp = newtxt;

  return(0);
}

SAVEDS ULONG mSaveAttributesArea(Class *cl, Object *obj, Msg msg)
{
  struct AreaData *data = INST_DATA(cl, obj);

  MB_SaveAttribute("MUIA_Background", TYPE_INT, data->background);
  MB_SaveAttribute("MUIA_Frame", TYPE_INT, data->frame);
  MB_SaveAttribute("MUIA_Fonts", TYPE_INT, data->fonts);
  MB_SaveAttribute("MUIA_Disabled", TYPE_BOOL, data->disabled);
  MB_SaveAttribute("MUIA_Show", TYPE_BOOL, data->show);
  MB_SaveAttribute("MUIA_HorizDisappear", TYPE_BOOL, data->horizdisappear);
  MB_SaveAttribute("MUIA_VertDisappear", TYPE_BOOL, data->vertdisappear);
  MB_SaveAttribute("MUIA_PhantomHoriz", TYPE_BOOL, data->phantomhoriz);
  MB_SaveAttribute("MUIA_FrameTitle", TYPE_STRING, (ULONG)data->frametitle);
  MB_SaveAttribute("MUIA_Selected", TYPE_BOOL, data->selected);
  MB_SaveAttribute("MUIA_InputMode", TYPE_INT, data->inputmode);
  MB_SaveAttribute("MUIA_ShowSelstate", TYPE_BOOL, data->showselstate);
  MB_SaveAttribute("MUIA_Draggable", TYPE_BOOL, data->draggable);
  MB_SaveAttribute("MUIA_Droppable", TYPE_BOOL, data->droppable);
  MB_SaveAttribute("MUIA_CycleChain", TYPE_INT, data->cyclechain);
  MB_SaveAttribute("MUIA_HorizWeight", TYPE_INT, data->horizweight);
  MB_SaveAttribute("MUIA_VertWeight", TYPE_INT, data->vertweight);
  MB_SaveAttribute("MUIA_ShortHelp", TYPE_STRING, (ULONG)data->shorthelp);

  return(1);
}

SAVEDS ULONG mSaveArea(Class *cl, Object *obj, Msg msg)
{
  char *label;

  get(obj, MBA_Object_Label, &label);
  MB_BeginSaveObject(AREA_TYPE, label);

  DoMethod(obj, MBM_Object_SaveAttributes);

  MB_EndSaveObject();
  return(1);
}

SAVEDS ULONG DispatcherArea(struct IClass *cl,
                            Object *obj,
                            Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
    case OM_NEW:                      return(mNewArea(cl, obj, (struct opSet *)msg));
    case OM_SET:                      return(mSetArea(cl, obj, (struct opSet *)msg));
    case OM_GET:                      return(mGetArea(cl, obj, (struct opGet *)msg));
    case OM_DISPOSE:                  return(mDisposeArea(cl, obj, msg));
    case MBM_Object_AttributesEdit:   return((ULONG)mAttributesEditArea(cl, obj, (struct MBP_AttributesEdit *) msg));
    case MBM_Object_CreateEdition:    return((ULONG)mCreateEditionArea(cl, obj, (struct MBP_CreateEdition*)msg));
    case MBM_Object_AttributesApply:  return(mAttributesApplyArea(cl, obj, msg));
    case MBM_Object_SaveAttributes:   return(mSaveAttributesArea(cl, obj, msg));
    case MBM_Object_Save:             return(mSaveArea(cl, obj, msg));
    default:
      retval = (APTR)DoSuperMethodA(cl, obj, msg);
      break;
  }

  return((ULONG)retval);
}

void InitAreaClass(void)
{
  extern ULONG HookEntry();

  if (AreaClass = MakeClass("MBAreaClass", "MBBaseClass", NULL, sizeof(struct AreaData), 0))
    {
      AreaClass->cl_Dispatcher.h_Entry = HookEntry;
      AreaClass->cl_Dispatcher.h_SubEntry = DispatcherArea;
      AddClass(AreaClass);
    }
}

void DisposeAreaClass(void)
{
  RemoveClass(AreaClass);
  FreeClass(AreaClass);
}
