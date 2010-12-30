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

struct StringData
{
  char   *accept;
  ULONG  advanceoncr; /* BOOL */
  Object *attachedlist;
  char  *contents;
  char  *edithook;
  ULONG format;
  char  *integer;
  ULONG lonelyedithook /* BOOL */
  char  *reject;
  ULONG secret; /* BOOL */
  APTR  STR_accept;
  APTR  CH_advanceoncr;
  APTR  MB_attachedlist;
  APTR  STR_contents;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;
extern struct MUI_CustomClass *GUIStringClass;

extern ULONG DoSuperNew(struct IClass *, Object *, ULONG, ...);

struct IClass   *StringClass;
ULONG           numString = 0;

SAVEDS ULONG mSetString(Class *cl, Object *obj, struct opSet * msg)
{
  return(DoSuperMethodA(cl, obj, (Msg)msg));
}

SAVEDS ULONG mGetString(Class *cl, Object *obj, struct opGet * msg)
{
  struct StringData *data = INST_DATA(cl, obj);
  ULONG   retval = 0;

  switch(msg->opg_AttrID)
    {
    case MBA_String_Contents:
      *(msg->opg_Storage) = (ULONG) data->contents;
      retval = 1;
      break;
    default:
      retval = DoSuperMethodA(cl, obj, (Msg)msg);
    }

  return(retval);
}

SAVEDS ULONG mNewString(Class *cl, Object *obj, struct opSet *msg)
{
  struct StringData *data = NULL;
  APTR retval = NULL;
  char *aux;
  char buffer_aux[20];

  if (retval = (APTR)DoSuperNew(cl, obj,
				MBA_Area_Frame, 5,
				MBA_Area_Background, 4,
				TAG_MORE, msg->ops_AttrList,
				TAG_DONE))
    {
      data = INST_DATA(cl, retval);
      /* string default label */
      sprintf(buffer_aux, "btString%d", numString++);
      aux = StringCopy(buffer_aux);
      set(retval, MBA_Object_Label, aux);
      /* string default attributes */
      data->accept   = NULL;
      data->contents = NULL;
      data->advanceoncr = FALSE;
      data->attachedlist = NULL;
      data->edithook = NULL;
      data->format = 0;
      data->integer = NULL;
      data->lonelyedithook = 0;
      data->reject = NULL;
      data->secret = FALSE;
      data->
    }
  return((ULONG)retval);
}

ULONG mDisposeString(Class *cl, Object *obj, Msg msg)
{
  struct StringData *data = INST_DATA(cl, obj);

  if (data->contents) FreeVec(data->contents);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS APTR mCreateTestString(Class *cl, Object *obj, Msg msg)
{
  struct StringData *data = INST_DATA(cl, obj);

  return(TextObject,
	 MUIA_String_Contents, data->contents,
	 End
	 );
}

SAVEDS APTR mCreateEditionString(Class *cl, Object *obj, Msg msg)
{
  struct StringData *data = INST_DATA(cl, obj);
  APTR   retval;
  char   *strpreparse, *strcontents;

  strpreparse = AllocVec(strlen(data->preparse), MEMF_PUBLIC|MEMF_CLEAR);
  strcontents = AllocVec(strlen(data->contents), MEMF_PUBLIC|MEMF_CLEAR);

  if (strpreparse) CInterpreter(data->preparse, strpreparse);
  if (strcontents) CInterpreter(data->contents, strcontents);

  if (strpreparse&&strcontents)
    retval = (APTR)DoSuperMethod(cl, obj, MBM_Object_CreateEdition,
				 GUIStringClass->mcc_Class, NULL,
				 MUIA_String_Contents, strcontents,
				 MUIA_String_HiChar, data->hichar,
				 MUIA_String_PreParse, strpreparse,
				 MUIA_String_SetMax, data->setmax,
				 MUIA_String_SetMin, data->setmin,
				 MUIA_String_SetVMax, data->setvmax,
				 MBA_GUIObject_Object, obj,
				 TAG_DONE
				 );
  if (strpreparse) FreeVec(strpreparse);
  if (strcontents) FreeVec(strcontents);

  set(obj, MBA_Object_MUIObject, retval);
  return(retval);
}

SAVEDS APTR mGetImageString(Class *cl, Object *obj, Msg msg)
{
  return(StringObject,
         StringFrame,
         MUIA_Draggable, TRUE,
         MUIA_String_Contents, "String",
         MUIA_String_PreParse, "\033c",
         MUIA_Background   , MUII_StringBack,
         End
         );
}

SAVEDS ULONG mAttributesEditString(Class *cl, Object *obj, struct MBP_AttributesEdit *msg)
{
  struct StringData *data = INST_DATA(cl, obj);
  char   **registertitles = msg->titles;
  APTR  STR_contents, STR_hichar, STR_preparse, CH_setmax, CH_setmin, CH_setvmax, aux;

  registertitles[msg->number] = "String";

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

SAVEDS ULONG mAttributesApplyString(Class *cl, Object *obj, Msg msg)
{
  struct StringData *data = INST_DATA(cl, obj);
  char * straux;

  /* Record String Contents */
  get(data->STR_contents, MUIA_String_Contents, &straux);
  ReplaceString(&data->contents, straux);

  /* Record String Preparse */
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

SAVEDS ULONG mSaveAttributesString(Class *cl, Object *obj, Msg msg)
{
  struct StringData *data = INST_DATA(cl, obj);

  if (data->contents&&(strlen(data->contents)>0))
    MB_SaveAttribute("MUIA_String_Contents", TYPE_STRING, (ULONG)data->contents);
  if (data->hichar) MB_SaveAttribute("MUIA_String_hichar", TYPE_CHAR, (ULONG)data->hichar);
  if (data->preparse&&(strlen(data->preparse)>0))
    MB_SaveAttribute("MUIA_String_Preparse", TYPE_STRING, (ULONG)data->preparse);
  MB_SaveAttribute("MUIA_String_SetMax", TYPE_INT, data->setmax);
  MB_SaveAttribute("MUIA_String_SetMin", TYPE_INT, data->setmin);
  MB_SaveAttribute("MUIA_String_SetVMax", TYPE_INT, data->setvmax);

  return(DoSuperMethodA(cl, obj, msg));;
}

SAVEDS ULONG mSaveString(Class *cl, Object *obj, Msg msg)
{
  char *label;

  get(obj, MBA_Object_Label, &label);
  MB_BeginSaveObject("MBString", label);

  DoMethod(obj, MBM_Object_SaveAttributes);

  MB_EndSaveObject();
  return(1);
}

SAVEDS ULONG DispatcherString(struct IClass *cl,
                            Object *obj,
                            Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
  case OM_NEW:                      return(mNewString(cl, obj, (struct opSet *)msg));
  case OM_SET:                      return(mSetString(cl, obj, (struct opSet *)msg));
  case OM_GET:                      return(mGetString(cl, obj, (struct opGet *)msg));
  case OM_DISPOSE:                  return(mDisposeString(cl, obj, msg));
  case MBM_Object_CreateTest:       return((ULONG)mCreateTestString(cl, obj, msg));
  case MBM_Object_CreateEdition:    return((ULONG)mCreateEditionString(cl, obj, msg));
  case MBM_Object_GetImage:         return((ULONG)mGetImageString(cl, obj, msg));
  case MBM_Object_AttributesEdit:   return((ULONG)mAttributesEditString(cl, obj, (struct MBP_AttributesEdit *)msg));
  case MBM_Object_AttributesApply:  return(mAttributesApplyString(cl, obj, msg));
  case MBM_Object_SaveAttributes:   return(mSaveAttributesString(cl, obj, msg));
  case MBM_Object_Save:             return(mSaveString(cl, obj, msg));
  default:
    retval = (APTR)DoSuperMethodA(cl, obj, msg);
    break;
  }

  return((ULONG)retval);
}

void InitStringClass(void)
{
  extern ULONG HookEntry();
  extern ULONG InitGUIStringClass(void);

  if (StringClass = MakeClass("MBStringClass", "MBAreaClass", NULL, sizeof(struct StringData), 0))
    {
      StringClass->cl_Dispatcher.h_Entry = HookEntry;
      StringClass->cl_Dispatcher.h_SubEntry = DispatcherString;
      AddClass(StringClass);
    }
  InitGUIStringClass();
}

void DisposeStringClass(void)
{
  RemoveClass(StringClass);
  FreeClass(StringClass);
}
