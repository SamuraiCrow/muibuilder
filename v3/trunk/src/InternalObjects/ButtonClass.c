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
#include "/Libs/strings.h"

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

#define MUI_OBSOLETE

struct ButtonData
{
  char* title;
  APTR  STR_title;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;
extern struct MUI_CustomClass *GUIButtonClass;

extern ULONG DoSuperNew(struct IClass *, Object *, ULONG, ...);

struct IClass   *ButtonClass;
ULONG           numButton = 0;

SAVEDS ULONG mSetButton(Class *cl, Object *obj, struct opSet * msg)
{
  return(DoSuperMethodA(cl, obj, (Msg)msg));
}

SAVEDS ULONG mGetButton(Class *cl, Object *obj, struct opGet * msg)
{
  struct ButtonData *data = INST_DATA(cl, obj);
  ULONG   retval = 0;

  switch(msg->opg_AttrID)
    {
    case MBA_Button_Title:
      *(msg->opg_Storage) = (ULONG) data->title;
      retval = 1;
      break;
    }

  if (!retval) DoSuperMethodA(cl, obj, (Msg)msg);

  return(retval);
}

SAVEDS ULONG mNewButton(Class *cl, Object *obj, struct opSet *msg)
{
  struct ButtonData *data = NULL;
  APTR retval = NULL;
  char *aux;

  if (retval = (APTR)DoSuperNew(cl, obj,
				MBA_Area_Frame, 1, 
				MBA_Area_Background, 2,
				TAG_MORE, msg->ops_AttrList,
				TAG_DONE))
    {
      data = INST_DATA(cl, retval);
      if (data->title = AllocVec(10, MEMF_PUBLIC|MEMF_CLEAR))
        sprintf(data->title, "Button%d", numButton);
      if (aux = AllocVec(12, MEMF_PUBLIC|MEMF_CLEAR))
	sprintf(aux, "btButton%d", numButton++);
      set(retval, MBA_Object_Label, aux);
    }
  return((ULONG)retval);
}

ULONG mDisposeButton(Class *cl, Object *obj, Msg msg)
{
  struct ButtonData *data = INST_DATA(cl, obj);

  if (data->title) FreeVec(data->title);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS APTR mCreateTestButton(Class *cl, Object *obj, Msg msg)
{
  struct ButtonData *data = INST_DATA(cl, obj);

  return(SimpleButton(data->title));
}

SAVEDS APTR mCreateEditionButton(Class *cl, Object *obj, Msg msg)
{
  struct ButtonData *data = INST_DATA(cl, obj);
  APTR   retval;

  retval = (APTR)DoSuperMethod(cl, obj, MBM_Object_CreateEdition,
                  GUIButtonClass->mcc_Class, NULL,
                  MUIA_Text_Contents, data->title,
                  MBA_GUIObject_Object, obj,
                  TAG_DONE
                   );

  set(obj, MBA_Object_MUIObject, retval);
  return(retval);
}

SAVEDS APTR mGetImageButton(Class *cl, Object *obj, Msg msg)
{
  return(TextObject,
         ButtonFrame,
         MUIA_Draggable, TRUE,
         MUIA_Text_Contents, "Button",
         MUIA_Text_PreParse, "\33c",
         MUIA_Text_HiChar  , 'b',
         MUIA_ControlChar  , 'b',
         MUIA_Background   , MUII_ButtonBack,
         End
         );
}

SAVEDS ULONG mAttributesEditButton(Class *cl, Object *obj, struct MBP_AttributesEdit *msg)
{
  struct ButtonData *data = INST_DATA(cl, obj);
  APTR   STR_title, aux;
  char   **registertitles = msg->titles;

  registertitles[msg->number] = "Button";

  aux = VGroup,
    Child, HVSpace,
    Child, HGroup,
      Child, Label("_Title"),
      Child, STR_title = KeyString(data->title, 80, 't'),
    End,
    Child, HVSpace,
  End;

  if (aux)
    {
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

SAVEDS ULONG mAttributesApplyButton(Class *cl, Object *obj, Msg msg)
{
  struct ButtonData *data = INST_DATA(cl, obj);
  char * straux;

  /* Record Button Title */
  get(data->STR_title, MUIA_String_Contents, &straux);

  ReplaceString(&data->title, straux);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mSaveAttributesButton(Class *cl, Object *obj, Msg msg)
{
  struct ButtonData *data = INST_DATA(cl, obj);

  MB_SaveAttribute("MUIA_Button_Title", TYPE_STRING, (ULONG)data->title);

  return(DoSuperMethodA(cl, obj, msg));;
}


SAVEDS ULONG mSaveButton(Class *cl, Object *obj, Msg msg)
{
  char *label;

  get(obj, MBA_Object_Label, &label);
  MB_BeginSaveObject("MBButton", label);

  DoMethod(obj, MBM_Object_SaveAttributes);

  MB_EndSaveObject();
  return(1);
}

SAVEDS ULONG DispatcherButton(struct IClass *cl,
                            Object *obj,
                            Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
  case OM_NEW:                      return(mNewButton(cl, obj, (struct opSet *)msg));
  case OM_SET:                      return(mSetButton(cl, obj, (struct opSet *)msg));
  case OM_GET:                      return(mGetButton(cl, obj, (struct opGet *)msg));
  case OM_DISPOSE:                  return(mDisposeButton(cl, obj, msg));
  case MBM_Object_CreateTest:       return((ULONG)mCreateTestButton(cl, obj, msg));
  case MBM_Object_CreateEdition:    return((ULONG)mCreateEditionButton(cl, obj, msg));
  case MBM_Object_GetImage:         return((ULONG)mGetImageButton(cl, obj, msg));
  case MBM_Object_AttributesEdit:   return((ULONG)mAttributesEditButton(cl, obj, (struct MBP_AttributesEdit *)msg));
  case MBM_Object_AttributesApply:  return(mAttributesApplyButton(cl, obj, msg));
  case MBM_Object_SaveAttributes:   return(mSaveAttributesButton(cl, obj, msg));
  case MBM_Object_Save:             return(mSaveButton(cl, obj, msg));
  default:
    retval = (APTR)DoSuperMethodA(cl, obj, msg);
    break;
  }

  return((ULONG)retval);
}

void InitButtonClass(void)
{
  extern ULONG HookEntry();
  extern ULONG InitGUIButtonClass(void);

  if (ButtonClass = MakeClass("MBButtonClass", "MBAreaClass", NULL, sizeof(struct ButtonData), 0))
    {
      ButtonClass->cl_Dispatcher.h_Entry = HookEntry;
      ButtonClass->cl_Dispatcher.h_SubEntry = DispatcherButton;
      AddClass(ButtonClass);
    }
  InitGUIButtonClass();
}

void DisposeButtonClass(void)
{
  RemoveClass(ButtonClass);
  FreeClass(ButtonClass);
}
