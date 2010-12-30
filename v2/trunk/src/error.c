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
#include <stdarg.h>

BOOL RequestMessage( char *message )
{
	ULONG	req;
	BOOL	result;
	char	inside[256];

	sprintf( inside, "*%s|%s", GetMUIBuilderString(MSG_Ok), GetMUIBuilderString(MSG_Cancel) );
	req = MUI_Request( app, WI_current, 0, NULL, inside, message, NULL );
	result = ( req == 1 );
	return( result );
}


BOOL RequestMessageEasy( char *body , ...)
{
	ULONG	req;
        BOOL    result;
        struct EasyStruct es;
        va_list args;
	char	inside[256];

	sprintf(inside,"%s|%s", GetMUIBuilderString(MSG_Ok), GetMUIBuilderString(MSG_Cancel) );

        if (!IntuitionBase)
        {
                Write(Output(), "Need AmigaDos 2.0+\n", -1);
                return(FALSE);
        }

        /* setup the argument array */
        va_start( args, body );

        /* initialise the structure */
        es.es_StructSize = sizeof(struct EasyStruct);
        es.es_Flags = 0L;
        es.es_Title = "MUIBuilder";
        es.es_TextFormat = body;
        es.es_GadgetFormat = inside;

        /* display the requester */
        req = EasyRequestArgs( NULL, &es, NULL, args);

        /* free the arguments */
        va_end( args );

        result = ( req == 1 );
        return( result );
}

void ErrorMessage( char *message )
{
        char    inside[256];

        sprintf( inside, "*%s", GetMUIBuilderString(MSG_Ok) );
        MUI_Request( app, WI_current, 0, NULL, inside, message, NULL );
}

void ErrorMessageEasy(char *body, ...)
{
        struct EasyStruct es;
        va_list args;

        if (!IntuitionBase)
        {
                Write(Output(), "Need AmigaDos 2.0+\n", -1);
                return;
        }

        /* setup the argument array */
        va_start( args, body );

        /* initialise the structure */
        es.es_StructSize = sizeof(struct EasyStruct);
        es.es_Flags = 0L;
        es.es_Title = "MUIBuilder";
        es.es_TextFormat = body;
        es.es_GadgetFormat = "Ok";

        /* display the requester */
        EasyRequestArgs( NULL, &es, NULL, args);

        /* free the arguments */
        va_end( args );
}

