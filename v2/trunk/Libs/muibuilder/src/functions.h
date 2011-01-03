/* Prototypes */
#include <clib/alib_protos.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/utility_protos.h>
#include <exec/memory.h>

/* ANSI */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Pragmas */
#include <pragmas/dos_pragmas.h>
#include <pragmas/exec_pragmas.h>
#include <pragmas/utility_pragmas.h>

struct Library *DOSBase = NULL;
struct Library *UtilityBase = NULL;

/* MB_GetA Tags */
#define MUIB_FileName 		(TAG_USER + 200)
#define MUIB_CatalogName 	(TAG_USER + 201)
#define MUIB_GetStringName	(TAG_USER + 202)
#define MUIB_Code		(TAG_USER + 203)
#define MUIB_Environment	(TAG_USER + 204)
#define MUIB_Locale		(TAG_USER + 205)
#define MUIB_Declarations	(TAG_USER + 206)
#define MUIB_VarNumber		(TAG_USER + 207)
#define MUIB_Notifications	(TAG_USER + 212)
#define MUIB_Application        (TAG_USER + 213)

/* MB_GetVarInfoA Tags */
#define MUIB_VarSize		(TAG_USER + 208)
#define MUIB_VarName		(TAG_USER + 209)
#define MUIB_VarType		(TAG_USER + 210)
#define MUIB_VarInitPtr		(TAG_USER + 211)

/* types of datas */

	/* variables types */
#define TYPEVAR_BOOL		1
#define TYPEVAR_INT		2
#define TYPEVAR_STRING		3
#define TYPEVAR_TABSTRING	4
#define TYPEVAR_PTR		5
#define TYPEVAR_HOOK		6

	/* Types of Code */
#define TC_CREATEOBJ		1
#define TC_ATTRIBUT		2
#define TC_END			3
#define TC_FUNCTION		4
#define TC_STRING		5
#define TC_INTEGER		6
#define TC_CHAR			7
#define TC_VAR_AFFECT		8
#define TC_VAR_ARG		9
#define TC_END_FUNCTION		10
#define TC_BOOL			11
#define	TC_MUIARG		12
#define TC_OBJFUNCTION		13
#define TC_LOCALESTRING		14
#define TC_EXTERNAL_CONSTANT    15
#define TC_EXTERNAL_FUNCTION    16
#define TC_MUIARG_ATTRIBUT      17
#define TC_MUIARG_FUNCTION	18
#define TC_MUIARG_OBJ		19
#define TC_BEGIN_NOTIFICATION	20
#define TC_END_NOTIFICATION	21
#define TC_EXTERNAL_VARIABLE    22
#define TC_MUIARG_OBJFUNCTION	23
