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

struct TextData
{
  char  *contents;
  char  hichar;
  char  *preparse;
  ULONG setmax;
  ULONG setmin;
  ULONG setvmax;
  APTR  STR_contents;
  APTR  STR_hichar;
  APTR  STR_preparse;
  APTR  CH_setmax;
  APTR  CH_setmin;
  APTR  CH_setvmax;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;
extern struct MUI_CustomClass *GUITextClass;

extern ULONG DoSuperNew(struct IClass *, Object *, ULONG, ...);

struct IClass   *TextClass;
ULONG           numText = 0;

SAVEDS ULONG mSetText(Class *cl, Object *obj, struct opSet * msg)
{
  return(DoSuperMethodA(cl, obj, (Msg)msg));
}

SAVEDS ULONG mGetText(Class *cl, Object *obj, struct opGet * msg)
{
  struct TextData *data = INST_DATA(cl, obj);
  ULONG   retval = 0;

  switch(msg->opg_AttrID)
    {
    case MBA_Text_Contents:
      *(msg->opg_Storage) = (ULONG) data->contents;
      retval = 1;
      break;
    default:
      retval = DoSuperMethodA(cl, obj, (Msg)msg);
    }

  return(retval);
}

SAVEDS ULONG mNewText(Class *cl, Object *obj, struct opSet *msg)
{
  struct TextData *data = NULL;
  APTR retval = NULL;
  char *aux;
  char buffer_aux[20];

  if (retval = (APTR)DoSuperNew(cl, obj,
				MBA_Area_Frame, 3,
				MBA_Area_Background, 4,
				TAG_MORE, msg->ops_AttrList,
				TAG_DONE))
    {
      data = INST_DATA(cl, retval);
      sprintf(buffer_aux, "Text%d", numText);
      data->contents = StringCopy(buffer_aux);
      sprintf(buffer_aux, "btText%d", numText++);
      aux = StringCopy(buffer_aux);
      set(retval, MBA_Object_Label, aux);
      data->hichar = '\0';
      data->preparse = StringCopy("\\033c");
      data->setmax   = FALSE;
      data->setmin   = TRUE;
      data->setvmax  = FALSE;
    }
  return((ULONG)retval);
}

ULONG mDisposeText(Class *cl, Object *obj, Msg msg)
{
  struct TextData *data = INST_DATA(cl, obj);

  if (data->contents) FreeVec(data->contents);
  if (data->preparse) FreeVec(data->preparse);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS APTR mCreateTestText(Class *cl, Object *obj, Msg msg)
{
  struct TextData *data = INST_DATA(cl, obj);

  return(TextObject,
	 MUIA_Text_Contents, data->contents,
	 MUIA_Text_HiChar, data->hichar,
	 MUIA_Text_PreParse, data->preparse,
	 MUIA_Text_SetMax, data->setmax,
	 MUIA_Text_SetMin, data->setmin,
	 MUIA_Text_SetVMax, data->setvmax,
	 End
	 );
}

SAVEDS APTR mCreateEditionText(Class *cl, Object *obj, Msg msg)
{
  struct TextData *data = INST_DATA(cl, obj);
  APTR   retval;
  char   *strpreparse, *strcontents;
  
  strpreparse = AllocVec(strlen(data->preparse), MEMF_PUBLIC|MEMF_CLEAR);
  strcontents = AllocVec(strlen(data->contents), MEMF_PUBLIC|MEMF_CLEAR);

  if (strpreparse) CInterpreter(data->preparse, strpreparse);
  if (strcontents) CInterpreter(data->contents, strcontents);

  if (strpreparse&&strcontents)
    retval = (APTR)DoSuperMethod(cl, obj, MBM_Object_CreateEdition,
				 GUITextClass->mcc_Class, NULL,
				 MUIA_Text_Contents, strcontents,
				 MUIA_Text_HiChar, data->hichar,
				 MUIA_Text_PreParse, strpreparse,
				 MUIA_Text_SetMax, data->setmax,
				 MUIA_Text_SetMin, data->setmin,
				 MUIA_Text_SetVMax, data->setvmax,
				 MBA_GUIObject_Object, obj,
				 TAG_DONE
				 );
  if (strpreparse) FreeVec(strpreparse);
  if (strcontents) FreeVec(strcontents);

  set(obj, MBA_Object_MUIObject, retval);
  return(retval);
}

SAVEDS APTR mGetImageText(Class *cl, Object *obj, Msg msg)
{
  return(TextObject,
         TextFrame,
         MUIA_Draggable, TRUE,
         MUIA_Text_Contents, "Text",
         MUIA_Text_PreParse, "\033c",
         MUIA_Background   , MUII_TextBack,
         End
         );
}

SAVEDS ULONG mAttributesEditText(Class *cl, Object *obj, struct MBP_AttributesEdit *msg)
{
  struct TextData *data = INST_DATA(cl, obj);
  char   **registertitles = msg->titles;
  APTR  STR_contents, STR_hichar, STR_preparse, CH_setmax, CH_setmin, CH_setvmax, aux;

  registertitles[msg->number] = "Text";

  aux = VGroup,
    Child, HVSpace,
    Child, ColGroup(2),
      Child, Label("_Contents"),
      Child, STR_contents = KeyString(data->contents, 80, 'c'),
      Child, Label("_Preparse"),
      Child, STR_preparse = KeyString(data->preparse, 80, 'p'),
      Child, Label("_HiChar"),
      Child, STR_hichar = KeyString(data->hichar, 2, 'h'),
    End,
    Child, ColGroup(4),
      Child, Label("SetMa_x"),
      Child, CH_setmax = KeyCheckMark(data->setmax, 'x'),
      Child, Label("SetM_in"),
      Child, CH_setmin = KeyCheckMark(data->setmin, 'i'),
      Child, Label("Set_VMax"),
      Child, CH_setvmax = KeyCheckMark(data->setvmax, 'v'),
    End,
    Child, HVSpace,
  End;

  if (aux)
    {
      data->STR_contents = STR_contents;
      data->STR_preparse = STR_contents;
      data->STR_hichar = STR_hichar;
      data->CH_setmax = CH_setmax;
      data->CH_setmin = CH_setmin;
      data->CH_setvmax = CH_setvmax;

      DoMethod(msg->group, OM_ADDMEMBER, aux);
      DoSuperMethod(cl, obj, MBM_Object_AttributesEdit,
                    msg->group,
                    msg->number+1,
                    registertitles);
    }
  else return(0);

  return(1);
}

SAVEDS ULONG mAttributesApplyText(Class *cl, Object *obj, Msg msg)
{
  struct TextData *data = INST_DATA(cl, obj);
  char * straux;

  /* Record Text Contents */
  get(data->STR_contents, MUIA_String_Contents, &straux);
  ReplaceString(&data->contents, straux);

  /* Record Text Preparse */
  get(data->STR_preparse, MUIA_String_Contents, &straux);
  ReplaceString(&data->preparse, straux);

  /* Record other attributes */
  get(data->CH_setmax, MUIA_Selected, &data->setmax);
  get(data->CH_setmin, MUIA_Selected, &data->setmin);
  get(data->CH_setvmax, MUIA_Selected, &data->setvmax);

  get(data->STR_hichar, MUIA_String_Contents, &straux);
  data->hichar = straux[0];

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mSaveAttributesText(Class *cl, Object *obj, Msg msg)
{
  struct TextData *data = INST_DATA(cl, obj);

  if (data->contents&&(strlen(data->contents)>0))
    MB_SaveAttribute("MUIA_Text_Contents", TYPE_STRING, (ULONG)data->contents);
  if (data->hichar) MB_SaveAttribute("MUIA_Text_hichar", TYPE_CHAR, (ULONG)data->hichar);
  if (data->preparse&&(strlen(data->preparse)>0))
    MB_SaveAttribute("MUIA_Text_Preparse", TYPE_STRING, (ULONG)data->preparse);
  MB_SaveAttribute("MUIA_Text_SetMax", TYPE_INT, data->setmax);
  MB_SaveAttribute("MUIA_Text_SetMin", TYPE_INT, data->setmin);
  MB_SaveAttribute("MUIA_Text_SetVMax", TYPE_INT, data->setvmax);

  return(DoSuperMethodA(cl, obj, msg));;
}

SAVEDS ULONG mSaveText(Class *cl, Object *obj, Msg msg)
{
  char *label;

  get(obj, MBA_Object_Label, &label);
  MB_BeginSaveObject("MBText", label);

  DoMethod(obj, MBM_Object_SaveAttributes);

  MB_EndSaveObject();
  return(1);
}

SAVEDS ULONG DispatcherText(struct IClass *cl,
                            Object *obj,
                            Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
  case OM_NEW:                      return(mNewText(cl, obj, (struct opSet *)msg));
  case OM_SET:                      return(mSetText(cl, obj, (struct opSet *)msg));
  case OM_GET:                      return(mGetText(cl, obj, (struct opGet *)msg));
  case OM_DISPOSE:                  return(mDisposeText(cl, obj, msg));
  case MBM_Object_CreateTest:       return((ULONG)mCreateTestText(cl, obj, msg));
  case MBM_Object_CreateEdition:    return((ULONG)mCreateEditionText(cl, obj, msg));
  case MBM_Object_GetImage:         return((ULONG)mGetImageText(cl, obj, msg));
  case MBM_Object_AttributesEdit:   return((ULONG)mAttributesEditText(cl, obj, (struct MBP_AttributesEdit *)msg));
  case MBM_Object_AttributesApply:  return(mAttributesApplyText(cl, obj, msg));
  case MBM_Object_SaveAttributes:   return(mSaveAttributesText(cl, obj, msg));
  case MBM_Object_Save:             return(mSaveText(cl, obj, msg));
  default:
    retval = (APTR)DoSuperMethodA(cl, obj, msg);
    break;
  }

  return((ULONG)retval);
}

void InitTextClass(void)
{
  extern ULONG HookEntry();
  extern ULONG InitGUITextClass(void);

  if (TextClass = MakeClass("MBTextClass", "MBAreaClass", NULL, sizeof(struct TextData), 0))
    {
      TextClass->cl_Dispatcher.h_Entry = HookEntry;
      TextClass->cl_Dispatcher.h_SubEntry = DispatcherText;
      AddClass(TextClass);
    }
  InitGUITextClass();
}

void DisposeTextClass(void)
{
  RemoveClass(TextClass);
  FreeClass(TextClass);
}
