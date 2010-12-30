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

#ifndef NOTIFY_H
#define NOTIFY_H

#include "builder2.h"
#include "notifyconstants.h"

extern	void	CreateLabels( APTR, APTR );
extern	BOOL	GetConstant( event *, int, BOOL );

/* constantes */
#define ID_evt
#define ID_obj
#define ID_action

/********************* Application ********************/
extern char *ACTAppli[];
extern int  TYAppli[];
extern char *EVTAppli[];

/*********************** Window ***********************/
extern char *ACTWindow[];
extern int  TYWindow[];
extern char *EVTWindow[];

/*********************** Button ***********************/
extern char *ACTButton[];
extern int  TYButton[];
extern char *EVTButton[];

/******************* Group *************************/
extern char *ACTGroup[];
extern int  TYGroup[];
extern char *EVTGroup[];

/******************* String ************************/
extern char *ACTString[];
extern int  TYString[];
extern char *EVTString[];

/****************** ListView ***********************/
extern char *ACTListview[];
extern int  TYListview[];
extern char *EVTListview[];

/******************* Gauge *************************/
extern char *ACTGauge[];
extern int  TYGauge[];
extern char *EVTGauge[];

/******************* Cycle *************************/
extern char *ACTCycle[];
extern int  TYCycle[];
extern char *EVTCycle[];

/******************* Radio *************************/
extern char *ACTRadio[];
extern int  TYRadio[];
extern char *EVTRadio[];

/******************* Label *************************/
extern char *ACTLabel[];
extern int  TYLabel[];
extern char *EVTLabel[];

/******************* Space *************************/
extern char *ACTSpace[];
extern int  TYSpace[];
extern char *EVTSpace[];

/******************* Check *************************/
extern char *ACTCheck[];
extern int  TYCheck[];
extern char *EVTCheck[];

/******************* Scale *************************/
extern char *ACTScale[];
extern int  TYScale[];
extern char *EVTScale[];

/******************* Image *************************/
extern char *ACTImage[];
extern int  TYImage[];
extern char *EVTImage[];

/******************* Slider ************************/
extern char *ACTSlider[];
extern int  TYSlider[];
extern char *EVTSlider[];

/******************* Text *************************/
extern char *ACTText[];
extern int  TYText[];
extern char *EVTText[];

/******************* Prop *************************/
extern char *ACTProp[];
extern int  TYProp[];
extern char *EVTProp[];

/******************* ColorField *************************/
extern char *ACTColorField[];
extern int  TYColorField[];
extern char *EVTColorField[];

/******************* PopAsl *************************/
extern char *ACTPopAsl[];
extern int  TYPopAsl[];
extern char *EVTPopAsl[];

/******************* PopObject *************************/
extern char *ACTPopObject[];
extern int  TYPopObject[];
extern char *EVTPopObject[];

/******************* MenuStrip *****************************/
extern char *ACTMenuStrip[];
extern int  TYMenuStrip[];
extern char *EVTMenuStrip[];

/******************* Menu *****************************/
extern char *ACTMenu[];
extern int  TYMenu[];
extern char *EVTMenu[];

/******************* MenuItem *************************/
extern char *ACTMenuItem[];
extern int  TYMenuItem[];
extern char *EVTMenuItem[];

/*************************** la totale *************************/

extern char **Evenements[];
extern char **Actions[];
extern int *TYActions[];

#endif
