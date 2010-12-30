#include "builder2.h"
#include <stdarg.h>

BOOL RequestMessage( char *message )
{
	ULONG	req;
	BOOL	result;
	char	inside[256];

	sprintf( inside, "*%s|%s", GetMUIBuilderString(MSG_Ok), GetMUIBuilderString(MSG_Cancel) );
	req = MUI_Request( app, WI_current, 0, NULL, inside, message );
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
        MUI_Request( app, WI_current, 0, NULL, inside, message );
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

