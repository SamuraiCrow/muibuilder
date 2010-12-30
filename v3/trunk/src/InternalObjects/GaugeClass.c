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

struct GaugeData
{
  ULONG current;
  ULONG divide;
  ULONG horiz;
  char  *infotext;
  ULONG max;
  APTR  SL_current;
  APTR  SL_divide;
  APTR  CH_horiz;
  APTR  STR_infotext;
  APTR  STR_max;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;
extern struct MUI_CustomClass *GUIGaugeClass;

extern ULONG DoSuperNew(struct IClass *, Object *, ULONG, ...);

struct IClass   *GaugeClass;
ULONG           numGauge = 0;

SAVEDS ULONG mSetGauge(Class *cl, Object *obj, struct opSet * msg)
{
  return(DoSuperMethodA(cl, obj, (Msg)msg));
}

SAVEDS ULONG mGetGauge(Class *cl, Object *obj, struct opGet * msg)
{
  struct GaugeData *data = INST_DATA(cl, obj);
  ULONG   retval = 0;

  switch(msg->opg_AttrID)
    {
    case MBA_Gauge_InfoText:
      *(msg->opg_Storage) = (ULONG) data->infotext;
      retval = 1;
      break;
    }

  if (!retval) DoSuperMethodA(cl, obj, (Msg)msg);

  return(retval);
}

SAVEDS ULONG mNewGauge(Class *cl, Object *obj, struct opSet *msg)
{
  struct GaugeData *data = NULL;
  APTR retval = NULL;
  char *aux;

  if (retval = (APTR)DoSuperNew(cl, obj,
				MBA_Area_Frame, 9, 
				MBA_Area_Background, 4,
				TAG_MORE, msg->ops_AttrList,
				TAG_DONE))

    {
      data = INST_DATA(cl, retval);
      if (aux = AllocVec(12, MEMF_PUBLIC|MEMF_CLEAR))
	sprintf(aux, "gaGauge%d", numGauge++);
      set(retval, MBA_Object_Label, aux);

      data->infotext = NULL;
      data->current  = 100;
      data->max      = 0;
      data->horiz    = TRUE;
    }
  return((ULONG)retval);
}

ULONG mDisposeGauge(Class *cl, Object *obj, Msg msg)
{
  struct GaugeData *data = INST_DATA(cl, obj);

  if (data->infotext) FreeVec(data->infotext);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS APTR mCreateTestGauge(Class *cl, Object *obj, Msg msg)
{
  struct GaugeData *data = INST_DATA(cl, obj);

  return(GaugeObject,
	 MUIA_Gauge_InfoText, data->infotext,
	 MUIA_Gauge_Current, data->current,
	 MUIA_Gauge_Max, data->max,
	 MUIA_Gauge_Horiz, data->horiz,
	 MUIA_Gauge_Divide, data->divide,
	 End
	 );
}

SAVEDS APTR mCreateEditionGauge(Class *cl, Object *obj, Msg msg)
{
  struct GaugeData *data = INST_DATA(cl, obj);
  APTR   retval;

  retval = (APTR)DoSuperMethod(cl, obj, MBM_Object_CreateEdition,
			       GUIGaugeClass->mcc_Class, NULL,
			       MUIA_Gauge_InfoText, data->infotext,
			       MUIA_Gauge_Current, data->current,
			       MUIA_Gauge_Max, data->max,
			       MUIA_Gauge_Horiz, data->horiz,
			       MUIA_Gauge_Divide, data->divide,
			       MBA_GUIObject_Object, obj,
			       TAG_DONE
			       );

  set(obj, MBA_Object_MUIObject, retval);
  return(retval);
}

SAVEDS APTR mGetImageGauge(Class *cl, Object *obj, Msg msg)
{
  return(GaugeObject,
         GaugeFrame,
	 MUIA_Gauge_InfoText, "Gauge",
	 MUIA_Gauge_Current,  60,
	 MUIA_Gauge_Max,      100,
	 MUIA_Gauge_Horiz,    TRUE,
         MUIA_Draggable,      TRUE,
         /*MUIA_Background,     MUII_GaugeBack,*/
         End
         );
}

SAVEDS ULONG mAttributesEditGauge(Class *cl, Object *obj, struct MBP_AttributesEdit *msg)
{
  struct GaugeData *data = INST_DATA(cl, obj);
  APTR   STR_max, SL_current, SL_divide, CH_horiz, STR_infotext, aux;
  char   **registertitles = msg->titles;

  registertitles[msg->number] = "Gauge";

  aux = VGroup,
    Child, HVSpace,
    Child, ColGroup(2),
      Child, Label("_InfoText"),
      Child, STR_infotext = KeyString(data->infotext, 80, 'i'),
      Child, Label("Ma_x"),
      Child, STR_max = StringObject,
        StringFrame,
        MUIA_String_Integer, data->max,
        MUIA_ControlChar, 'x',
        MUIA_String_MaxLen, 10,
        MUIA_String_Accept, "0123456789-",
      End,
      Child, Label("_Current"),
      Child, SL_current = KeySlider(0, data->max, data->current, 'c'),
      Child, Label("_Divide"),
      Child, SL_divide = KeySlider(0, data->max, data->divide, 'd'),
    End,
    Child, HGroup,
      Child, HVSpace,
      Child, Label("_Horizontal"),
      Child, CH_horiz = KeyCheckMark(data->horiz, 'h'),
      Child, HVSpace,
    End,
    Child, HVSpace,
  End;

  if (aux)
    {
      data->STR_infotext = STR_infotext;
      data->STR_max      = STR_max;
      data->SL_current   = SL_current;
      data->SL_divide    = SL_divide;
      data->CH_horiz     = CH_horiz;

      DoMethod(msg->group, OM_ADDMEMBER, aux);
      DoSuperMethod(cl, obj, MBM_Object_AttributesEdit,
                    msg->group,
                    msg->number+1,
                    registertitles);
    }
  else return(0);

  return(1);
}

SAVEDS ULONG mAttributesApplyGauge(Class *cl, Object *obj, Msg msg)
{
  struct GaugeData *data = INST_DATA(cl, obj);
  char * straux;

  /* Record Gauge Title */
  get(data->STR_infotext, MUIA_String_Contents, &straux);

  if (data->infotext) FreeVec(data->infotext);
  if (data->infotext = AllocVec(strlen(straux)+1, MEMF_PUBLIC|MEMF_CLEAR))
    strcpy(data->infotext, straux);

  get(data->STR_max, MUIA_String_Integer, &data->max);
  get(data->SL_current, MUIA_Slider_Level, &data->current);
  get(data->SL_divide, MUIA_Slider_Level, &data->divide);
  get(data->CH_horiz, MUIA_Selected, &data->horiz);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG DispatcherGauge(struct IClass *cl,
                            Object *obj,
                            Msg msg)
{
  APTR retval = NULL;

  switch(msg->MethodID)
  {
  case OM_NEW:                      return(mNewGauge(cl, obj, (struct opSet *)msg));
  case OM_SET:                      return(mSetGauge(cl, obj, (struct opSet *)msg));
  case OM_GET:                      return(mGetGauge(cl, obj, (struct opGet *)msg));
  case OM_DISPOSE:                  return(mDisposeGauge(cl, obj, msg));
  case MBM_Object_CreateTest:       return((ULONG)mCreateTestGauge(cl, obj, msg));
  case MBM_Object_CreateEdition:    return((ULONG)mCreateEditionGauge(cl, obj, msg));
  case MBM_Object_GetImage:         return((ULONG)mGetImageGauge(cl, obj, msg));
  case MBM_Object_AttributesEdit:   return((ULONG)mAttributesEditGauge(cl, obj, (struct MBP_AttributesEdit *)msg));
  case MBM_Object_AttributesApply:  return(mAttributesApplyGauge(cl, obj, msg));
  default:
    retval = (APTR)DoSuperMethodA(cl, obj, msg);
    break;
  }

  return((ULONG)retval);
}

void InitGaugeClass(void)
{
  extern ULONG HookEntry();
  extern ULONG InitGUIGaugeClass(void);

  if (GaugeClass = MakeClass("MBGaugeClass", "MBAreaClass", NULL, sizeof(struct GaugeData), 0))
    {
      GaugeClass->cl_Dispatcher.h_Entry = HookEntry;
      GaugeClass->cl_Dispatcher.h_SubEntry = DispatcherGauge;
      AddClass(GaugeClass);
    }
  InitGUIGaugeClass();
}

void DisposeGaugeClass(void)
{
  RemoveClass(GaugeClass);
  FreeClass(GaugeClass);
}
