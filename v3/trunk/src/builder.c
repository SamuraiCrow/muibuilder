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

#include "builder.h"
#include "LoadSave/Save.h"

struct Library *UtilityBase = NULL;
struct Library *IntuitionBase = NULL;
struct Library *MUIMasterBase = NULL;
struct Library *TextFieldBase = NULL;

extern struct MUI_CustomClass *PaletteClass;
extern struct MUI_CustomClass *InspectorClass;
extern struct MUI_CustomClass *ProjectMgrClass;
extern struct MUI_CustomClass *MUIBMainClass;

Object *app, *inspector, *muibuilder, *palette;
Object *aboutmui;

void InitClasses(void)
{
  /* Public Classes */
  InitBaseClass();
  InitAreaClass();
  InitGroupClass();
  InitButtonClass();
  InitWindowClass();
  InitTextClass();
  InitGaugeClass();

  /* Private Classes */
  InitPaletteClass();
  InitProjectMgrClass();
  InitInspectorClass();
  InitMUIBMainClass();
}

void DisposeClasses(void)
{
  /* Public Classes */
  DisposeBaseClass();
  DisposeAreaClass();
  DisposeGroupClass();
  DisposeButtonClass();
  DisposeWindowClass();
  DisposeTextClass();
  DisposeGaugeClass();

  /* Private Classes */
  DisposePaletteClass();
  DisposeProjectMgrClass();
  DisposeInspectorClass();
  DisposeMUIBMainClass();
}

BOOL OpenLibs(void)
{
  IntuitionBase = OpenLibrary("intuition.library",0);
  UtilityBase = OpenLibrary("utility.library",0);
  MUIMasterBase = OpenLibrary("muimaster.library", 0);
  TextFieldBase = OpenLibrary("gadgets/textfield.gadget", 0);
  
  if (!(IntuitionBase&&UtilityBase&&MUIMasterBase&&TextFieldBase)) return(FALSE);;
  
  return(TRUE);
}

void CloseLibs(void)
{
  if (MUIMasterBase) CloseLibrary(MUIMasterBase);
  if (UtilityBase) CloseLibrary(UtilityBase);
  if (IntuitionBase) CloseLibrary(IntuitionBase);
  if (TextFieldBase) CloseLibrary(TextFieldBase);
}

int main(int argc, char* argv)
{
  ULONG sigs = 0;
  
  if (!OpenLibs())
    {
      CloseLibs();
      exit(20);
    }
  InitClasses();

  app = ApplicationObject,
    MUIA_Application_Base, "MUIBUILDER",
    MUIA_Application_Title, "MUIBuilder",
    MUIA_Application_Version, "$VER: MUIBuilder 3.0 (XX.XX.XX)",
    MUIA_Application_Copyright, "Eric Totel & Jérome Souquières",
    MUIA_Application_Description, "MUI Interface Builder",
    MUIA_Application_Author, "Eric Totel",
    SubWindow, palette = NewObject(PaletteClass->mcc_Class, NULL,
                                   MUIA_Window_ID, 2,
                                   TAG_DONE),
    SubWindow, inspector = NewObject(InspectorClass->mcc_Class, NULL,
                                     MUIA_Window_ID, 3,
                                     TAG_DONE),
  End;

  /* The main window MUST be created AFTER the secondary ones */
  /* because it controls them (closing/opening)               */
  if(muibuilder = NewObject(MUIBMainClass->mcc_Class, NULL,
                                    MUIA_Window_ID, 1,
                                    TAG_DONE))
    {
      DoMethod(app, OM_ADDMEMBER, muibuilder);
      set(muibuilder, MUIA_Window_Open, TRUE);
    }
  else
    {
      MUI_DisposeObject(app);
      app = NULL;
    }
  
  if (!app)
  {
    DisposeClasses();
    CloseLibs();
    exit(20);
  }


  set(muibuilder, MUIA_Window_Open, TRUE);

  while (DoMethod(app,MUIM_Application_NewInput,&sigs) != MUIV_Application_ReturnID_Quit)
    {
      if (sigs)
        {
          sigs = Wait(sigs | SIGBREAKF_CTRL_C);
          if (sigs & SIGBREAKF_CTRL_C) break;
        }
    }         
  
  MUI_DisposeObject(app);
  DisposeClasses();
  CloseLibs();
  return(0);
  
}
