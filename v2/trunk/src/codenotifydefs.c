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

  $Id$$

***************************************************************************/

#include "builder2.h"
#include "codenotify.h"
#include "muib_file.h"

/********************* Application ********************/
int CACTAppli[] =
        {
                2, MB_MUIM_Application_ReturnID, NULL, NULL,
		2, MB_MUIM_Application_ReturnID, MB_MUIV_Application_ReturnID_Quit, NULL,
		3, MB_MUIM_Set, MB_MUIA_Application_Sleep, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Application_Sleep, FALSE,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTAppli[] =
        {
		MB_MUIA_Application_Iconified, TRUE,
		MB_MUIA_Application_Iconified, FALSE,
		MB_MUIA_Application_Active, TRUE,
		MB_MUIA_Application_Active, FALSE,
                NULL, NULL,
        };
int ArgEVTAppli[] =
	{
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		NULL
	};

/*********************** Window ***********************/
int CACTWindow[] =
	{
		3, MB_MUIM_Set, MB_MUIA_Window_Open, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Window_Open, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Window_Activate, TRUE,
		2, MB_MUIM_CallHook, NULL, NULL,
		3, MB_MUIM_Set, MB_MUIA_Window_ActiveObject, MB_MUIV_Window_ActiveObject_None,
		3, MB_MUIM_Set, MB_MUIA_Window_ActiveObject, MB_MUIV_Window_ActiveObject_Next,
		3, MB_MUIM_Set, MB_MUIA_Window_ActiveObject, MB_MUIV_Window_ActiveObject_Prev,
		NULL, NULL, NULL, NULL
	};
int CEVTWindow[] =
	{
		MB_MUIA_Window_CloseRequest, TRUE,
		MB_MUIA_Window_Activate, TRUE,
		MB_MUIA_Window_Activate, FALSE,
		NULL, NULL
	};
int ArgEVTWindow[] =
        {
                ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_MUI,
                NULL
        };

/*********************** Button ***********************/
int CACTButton[] =
	{
		3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
		3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Window_ActiveObject, NULL,
		3, MB_MUIM_Set, MB_MUIA_Text_Contents, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Text_Contents, NULL,
		3, MB_MUIM_Set, MB_MUIA_Text_Contents, NULL,
		2, MB_MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};
int CEVTButton[] =
	{
		MB_MUIA_Pressed, TRUE,
		MB_MUIA_Pressed, FALSE,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
		MB_MUIA_Timer, MB_MUIV_EveryTime,
		NULL, NULL
	};
int ArgEVTButton[] =
        {
                ARG_BOOL,
                ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_MUI,
                NULL
        };

/******************* Group *************************/
int CACTGroup[] =
	{
		3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
		3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Window_ActiveObject, NULL,
		3, MB_MUIM_Set, MB_MUIA_Group_ActivePage, MB_MUIV_TriggerValue,
		2, MB_MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};
int CEVTGroup[] =
	{
		MB_MUIA_Group_ActivePage, MB_MUIV_EveryTime,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
		NULL, NULL
	};
int ArgEVTGroup[] =
        {
		ARG_MUI,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* String ************************/
int CACTString[] =
        {
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Window_ActiveObject, NULL,
		3, MB_MUIM_Set, MB_MUIA_String_Contents, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_String_Contents, NULL,
		3, MB_MUIM_Set, MB_MUIA_String_Contents, NULL,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTString[] =
        {
		MB_MUIA_String_Acknowledge, MB_MUIV_EveryTime,
		MB_MUIA_String_BufferPos, MB_MUIV_EveryTime,
		MB_MUIA_String_Contents, MB_MUIV_EveryTime,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTString[] =
        {
                ARG_MUI,
		ARG_MUI,
		ARG_MUI,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/****************** Listview ***********************/
int CACTListview[] =
        {
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Window_ActiveObject, NULL,
		1, MB_MUIM_List_Clear, NULL, NULL,
		2, MB_MUIM_List_Jump, NULL, NULL,
		2, MB_MUIM_List_Redraw, MB_MUIV_List_Redraw_All, NULL,
		2, MB_MUIM_List_Redraw, MB_MUIV_List_Redraw_Active, NULL,
		2, MB_MUIM_List_Remove, MB_MUIV_List_Remove_Active, NULL,
		1, MB_MUIM_List_Sort, NULL, NULL,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTListview[] =
        {
		MB_MUIA_Listview_DoubleClick, TRUE,
		MB_MUIA_List_Active, MB_MUIV_EveryTime,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTListview[] =
        {
                ARG_BOOL,
		ARG_MUI,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* Gauge *************************/
int CACTGauge[] =
        {
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Gauge_Current, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Gauge_Current, NULL,
		3, MB_MUIM_Set, MB_MUIA_Gauge_Current, NULL,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTGauge[] =
        {
		MB_MUIA_Gauge_Current, MB_MUIV_EveryTime,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTGauge[] =
        {
		ARG_MUI,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* Cycle *************************/
int CACTCycle[] =
        {
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Window_ActiveObject, NULL,
		3, MB_MUIM_Set, MB_MUIA_Cycle_Active, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Cycle_Active, NULL,
		3, MB_MUIM_Set, MB_MUIA_Cycle_Active, NULL,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTCycle[] =
        {
		MB_MUIA_Cycle_Active, MB_MUIV_EveryTime,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTCycle[] =
        {
                ARG_MUI,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* Radio *************************/
int CACTRadio[] =
        {
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Window_ActiveObject, NULL,
		3, MB_MUIM_Set, MB_MUIA_Radio_Active, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Radio_Active, NULL,
		3, MB_MUIM_Set, MB_MUIA_Radio_Active, NULL,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTRadio[] =
        {
		MB_MUIA_Radio_Active, MB_MUIV_EveryTime,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTRadio[] =
        {
                ARG_MUI,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* Label *************************/
int CACTLabel[] =
        {
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Text_Contents, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Text_Contents, NULL,
		3, MB_MUIM_Set, MB_MUIA_Text_Contents, NULL,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTLabel[] =
        {
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTLabel[] =
        {
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* Space *************************/
int CACTSpace[] =
        {
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTSpace[] =
        {
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
                NULL, NULL
        };
int ArgEVTSpace[] =
        {
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* Check *************************/
int CACTCheck[] =
        {
		3, MB_MUIM_Set, MB_MUIA_Selected, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Selected, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Selected, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Selected, NULL,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Window_ActiveObject, NULL,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTCheck[] =
        {
		MB_MUIA_Selected, TRUE,
                MB_MUIA_Selected, FALSE,
		MB_MUIA_Selected, MB_MUIV_EveryTime,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTCheck[] =
        {
                ARG_BOOL,
                ARG_BOOL,
		ARG_MUI,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* Scale *************************/
int CACTScale[] =
        {
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTScale[] =
        {
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
                NULL, NULL
        };
int ArgEVTScale[] =
        {
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* Image *************************/
int CACTImage[] =
        {
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTImage[] =
        {
		MB_MUIA_Pressed, TRUE,
		MB_MUIA_Pressed, FALSE,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTImage[] =
        {
                ARG_BOOL,
		ARG_BOOL,
                ARG_BOOL,
		ARG_BOOL,
                ARG_BOOL,
		ARG_BOOL,
		NULL
        };

/******************* Slider ************************/
int CACTSlider[] =
        {
		3, MB_MUIM_Set, MB_MUIA_Slider_Level, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Slider_Level, NULL,
		3, MB_MUIM_Set, MB_MUIA_Slider_Level, NULL,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTSlider[] =
        {
		MB_MUIA_Slider_Level, MB_MUIV_EveryTime,
		MB_MUIA_Slider_Level, MB_MUIV_EveryTime,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTSlider[] =
        {
                ARG_MUI,
		ARG_MUI,
                ARG_BOOL,
		ARG_BOOL,
                ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* Text *************************/
int CACTText[] =
        {
		3, MB_MUIM_Set, MB_MUIA_Text_Contents, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Text_Contents, NULL,
		3, MB_MUIM_Set, MB_MUIA_Text_Contents, NULL,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTText[] =
        {
		MB_MUIA_Text_Contents, MB_MUIV_EveryTime,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTText[] =
        {
		ARG_MUI,
                ARG_BOOL,
		ARG_BOOL,
                ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* Prop *************************/
int CACTProp[] =
        {
		3, MB_MUIM_Set, MB_MUIA_Prop_First, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Prop_First, NULL,
		3, MB_MUIM_Set, MB_MUIA_Prop_First, NULL,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTProp[] =
        {
		MB_MUIA_Prop_First, MB_MUIV_EveryTime,
		MB_MUIA_Prop_Entries, MB_MUIV_EveryTime,
		MB_MUIA_Prop_Visible, MB_MUIV_EveryTime,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTProp[] =
        {
                ARG_MUI,
		ARG_MUI,
		ARG_MUI,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* ColorField *************************/
int CACTColorField[] =
        {
		3, MB_MUIM_Set, MB_MUIA_Colorfield_Red, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Colorfield_Red, NULL,
		3, MB_MUIM_Set, MB_MUIA_Colorfield_Red, NULL,
		3, MB_MUIM_Set, MB_MUIA_Colorfield_Blue, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Colorfield_Blue, NULL,
		3, MB_MUIM_Set, MB_MUIA_Colorfield_Blue, NULL,
		3, MB_MUIM_Set, MB_MUIA_Colorfield_Green, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Colorfield_Green, NULL,
		3, MB_MUIM_Set, MB_MUIA_Colorfield_Green, NULL,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
                3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		2, MB_MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int CEVTColorField[] =
        {
		MB_MUIA_Colorfield_Red, MB_MUIV_EveryTime,
		MB_MUIA_Colorfield_Blue, MB_MUIV_EveryTime,
		MB_MUIA_Colorfield_Green, MB_MUIV_EveryTime,
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
                NULL, NULL
        };
int ArgEVTColorField[] =
        {
                ARG_MUI,
		ARG_MUI,
		ARG_MUI,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* PopAsl *************************/
int CACTPopAsl[] =
	{
		3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
		3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		1, MB_MUIM_Popstring_Open, NULL, NULL,
		2, MB_MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};
int CEVTPopAsl[] =
	{
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
		NULL, NULL
	};
int ArgEVTPopAsl[] =
        {
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/******************* PopObject *************************/
int CACTPopObject[] =
	{
		3, MB_MUIM_Set, MB_MUIA_ShowMe, FALSE,
		3, MB_MUIM_Set, MB_MUIA_ShowMe, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Disabled, TRUE,
                3, MB_MUIM_Set, MB_MUIA_Disabled, FALSE,
		2, MB_MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};
int CEVTPopObject[] =
	{
		MB_MUIA_ShowMe, FALSE,
		MB_MUIA_ShowMe, TRUE,
		MB_MUIA_Disabled, TRUE,
		MB_MUIA_Disabled, FALSE,
		NULL, NULL
	};
int ArgEVTPopObject[] =
        {
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
                NULL
        };

/***************** MenuStrip ***********************************/
int CACTMenuStrip[] =
	{
		3, MB_MUIM_Set, MB_MUIA_Menustrip_Enabled, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Menustrip_Enabled, FALSE,
		2, MB_MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};

int CEVTMenuStrip[] =
	{
		MB_MUIA_Menustrip_Enabled, TRUE,
		MB_MUIA_Menustrip_Enabled, FALSE,
		NULL, NULL,
	};

int ArgEVTMenuStrip[] =
	{
		ARG_BOOL,
		ARG_BOOL,
		NULL
	};

/***************** Menu ***********************************/
int CACTMenu[] =
	{
		3, MB_MUIM_Set, MB_MUIA_Menu_Enabled, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Menu_Enabled, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Menu_Title, NULL,
		3, MB_MUIM_Set, MB_MUIA_Menu_Title, NULL,
		3, MB_MUIM_Set, MB_MUIA_Menu_Title, NULL,
		2, MB_MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};

int CEVTMenu[] =
	{
		MB_MUIA_Menu_Enabled, TRUE,
		MB_MUIA_Menu_Enabled, FALSE,
		NULL, NULL,
	};

int ArgEVTMenu[] =
	{
		ARG_BOOL,
		ARG_BOOL,
		NULL
	};

/***************** MenuItem ***********************************/
int CACTMenuItem[] =
	{
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Enabled, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Enabled, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Checked, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Checked, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Checkit, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Checkit, FALSE,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Title, MB_MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Title, NULL,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Title, NULL,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Shortcut, MUIV_TriggerValue,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Shortcut, NULL,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Shortcut, NULL,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Toggle, TRUE,
		3, MB_MUIM_Set, MB_MUIA_Menuitem_Toggle, FALSE,
		2, MB_MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};

int CEVTMenuItem[] =
	{
		MB_MUIA_Menuitem_Trigger, MB_MUIV_EveryTime,
		MB_MUIA_Menuitem_Enabled, TRUE,
		MB_MUIA_Menuitem_Enabled, FALSE,
		MB_MUIA_Menuitem_Checked, TRUE,
		MB_MUIA_Menuitem_Checked, FALSE,
		MB_MUIA_Menuitem_Checkit, TRUE,
		MB_MUIA_Menuitem_Checkit, FALSE,
		2, MB_MUIM_CallHook, NULL, NULL,
		NULL, NULL,
	};

int ArgEVTMenuItem[] =
	{
		ARG_MUI,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		ARG_BOOL,
		NULL
	};

/*************************** la totale *************************/

int *CEvenements[] =
	{
		CEVTAppli,
		CEVTWindow,
		CEVTGroup,
		CEVTButton,
		CEVTString,
		CEVTListview,
                CEVTGauge,
                CEVTCycle,
                CEVTRadio,
                CEVTLabel,
                CEVTSpace,
		CEVTCheck,
                CEVTScale,
                CEVTImage,
                CEVTSlider,
                CEVTText,
                CEVTProp,
                CEVTListview,	/* ListView = DirList */
		CEVTSpace,	/* Rectangle = Space */
		CEVTColorField,
		CEVTPopAsl,
		CEVTPopObject,
		CEVTMenuStrip,
		CEVTMenu,
		CEVTMenuItem
	};
int *CActions[] =
	{
		CACTAppli,
		CACTWindow,
		CACTGroup,
		CACTButton,
		CACTString,
		CACTListview,
                CACTGauge,
                CACTCycle,
                CACTRadio,
                CACTLabel,
                CACTSpace,
		CACTCheck,
		CACTScale,
                CACTImage,
                CACTSlider,
                CACTText,
                CACTProp,
                CACTListview,	/* ListView = DirList */
		CACTSpace,	/* Rectangle = Space */
		CACTColorField,
		CACTPopAsl,
		CACTPopObject,
		CACTMenuStrip,
		CACTMenu,
		CACTMenuItem
	};
int *ArgEVT[] =
        {
                ArgEVTAppli,
                ArgEVTWindow,
                ArgEVTGroup,
                ArgEVTButton,
                ArgEVTString,
                ArgEVTListview,
                ArgEVTGauge,
                ArgEVTCycle,
                ArgEVTRadio,
                ArgEVTLabel,
                ArgEVTSpace,
                ArgEVTCheck,
                ArgEVTScale,
                ArgEVTImage,
                ArgEVTSlider,
                ArgEVTText,
                ArgEVTProp,
                ArgEVTListview,	/* ListView = DirList */
		ArgEVTSpace,	/* Rectangle = Space */
		ArgEVTColorField,
		ArgEVTPopAsl,
		ArgEVTPopObject,
		ArgEVTMenuStrip,
		ArgEVTMenu,
		ArgEVTMenuItem
        };
