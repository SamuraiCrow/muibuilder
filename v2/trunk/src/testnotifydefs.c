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

/********************* Application ********************/
int NACTAppli[] =
        {
                2, MUIM_Application_ReturnID, NULL, NULL,
		2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit, NULL,
		3, MUIM_Set, MUIA_Application_Sleep, TRUE,
		3, MUIM_Set, MUIA_Application_Sleep, FALSE,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTAppli[] =
        {
		MUIA_Application_Iconified, TRUE,
		MUIA_Application_Iconified, FALSE,
		MUIA_Application_Active, TRUE,
		MUIA_Application_Active, FALSE,
                NULL, NULL,
        };

/*********************** Window ***********************/
int NACTWindow[] =
	{
		3, MUIM_Set, MUIA_Window_Open, TRUE,
		3, MUIM_Set, MUIA_Window_Open, FALSE,
		3, MUIM_Set, MUIA_Window_Activate, TRUE,
		2, MUIM_CallHook, NULL, NULL,
		3, MUIM_Set, MUIA_Window_ActiveObject, MUIV_Window_ActiveObject_None,
		3, MUIM_Set, MUIA_Window_ActiveObject, MUIV_Window_ActiveObject_Next,
		3, MUIM_Set, MUIA_Window_ActiveObject, MUIV_Window_ActiveObject_Prev,
		NULL, NULL, NULL, NULL
	};
int NEVTWindow[] =
	{
		MUIA_Window_CloseRequest, TRUE,
		MUIA_Window_Activate, TRUE,
		MUIA_Window_Activate, FALSE,
		NULL, NULL
	};

/*********************** Button ***********************/
int NACTButton[] =
	{
		3, MUIM_Set, MUIA_ShowMe		, FALSE,
		3, MUIM_Set, MUIA_ShowMe		, TRUE,
		3, MUIM_Set, MUIA_Disabled		, TRUE,
		3, MUIM_Set, MUIA_Disabled		, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject	, NULL,
		3, MUIM_Set, MUIA_Text_Contents		, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Text_Contents		, NULL,
		3, MUIM_Set, MUIA_Text_Contents		, NULL,
		2, MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};
int NEVTButton[] =
	{
		MUIA_Pressed, TRUE,
		MUIA_Pressed, FALSE,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
		MUIA_Timer, MUIV_EveryTime,
		NULL, NULL
	};

/******************* Group *************************/
int NACTGroup[] =
	{
		3, MUIM_Set, MUIA_ShowMe, FALSE,
		3, MUIM_Set, MUIA_ShowMe, TRUE,
		3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject, NULL,
		3, MUIM_Set, MUIA_Group_ActivePage, MUIV_TriggerValue,
		2, MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};
int NEVTGroup[] =
	{
		MUIA_Group_ActivePage, MUIV_EveryTime,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
		NULL, NULL
	};

/******************* String ************************/
int NACTString[] =
        {
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
		3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject, NULL,
		3, MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_String_Contents, NULL,
		3, MUIM_Set, MUIA_String_Contents, NULL,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTString[] =
        {
		MUIA_String_Acknowledge, MUIV_EveryTime,
		MUIA_String_BufferPos, MUIV_EveryTime,
		MUIA_String_Contents, MUIV_EveryTime,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
                NULL, NULL
        };

/****************** Listview ***********************/
int NACTListview[] =
        {
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
                3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject, NULL,
		1, MUIM_List_Clear, NULL, NULL,
		2, MUIM_List_Jump, NULL, NULL,
		2, MUIM_List_Redraw, MUIV_List_Redraw_All, NULL,
		2, MUIM_List_Redraw, MUIV_List_Redraw_Active, NULL,
		2, MUIM_List_Remove, MUIV_List_Remove_Active, NULL,
		1, MUIM_List_Sort, NULL, NULL,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTListview[] =
        {
		MUIA_Listview_DoubleClick, TRUE,
		MUIA_List_Active, MUIV_EveryTime,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
                NULL, NULL
        };

/******************* Gauge *************************/
int NACTGauge[] =
        {
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
                3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Gauge_Current, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Gauge_Current, NULL,
		3, MUIM_Set, MUIA_Gauge_Current, NULL,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTGauge[] =
        {
		MUIA_Gauge_Current, MUIV_EveryTime,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
                NULL, NULL
        };

/******************* Cycle *************************/
int NACTCycle[] =
        {
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
                3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject, NULL,
		3, MUIM_Set, MUIA_Cycle_Active, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Cycle_Active, NULL,
		3, MUIM_Set, MUIA_Cycle_Active, NULL,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTCycle[] =
        {
		MUIA_Cycle_Active, MUIV_EveryTime,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
                NULL, NULL
        };

/******************* Radio *************************/
int NACTRadio[] =
        {
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
                3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject, NULL,
		3, MUIM_Set, MUIA_Radio_Active, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Radio_Active, NULL,
		3, MUIM_Set, MUIA_Radio_Active, NULL,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTRadio[] =
        {
		MUIA_Radio_Active, MUIV_EveryTime,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
                NULL, NULL
        };

/******************* Label *************************/
int NACTLabel[] =
        {
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
                3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Text_Contents, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Text_Contents, NULL,
		3, MUIM_Set, MUIA_Text_Contents, NULL,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTLabel[] =
        {
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
                NULL, NULL
        };

/******************* Space *************************/
int NACTSpace[] =
        {
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTSpace[] =
        {
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
                NULL, NULL
        };

/******************* Check *************************/
int NACTCheck[] =
        {
		3, MUIM_Set, MUIA_Selected, TRUE,
		3, MUIM_Set, MUIA_Selected, FALSE,
		3, MUIM_Set, MUIA_Selected, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Selected, NULL,
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
                3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject, NULL,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTCheck[] =
        {
		MUIA_Selected, TRUE,
                MUIA_Selected, FALSE,
		MUIA_Selected, MUIV_EveryTime,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
                NULL, NULL
        };

/******************* Scale *************************/
int NACTScale[] =
        {
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTScale[] =
        {
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
                NULL, NULL
        };

/******************* Image *************************/
int NACTImage[] =
        {
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
                3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject, NULL,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTImage[] =
        {
                MUIA_Pressed, TRUE,
		MUIA_Pressed, FALSE,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
		NULL, NULL
        };

/******************* Slider ************************/
int NACTSlider[] =
        {
		3, MUIM_Set, MUIA_Slider_Level, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Slider_Level, NULL,
		3, NULL, NULL, NULL, /* not tested */
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
                3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject, NULL,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTSlider[] =
        {
		MUIA_Slider_Level, MUIV_EveryTime,
		MUIA_Slider_Reverse, MUIV_EveryTime,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
                NULL, NULL
        };

/******************* Text *************************/
int NACTText[] =
        {
		3, MUIM_Set, MUIA_Text_Contents, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Text_Contents, NULL,
		3, MUIM_Set, MUIA_Text_Contents, NULL,
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
                3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTText[] =
        {
		MUIA_Text_Contents, MUIV_TriggerValue,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
                NULL, NULL
        };

/******************* Prop *************************/
int NACTProp[] =
        {
		3, MUIM_Set, MUIA_Prop_First, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Prop_First, NULL,
		3, MUIM_Set, MUIA_Prop_First, NULL,
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
                3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject, NULL,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTProp[] =
        {
		MUIA_Prop_First, MUIV_EveryTime,
		MUIA_Prop_Entries, MUIV_EveryTime,
		MUIA_Prop_Visible, MUIV_EveryTime,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
                NULL, NULL
        };

/******************* ColorField *************************/
int NACTColorField[] =
        {
		3, MUIM_Set, MUIA_Colorfield_Red, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Colorfield_Red, NULL,
		3, MUIM_Set, MUIA_Colorfield_Red, NULL,
		3, MUIM_Set, MUIA_Colorfield_Blue, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Colorfield_Blue, NULL,
		3, MUIM_Set, MUIA_Colorfield_Blue, NULL,
		3, MUIM_Set, MUIA_Colorfield_Green, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Colorfield_Green, NULL,
		3, MUIM_Set, MUIA_Colorfield_Green, NULL,
                3, MUIM_Set, MUIA_ShowMe, FALSE,
                3, MUIM_Set, MUIA_ShowMe, TRUE,
                3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject, NULL,
		2, MUIM_CallHook, NULL, NULL,
                NULL, NULL, NULL, NULL
        };
int NEVTColorField[] =
        {
		MUIA_Colorfield_Red, MUIV_EveryTime,
		MUIA_Colorfield_Blue, MUIV_EveryTime,
		MUIA_Colorfield_Green, MUIV_EveryTime,
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
                NULL, NULL
        };

/******************* PopAsl *************************/
int NACTPopAsl[] =
	{
		3, MUIM_Set, MUIA_ShowMe, FALSE,
		3, MUIM_Set, MUIA_ShowMe, TRUE,
		3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		1, MUIM_Popstring_Open, NULL, NULL,
		2, MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};
int NEVTPopAsl[] =
	{
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
		NULL, NULL
	};

/******************* PopObject *************************/
int NACTPopObject[] =
	{
		3, MUIM_Set, MUIA_ShowMe, FALSE,
		3, MUIM_Set, MUIA_ShowMe, TRUE,
		3, MUIM_Set, MUIA_Disabled, TRUE,
                3, MUIM_Set, MUIA_Disabled, FALSE,
		3, MUIM_Set, MUIA_Window_ActiveObject, NULL,
		2, MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};
int NEVTPopObject[] =
	{
		MUIA_ShowMe, FALSE,
		MUIA_ShowMe, TRUE,
		MUIA_Disabled, TRUE,
		MUIA_Disabled, FALSE,
		NULL, NULL
	};

/***************** MenuStrip ***********************************/
int NACTMenuStrip[] =
	{
		3, MUIM_Set, MUIA_Menustrip_Enabled, TRUE,
		3, MUIM_Set, MUIA_Menustrip_Enabled, FALSE,
		2, MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};

int NEVTMenuStrip[] =
	{
		MUIA_Menustrip_Enabled, TRUE,
		MUIA_Menustrip_Enabled, FALSE,
		NULL, NULL,
	};

/***************** Menu ***********************************/
int NACTMenu[] =
	{
		3, MUIM_Set, MUIA_Menu_Enabled, TRUE,
		3, MUIM_Set, MUIA_Menu_Enabled, FALSE,
		3, MUIM_Set, MUIA_Menu_Title, NULL,
		3, MUIM_Set, MUIA_Menu_Title, NULL,
		3, MUIM_Set, MUIA_Menu_Title, NULL,
		2, MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};

int NEVTMenu[] =
	{
		MUIA_Menu_Enabled, TRUE,
		MUIA_Menu_Enabled, FALSE,
		NULL, NULL,
	};

/***************** MenuItem ***********************************/
int NACTMenuItem[] =
	{
		3, MUIM_Set, MUIA_Menuitem_Enabled, TRUE,
		3, MUIM_Set, MUIA_Menuitem_Enabled, FALSE,
		3, MUIM_Set, MUIA_Menuitem_Checked, TRUE,
		3, MUIM_Set, MUIA_Menuitem_Checked, FALSE,
		3, MUIM_Set, MUIA_Menuitem_Checkit, TRUE,
		3, MUIM_Set, MUIA_Menuitem_Checkit, FALSE,
		3, MUIM_Set, MUIA_Menuitem_Title, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Menuitem_Title, NULL,
		3, MUIM_Set, MUIA_Menuitem_Title, NULL,
		3, MUIM_Set, MUIA_Menuitem_Shortcut, MUIV_TriggerValue,
		3, MUIM_Set, MUIA_Menuitem_Shortcut, NULL,
		3, MUIM_Set, MUIA_Menuitem_Shortcut, NULL,
		3, MUIM_Set, MUIA_Menuitem_Toggle, TRUE,
		3, MUIM_Set, MUIA_Menuitem_Toggle, FALSE,
		2, MUIM_CallHook, NULL, NULL,
		NULL, NULL, NULL, NULL
	};

int NEVTMenuItem[] =
	{
		MUIA_Menuitem_Trigger, MUIV_EveryTime,
		MUIA_Menuitem_Enabled, TRUE,
		MUIA_Menuitem_Enabled, FALSE,
		MUIA_Menuitem_Checked, TRUE,
		MUIA_Menuitem_Checked, FALSE,
		MUIA_Menuitem_Checkit, TRUE,
		MUIA_Menuitem_Checkit, FALSE,
		NULL, NULL,
	};

/*************************** la totale *************************/

int *NEvenements[] =
	{
		NEVTAppli,
		NEVTWindow,
		NEVTGroup,
		NEVTButton,
		NEVTString,
		NEVTListview,
                NEVTGauge,
                NEVTCycle,
                NEVTRadio,
                NEVTLabel,
                NEVTSpace,
		NEVTCheck,
                NEVTScale,
                NEVTImage,
                NEVTSlider,
                NEVTText,
                NEVTProp,
                NEVTListview,
		NEVTSpace,
		NEVTColorField,
		NEVTPopAsl,
		NEVTPopObject,
		NEVTMenuStrip,
		NEVTMenu,
		NEVTMenuItem
	};
int *NActions[] =
	{
		NACTAppli,
		NACTWindow,
		NACTGroup,
		NACTButton,
		NACTString,
		NACTListview,
                NACTGauge,
                NACTCycle,
                NACTRadio,
                NACTLabel,
                NACTSpace,
		NACTCheck,
		NACTScale,
                NACTImage,
                NACTSlider,
                NACTText,
                NACTProp,
                NACTListview,
		NACTSpace,
		NACTColorField,
		NACTPopAsl,
		NACTPopObject,
		NACTMenuStrip,
		NACTMenu,
		NACTMenuItem
	};
