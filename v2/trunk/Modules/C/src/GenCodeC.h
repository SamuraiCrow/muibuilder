/* Prototypes */
#include <clib/alib_protos.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <exec/memory.h>

/* ANSI */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Pragmas */
#include <pragmas/dos_pragmas.h>

/* MUIBuilder library */
#include "MB.h"
#include "MB_pragmas.h"
#include "MB_protos.h"

/* File name Utilities */
#include "file.h"

struct Library * DOSBase = NULL;
