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

#define GROUP_TYPE "MBGroup"

struct GroupData
{
  ULONG horiz;
  ULONG activepage;
  ULONG columns;
  ULONG pagemode;
  ULONG rows;
  ULONG sameheight;
  ULONG samewidth;
  APTR  CH_horiz;
  APTR  SL_activepage;
  APTR  SL_columns;
  APTR  CH_pagemode;
  APTR  SL_rows;
  APTR  CH_sameheight;
  APTR  CH_samewidth;
  struct List *childs;
};

extern struct Library *UtilityBase;
extern struct Library *IntuitionBase;
extern struct Library *MUIMasterBase;

extern ULONG DoSuperNew(struct IClass *, Object *, ULONG, ...);

struct IClass *GroupClass;
extern struct MUI_CustomClass *GUIGroupClass;

ULONG numGroup = 0;

SAVEDS ULONG mSetGroup(Class *cl, Object *obj, struct opSet * msg)
{
  struct GroupData *data = INST_DATA(cl, obj);
  struct TagItem *ti, *tstate;
  ULONG  retval = 0;

  retval = DoSuperMethodA(cl, obj, (Msg)msg);
  ti = msg->ops_AttrList;
  tstate = ti;
  while(ti = NextTagItem(&tstate))
  {
    switch(ti->ti_Tag)
    {
    case MBA_Group_Horiz:
      data->horiz = (ti->ti_Data == TRUE);
      retval = 1L;
      break;
    }
  }
  return(retval);
}

SAVEDS ULONG mGetGroup(Class *cl, Object *obj, struct opGet * msg)
{
  struct GroupData *data = INST_DATA(cl, obj);
  ULONG   retval = 0;

  switch(msg->opg_AttrID)
    {
    case MBA_Group_Horiz:
      *(msg->opg_Storage) = (ULONG) data->horiz;
      retval = 1L;
      break;
    case MBA_Group_IsEmpty:
      *(msg->opg_Storage) = (ULONG) IsListEmpty(data->childs);
      retval = 1L;
      break;
    default:
      retval = DoSuperMethodA(cl, obj, (Msg)msg);
      break;
    }

  return(retval);
}

SAVEDS ULONG mNewGroup(Class *cl, Object *obj, struct opSet *msg)
{
  struct GroupData *data = NULL;
  APTR retval = NULL;
  char *aux;

  if (retval = (APTR)DoSuperNew(cl, obj, 
				MBA_Area_Frame, 11,
				MBA_Area_Background, 9,
				TAG_MORE, msg->ops_AttrList,
				TAG_DONE))
    {
      data = INST_DATA(cl, retval);
      data->horiz = GetTagData(MBA_Group_Horiz, 1L, msg->ops_AttrList);
      data->activepage = 0;
      data->columns = 0;
      data->pagemode = 0;
      data->rows = 0;
      data->sameheight = 0;
      data->samewidth = 0;
      if (data->childs = AllocVec(sizeof(struct List), MEMF_PUBLIC|MEMF_CLEAR))
        NewList(data->childs);
      if (aux = AllocVec(12, MEMF_PUBLIC|MEMF_CLEAR))
	sprintf(aux, "grGroup%d", numGroup++);
      set(retval, MBA_Object_Label, aux);
    }
  return((ULONG)retval);
}

ULONG mDisposeGroup(Class *cl, Object *obj, Msg msg)
{
  struct GroupData *data = INST_DATA(cl, obj);
  struct Node *node;

  if (!IsListEmpty(data->childs))
    {
      for(node=data->childs->lh_Head; node->ln_Succ; node=node->ln_Succ)
        {
          DisposeObject(node->ln_Name);
        }
    }
  
  FreeVec(data->childs);
  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS APTR mCreateEditionGroup(Class *cl, Object *obj, Msg msg)
{
  struct GroupData *data = INST_DATA(cl, obj);
  APTR retval, aux;
  struct Node *node;

  if (data->rows&&data->columns)
    {
      retval = (APTR)DoSuperMethod(cl, obj, MBM_Object_CreateEdition,
				   GUIGroupClass->mcc_Class, NULL,
				   MUIA_Group_Horiz, data->horiz,
				   MUIA_Group_PageMode, data->pagemode,
				   MUIA_Group_ActivePage, data->activepage,
				   MUIA_Group_SameHeight, data->sameheight,
				   MUIA_Group_SameWidth, data->samewidth,
				   MUIA_Group_Columns, data->columns,
				   MUIA_Group_Rows, data->rows,
				   MBA_GUIObject_Object, obj,
				   TAG_DONE
				   );
    }
  else if (data->rows)
    {
      retval = (APTR)DoSuperMethod(cl, obj, MBM_Object_CreateEdition,
				   GUIGroupClass->mcc_Class, NULL,
				   MUIA_Group_Horiz, data->horiz,
				   MUIA_Group_PageMode, data->pagemode,
				   MUIA_Group_ActivePage, data->activepage,
				   MUIA_Group_SameHeight, data->sameheight,
				   MUIA_Group_SameWidth, data->samewidth,
				   MUIA_Group_Rows, data->rows,
				   MBA_GUIObject_Object, obj,
				   TAG_DONE
				   );
    }
  else if (data->columns)
    {
      retval = (APTR)DoSuperMethod(cl, obj, MBM_Object_CreateEdition,
				   GUIGroupClass->mcc_Class, NULL,
				   MUIA_Group_Horiz, data->horiz,
				   MUIA_Group_PageMode, data->pagemode,
				   MUIA_Group_ActivePage, data->activepage,
				   MUIA_Group_SameHeight, data->sameheight,
				   MUIA_Group_SameWidth, data->samewidth,
				   MUIA_Group_Columns, data->columns,
				   MBA_GUIObject_Object, obj,
				   TAG_DONE
				   );
    }
  else
    retval = (APTR)DoSuperMethod(cl, obj, MBM_Object_CreateEdition,
				 GUIGroupClass->mcc_Class, NULL,
				 MUIA_Group_Horiz, data->horiz,
				 MUIA_Group_PageMode, data->pagemode,
				 MUIA_Group_ActivePage, data->activepage,
				 MUIA_Group_SameHeight, data->sameheight,
				 MUIA_Group_SameWidth, data->samewidth,
				 MBA_GUIObject_Object, obj,
				 TAG_DONE
				 );

  if (retval)
    {
      if (IsListEmpty(data->childs))
	{
	  DoMethod(retval, OM_ADDMEMBER, HVSpace);
	}
      else
        for(node=data->childs->lh_Head; node->ln_Succ; node=node->ln_Succ)
          {
            aux = (APTR)DoMethod((APTR)node->ln_Name, MBM_Object_CreateEdition, TAG_DONE);
            if (aux)
              {
                set(aux, MUIA_CycleChain, 1L);
                DoMethod(retval, OM_ADDMEMBER, aux);
              }
          }
    }
  
  set(obj, MBA_Object_MUIObject, retval);
  return(retval);
}

SAVEDS APTR mCreateTestGroup(Class *cl, Object *obj, Msg msg)
{
  struct GroupData *data = INST_DATA(cl, obj);
  APTR retval;
  struct Node *node;

  retval = GroupObject,
    MUIA_Group_Horiz, data->horiz,
    End;

  if (!IsListEmpty(data->childs))
    for(node=data->childs->lh_Head; node->ln_Succ; node=node->ln_Succ)
      {
        DoMethod(retval, OM_ADDMEMBER,
                 DoMethod((APTR)node->ln_Name, MBM_Object_CreateTest, TAG_DONE));
      }
  
  return(retval);
}

SAVEDS ULONG mAddObject(Class *cl, Object *obj, struct MBP_AddObj *msg)
{
  struct GroupData *data = INST_DATA(cl, obj);
  struct Node *node, *allocnode;

  if (allocnode = AllocVec(sizeof(struct Node), MEMF_PUBLIC|MEMF_CLEAR))
    {
      allocnode->ln_Name = (char*)(msg->source);
      allocnode->ln_Pri = 0;
    }
  else return(0);

  if (IsListEmpty(data->childs))
    {
      AddHead(data->childs, allocnode);
      return(1);
    }

  if (data->childs->lh_Head->ln_Name == (char*)msg->position)
    {
      AddHead(data->childs, allocnode);
      return(1);
    }

  if (!msg->position)
    {
      AddTail(data->childs, allocnode);
      return(1);
    }

  for(node=data->childs->lh_Head; node->ln_Succ; node=node->ln_Succ)
    {
      if (node->ln_Name == (char*)msg->position)
        {
          Insert(data->childs, allocnode, node->ln_Pred);
        }
    }
  return(0);
}

SAVEDS ULONG mRemObject(Class *cl, Object *obj, struct MBP_AddObj *msg)
{
  struct GroupData *data = INST_DATA(cl, obj);
  struct Node *node;
  Object *guigroup;

  for(node=data->childs->lh_Head; node->ln_Succ; node=node->ln_Succ)
    {
      if (node->ln_Name == (char*)msg->source)
        {
          Remove(node);
          FreeVec(node);
        }
    }

  get(obj, MBA_Object_MUIObject, &guigroup);

  if (IsListEmpty(data->childs))
    {
      DoMethod(guigroup, MUIM_Group_InitChange, TAG_DONE);
      DoMethod(guigroup, OM_ADDMEMBER, HVSpace);
      DoMethod(guigroup, MUIM_Group_ExitChange, TAG_DONE);
    }

  return(1);
}

SAVEDS APTR mGetImageGroup(Class *cl, Object *obj, Msg msg)
{
    return(GroupObject,
           GroupFrame,
           MUIA_FrameTitle, "Group",
           MUIA_Draggable, TRUE,
           Child, HVSpace,
           End
           );
}

SAVEDS ULONG mAttributesEditGroup(Class *cl, Object *obj, struct MBP_AttributesEdit *msg)
{
  struct GroupData *data = INST_DATA(cl, obj);
  APTR   aux;
  char   **registertitles = msg->titles;

  registertitles[msg->number] = "Group";

  aux = VGroup,
    Child, HVSpace,
    Child, ColGroup(4),
      Child, Label("_Horizontal"),
      Child, data->CH_horiz = KeyCheckMark(data->horiz, 'h'),
      Child, Label("_Page Mode"),
      Child, data->CH_pagemode = KeyCheckMark(data->pagemode, 'p'),
      Child, Label("_Same Height"),
      Child, data->CH_sameheight = KeyCheckMark(data->sameheight, 's'),
      Child, Label("Same _Width"),
      Child, data->CH_samewidth = KeyCheckMark(data->samewidth, 'w'),
    End,
    Child, ColGroup(2),
      Child, Label("_Active Page"),
      Child, data->SL_activepage = KeySlider(0, 100, data->activepage,'a'),
      Child, Label("_Columns"),
      Child, data->SL_columns = KeySlider(0, 100, data->columns,'c'),
      Child, Label("_Rows"),
      Child, data->SL_rows = KeySlider(0, 100, data->rows,'r'),
    End,
    Child, HVSpace,
  End;

  if (aux)
    {
      set(data->SL_activepage, MUIA_Disabled, !data->pagemode);
      DoMethod(data->CH_pagemode, MUIM_Notify, MUIA_Selected, TRUE,
	       data->SL_activepage, 3, MUIM_Set, MUIA_Disabled, FALSE);
      DoMethod(data->CH_pagemode, MUIM_Notify, MUIA_Selected, FALSE,
	       data->SL_activepage, 3, MUIM_Set, MUIA_Disabled, TRUE);

      DoMethod(msg->group, OM_ADDMEMBER, aux);
      DoSuperMethod(cl, obj, MBM_Object_AttributesEdit,
                    msg->group, msg->number+1, registertitles);
    }
  else return(0);

  return(1);
}

SAVEDS ULONG mAttributesApplyGroup(Class *cl, Object *obj, Msg msg)
{
  struct GroupData *data = INST_DATA(cl, obj);

  get(data->CH_horiz, MUIA_Selected, &data->horiz);
  get(data->SL_activepage, MUIA_Slider_Level, &data->activepage);
  get(data->SL_columns, MUIA_Slider_Level, &data->columns);
  get(data->SL_rows, MUIA_Slider_Level, &data->rows);
  get(data->CH_pagemode, MUIA_Selected, &data->pagemode);
  get(data->CH_sameheight, MUIA_Selected, &data->sameheight);
  get(data->CH_samewidth, MUIA_Selected, &data->samewidth);

  return(DoSuperMethodA(cl, obj, msg));
}

SAVEDS ULONG mSaveGroup(Class *cl, Object *obj, Msg msg)
{
  struct GroupData *data = INST_DATA(cl, obj);
  char *aux;
  struct Node *node;

  /* Save group childs first */

  for(node=data->childs->lh_Head; node->ln_Succ; node=node->ln_Succ)
    {
      DoMethodA((Object*)node->ln_Name, msg);
    }

  /* Then save group object */
  get(obj, MBA_Object_Label, &aux);
  MB_BeginSaveObject(GROUP_TYPE, aux);
  DoMethod(obj, MBM_Object_SaveAttributes, TAG_DONE);
  MB_EndSaveObject();

  return(1);
}

SAVEDS ULONG mSaveAttributesGroup(Class *cl, Object *obj, Msg msg)
{
  struct GroupData *data = INST_DATA(cl, obj);
  char   *aux;
  struct Node *node;  

  MB_SaveAttribute("MUIA_Group_Horiz", TYPE_BOOL, data->horiz);
  MB_SaveAttribute("MUIA_Group_ActivePage", TYPE_INT, data->activepage);
  MB_SaveAttribute("MUIA_Group_Columns", TYPE_INT, data->columns);
  MB_SaveAttribute("MUIA_Group_PageMode", TYPE_BOOL, data->pagemode);
  MB_SaveAttribute("MUIA_Group_Rows", TYPE_INT, data->rows);
  MB_SaveAttribute("MUIA_Group_SameHeight", TYPE_BOOL, data->sameheight);
  MB_SaveAttribute("MUIA_Group_SameWidth", TYPE_BOOL, data->samewidth);

  for(node=data->childs->lh_Head; node->ln_Succ; node=node->ln_Succ)
    {
      get(node->ln_Name, MBA_Object_Label, &aux);
      MB_SaveAttribute("MUIA_Group_Child", TYPE_OBJ, (ULONG)aux);
    }

  return(DoSuperMethodA(cl, obj, msg));;
}

SAVEDS ULONG DispatcherGroup(struct IClass *cl,
                            Object *obj,
                            Msg msg)
{
  switch(msg->MethodID)
  {
    case OM_NEW:                    return(mNewGroup(cl, obj, (struct opSet *)msg));
    case OM_SET:                    return(mSetGroup(cl, obj, (struct opSet *)msg));
    case OM_GET:                    return(mGetGroup(cl, obj, (struct opGet *)msg));
    case OM_DISPOSE:                return(mDisposeGroup(cl, obj, msg));
    case MBM_Object_CreateEdition:  return((ULONG)mCreateEditionGroup(cl, obj, msg));
    case MBM_Object_CreateTest:     return((ULONG)mCreateTestGroup(cl, obj, msg));
    case MBM_Object_GetImage:       return((ULONG)mGetImageGroup(cl, obj, msg));
    case MBM_Group_AddObject:       return(mAddObject(cl, obj, (struct MBP_AddObj *)msg));
    case MBM_Group_RemObject:       return(mRemObject(cl, obj, (struct MBP_AddObj *)msg));
    case MBM_Object_AttributesEdit: return((ULONG)mAttributesEditGroup(cl, obj, (struct MBP_AttributesEdit *)msg));
    case MBM_Object_AttributesApply:return(mAttributesApplyGroup(cl, obj, msg));
    case MBM_Object_SaveAttributes: return(mSaveAttributesGroup(cl, obj, msg));
    case MBM_Object_Save:           return(mSaveGroup(cl, obj, msg));
    default:                        return(DoSuperMethodA(cl, obj, msg));
  }
}

void InitGroupClass(void)
{
  extern ULONG HookEntry();
  extern ULONG InitGUIGroupClass(void);

  if (GroupClass = MakeClass("MBGroupClass", "MBAreaClass", NULL, sizeof(struct GroupData), 0))
    {
      GroupClass->cl_Dispatcher.h_Entry = HookEntry;
      GroupClass->cl_Dispatcher.h_SubEntry = DispatcherGroup;
      AddClass(GroupClass);
    }
  InitGUIGroupClass();
}

void DisposeGroupClass(void)
{
  extern void DisposeGUIGroupClass(void);

  RemoveClass(GroupClass);
  FreeClass(GroupClass);
  DisposeGUIGroupClass();
}
