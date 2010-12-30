#include <dos/dos.h>
#include <clib/alib_protos.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/intuition_protos.h>
#include <clib/utility_protos.h>

#include <pragmas/exec_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/intuition_pragmas.h>
#include <pragmas/utility_pragmas.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <clib/muimaster_protos.h>
#include <libraries/mui.h>
#include <pragmas/muimaster_pragmas.h>

extern struct MUI_CustomClass *GUIGroupClass;

/* Init procedures */
extern void InitBaseClass(void);
extern ULONG InitGUIGroupClass(void);
extern void InitGroupClass(void);
extern void InitRessourceClass(void);
extern void InitAreaClass(void);
extern void InitButtonClass(void);
extern void InitWindowClass(void);
extern void InitPaletteClass(void);
extern void InitInspectorClass(void);
extern void InitProjectMgrClass(void);
extern void InitMUIBMainClass(void);
extern void InitTextClass(void);
extern void InitGaugeClass(void);

/* Dispose procedures */
extern void DisposeBaseClass(void);
extern void DisposeGUIGroupClass(void);
extern void DisposeGroupClass(void);
extern void DisposeRessourceClass(void);
extern void DisposeAreaClass(void);
extern void DisposeButtonClass(void);
extern void DisposeWindowClass(void);
extern void DisposePaletteClass(void);
extern void DisposeInspectorClass(void);
extern void DisposeProjectMgrClass(void);
extern void DisposeMUIBMainClass(void);
extern void DisposeTextClass(void);
extern void DisposeGaugeClass(void);
