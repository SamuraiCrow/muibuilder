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

#ifndef BUILDER_H
#define BUILDER_H

/*************************************************************/
/* Includes and other common stuff for the MUI demo programs */
/*************************************************************/

/* MUI */
#include <libraries/mui.h>

/* Prototypes */
#include <proto/alib.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/muimaster.h>
#include <proto/asl.h>
#include <proto/icon.h>
#include <proto/gadtools.h>

/* ANSI C */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Locale */
#include "MUIBuilder_cat.h"

#include <exec/memory.h>

/********************************/
/* Propre au programme lui-meme */
/********************************/

#include "queue.h"
#include "file.h"

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#define ID_INFO         1
#define ID_END          2
#define ID_NEWWIN       3
#define ID_OKWIN        4
#define ID_NEWGRP       5
#define ID_TEST         6
#define ID_FINTEST      7
#define ID_NEWOBJ       8
#define ID_KEYBUTTON    9
#define ID_LISTVIEW     10
#define ID_STRING       11
#define ID_GROUP        12
#define ID_DBCLICK      13
#define ID_DBCLICK2     14
#define ID_DELETE       15
#define ID_DELETEGRP    16
#define ID_GENERATE     17
#define ID_GAUGE        18
#define ID_DOWN         19
#define ID_UP           20
#define ID_CYCLE        21
#define ID_GENERATEWIN  22
#define ID_RADIO        23
#define ID_LABEL        30
#define ID_SPACE        31
#define ID_SAVE         32
#define ID_LOAD         33
#define ID_CHECK        34
#define ID_EDIT         35
#define ID_CONFIG       36
#define ID_APPLI        37
#define ID_GUIDE        38
#define ID_MERGE        39
#define ID_NEWAPPLI     40
#define ID_SAVEAS       41
#define ID_NOTIFY       42
#define ID_NEW_SUBMENU  43

#define TY_APPLI        0
#define TY_WINDOW       1
#define TY_GROUP        2
#define TY_KEYBUTTON    3
#define TY_STRING       4
#define TY_LISTVIEW     5
#define TY_GAUGE        6
#define TY_CYCLE        7
#define TY_RADIO        8
#define TY_LABEL        9
#define TY_SPACE        10

#define GR_ROOT         1
#define GR_NOROOT       2

typedef struct help1
{
    int nb_char;
    char title[80];
    APTR content;
    BOOL generated;
} help;

typedef struct area1
{
    BOOL Hide;
    BOOL Disable;
    BOOL InputMode;
    BOOL Phantom;
    int Weight;
    int Background;
    int Frame;
    char key;
    char TitleFrame[80];
} area;

typedef struct menu1
{
    int id;
    char label[80];
    APTR father;
    BOOL generated;
    help Help;
    queue *notify;
    queue *notifysource;
    APTR muiobj;
    char name[80];
    char key;
    BOOL menu_enable;
    BOOL check_enable;
    BOOL check_state;
    BOOL Toggle;
    queue *childs;
    BOOL fold;
} menu;

typedef struct conf1
{
    BOOL phantom;
    BOOL request;
    char langage[512];
    char editor[128];
    BOOL icons;
    BOOL depth;
    char tree_char;
    int columns;
    BOOL labels;
} conf;

typedef struct appli1
{
    int id;
    char label[80];
    APTR father;
    BOOL generated;
    help Help;
    queue *notify;
    queue *notifysource;
    APTR muiobj;
    char author[80];
    char base[80];
    char title[80];
    char version[80];
    char copyright[80];
    char description[80];
    char helpfile[256];
    queue *Idents;
    queue *Functions;
    queue *Constants;
    queue *Variables;
    menu *appmenu;
} appli;

typedef struct object1
{
    int id;
    char label[80];
    APTR father;
    BOOL generated;
    help Help;
    queue *notify;
    queue *notifysource;
    APTR muiobj;
} object;

typedef struct group1
{
    int id;
    char label[80];
    APTR father;
    BOOL generated;
    help Help;
    queue *notify;
    queue *notifysource;
    APTR muiobj;
    area Area;
    BOOL root;
    BOOL horizontal;
    BOOL registermode;
    BOOL sameheight;
    BOOL samesize;
    BOOL samewidth;
    BOOL virtual;
    BOOL columns;
    BOOL rows;
    BOOL horizspacing;
    BOOL vertspacing;
    int number;
    queue *child;
    int horizspace;
    int vertspace;
    queue *entries;
    char *registertitles[200];
    BOOL deplie;
} group;

typedef struct window1
{
    int id;
    char label[80];
    APTR father;
    BOOL generated;
    help Help;
    queue *notify;
    queue *notifysource;
    APTR muiobj;
    char title[80];
    group root;
    queue *groups;
    BOOL appwindow;
    BOOL backdrop;
    BOOL borderless;
    BOOL closegadget;
    BOOL depthgadget;
    BOOL dragbar;
    BOOL sizegadget;
    BOOL initopen;
    BOOL deplie;
    queue *chain;
    menu *menu;
    APTR muichain;
    BOOL nomenu;
    BOOL needmouse;
} window;

extern CONST_STRPTR GetStr(APTR);
extern BOOL NewGroup(group *, int, BOOL);
extern BOOL NewWindow(window *, BOOL, object *);
extern APTR NewChild(APTR, BOOL);
extern void TestWindow(window *);
extern APTR CreateWindow(APTR);
extern void DeleteObject(APTR);
extern void DeleteLabel(queue *, APTR);
extern void ErrorMessage(CONST_STRPTR);
extern void CodeCreate(object *, FILE *);
extern void GenerateCode(queue *);
extern void GenerateCodeWindow(APTR);
extern void EXIT_PRG(void);
extern void SaveObject(APTR, FILE *);
extern void SaveApplication(BOOL);
extern void LoadApplication(void);
extern APTR LoadObject(FILE *, APTR, int);
extern void LoadFile(char *);
extern BOOL RequestMessage(CONST_STRPTR);
extern void Code(void);
extern void Guide(void);
extern void InitLocale(void);
extern void CloseLocale(void);
extern void NextLine(FILE *);
extern BOOL LoadRequester(CONST_STRPTR, char *, char *);
extern void ErrorMessageEasy(CONST_STRPTR);

extern char guidenamefile[256];
extern char guidenamedir[128];
extern int nb_modules;
extern char savedir[256];
extern char savefile[256];
extern char get_string[80];
extern char real_getstring[100];

#endif
