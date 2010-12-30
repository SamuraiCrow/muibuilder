#include "LoadSave/AttributeTypes.h"

/*
 *   GUI Objects
 *
 * Attribute Base = TAG_USER
 * Method    Base = TAG_USER + 1000
 */



/* Attributes */

#define MBA_Object_Label      (TAG_USER + 1) /* char*   (isg) */
#define MBA_Object_MUIObject  (TAG_USER + 2) /* Object* (.sg) */
#define MBA_Object_Help       (TAG_USER + 3) /* char*   (isg) */
#define MBA_Object_Menu       (TAG_USER + 4) /* Object* (isg) */
#define MBA_GUIObject_Object  (TAG_USER + 5) /* Object* (isg) */
#define MBA_Object_Type       (TAG_USER + 6) /* ULONG */

/* Methods */

#define MBM_Object_CreateEdition    (TAG_USER + 1001) /* return Object*  */
#define MBM_Object_CreateTest       (TAG_USER + 1002) /* return Object*  */
#define MBM_Object_GetImage         (TAG_USER + 1003) /* return Object*  */
#define MBM_Object_AttributesEdit   (TAG_USER + 1004) /* return Object*  */
#define MBM_Object_AttributesApply  (TAG_USER + 1005) /* return void     */
#define MBM_Object_Save             (TAG_USER + 1006) /* return void     */
#define MBM_Object_SaveAttributes   (TAG_USER + 1007) /* return void     */

/* Constants */

#define MBV_Object_Type_Standard 0
#define MBV_Object_Type_Window   1

/* Parameters */

struct MBP_AttributesEdit
{
  ULONG MethodID;
  Object *group;
  ULONG number;
  char  **titles;
};

struct MBP_CreateEdition
{
  ULONG MethodID;
  Class *cl;
  UBYTE *ID;
  struct TagItem tags;
};

struct MBP_Save
{
  ULONG MethodID;
  ULONG SaveAttributesOnly;
};

/*-----------*
 *   Area    *
 *-----------*/

/* Attributes */

#define MBA_Area_Show         (TAG_USER + 20) /* BOOL  ... */
#define MBA_Area_Disabled     (TAG_USER + 21) /* BOOL  ... */
#define MBA_Area_InputMode    (TAG_USER + 22) /* ULONG i.g */
#define MBA_Area_PhantomHoriz (TAG_USER + 23) /* BOOL  ..g */

#define MBA_Area_Background   (TAG_USER + 25) /* ULONG i.. */
#define MBA_Area_Frame        (TAG_USER + 26) /* ULONG i.g */
#define MBA_Area_FrameTitle   (TAG_USER + 28) /* char* ..g */
#define MBA_Area_Font         (TAG_USER + 29) /* ULONG ..g */
#define MBA_Area_Selected     (TAG_USER + 30) /* ULONG i.. */
#define MBA_Area_ShowSelState (TAG_USER + 31) /* ULONG i.g */
#define MBA_Area_ShortHelp    (TAG_USER + 32) /* char* .sg */

/* Methods */

/*-------------*
 *   Group     *
 *-------------*/

/* Attributes */

#define MBA_Group_Horiz     (TAG_USER + 40) /* BOOL isg */
#define MBA_Group_IsEmpty   (TAG_USER + 41) /* BOOL ..g */

/* Methods */

#define MBM_Group_AddObject    (TAG_USER + 1040) /* struct MBP_AddObj */
#define MBM_Group_UpdateObject (TAG_USER + 1041) /* struct MBP_AddObj */
#define MBM_Group_RemObject    (TAG_USER + 1042) /* struct MBP_AddObj */

/* Structures */
struct MBP_AddObj
{
  ULONG   MethodID;
  Object* source;
  Object* position;
};

/*--------------*
 *   Button     *
 *--------------*/

/* Attributes */
#define MBA_Button_Title    (TAG_USER + 50) /* char* ..g */

/*--------------*
 *   Window     *
 *--------------*/

/* Attributes */
#define MBA_Window_Title     (TAG_USER + 60) /* char* isg */
#define MBA_Window_RootGroup (TAG_USER + 61) /* char* isg */

/*--------------*
 *   Text       *
 *--------------*/

/* Attributes */
#define MBA_Text_Contents    (TAG_USER + 70) /* char* ..g */

/*--------------*
 *   Gauge      *
 *--------------*/

/* Attributes */
#define MBA_Gauge_InfoText   (TAG_USER + 70) /* char* ..g */

/*---------------------------------------------------*
 *              MUIBuilder Objects                   *
 *---------------------------------------------------*/

/* Attribute Base = TAG_USER + 20000
 * Method    Base = TAG_USER + 30000
 */


/*------------------*
 * Ressource Object *
 *------------------*/

/* Attributes */

#define MBA_Ressource_Name    (TAG_USER + 20001) /* char* */
#define MBA_Ressource_Type    (TAG_USER + 20002) /* char* */

/* Methods */

#define MBM_Ressource_Edit     (TAG_USER + 30001)
#define MBM_Ressource_Save     (TAG_USER + 30002)
#define MBM_Ressource_Load     (TAG_USER + 30003)

/*------------------*
 * Inspector Object *
 *------------------*/

/* Attributes */

#define MBA_Inspector_CurrentObject     (TAG_USER + 20010)
#define MBA_Inspector_NotifyDestination (TAG_USER + 20011)

/*-------------------*
 * ProjectMgr Object *
 *-------------------*/

/* Attributes */

#define MBA_ProjectMgr_CurrentObject (TAG_USER + 20020)
#define MBA_ProjectMgr_Name          (TAG_USER + 20021)

/* Methods */
#define MBM_ProjectMgr_AddRessource (TAG_USER + 30020)
#define MBM_ProjectMgr_Save         (TAG_USER + 30021)
#define MBM_ProjectMgr_Load         (TAG_USER + 30022)

struct MBP_AddRessource
{
  ULONG Method;
  Object *obj;
};

/*-------------------*
 *  MUIBMain Object  *
 *-------------------*/

/* Attributes */
#define MBA_MUIBMain_CurrentProject (TAG_USER + 20030)

/* Methods */
#define MBM_MUIBMain_AddProject (TAG_USER + 30030)
#define MBM_MUIBMain_Save       (TAG_USER + 30031)
#define MBM_MUIBMain_Load       (TAG_USER + 30032)

struct MBP_AddProject
{
  ULONG Method;
  Object *obj;
};
