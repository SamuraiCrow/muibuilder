#include "builder2.h"
#include "NotifyConstants.h"

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
