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
#include <proto/alib.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/utility.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <proto/muimaster.h>
#include <libraries/mui.h>

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
