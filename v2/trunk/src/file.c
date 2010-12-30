#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <clib/dos_protos.h>

void extract_catalog( char *filename )
{
	char *aux;

	aux = strrchr( filename, '/' );
	if (!aux) aux = strrchr( filename, ':' );
	if (aux)
	{
		aux++;
		*aux = '\0';
	}
	else filename[0] = '\0';
}

void extract_file( char * path, char * filename )
{
	strcpy( filename, FilePart( path ) );
}

void add_extend( char *filename, char * extend )
{
	char *aux, *aux2;
	char temp[512];
	char temp2[20];

	strncpy( temp2, extend, 20 );
	aux2 = temp2;
	while (*aux2 != '\0' )
	{
		if (islower(*aux2)) *aux2 = toupper(*aux2);
		aux2++;
	}

	aux = strrchr( filename, '.' );
	if (!aux) strcat( filename, extend );
	else
	{
		strncpy( temp, aux, 512 );
		aux2 = temp;
		while (*aux2 != '\0')
		{
			if (islower(*aux2)) *aux2 = toupper(*aux2);
			aux2++;
		}
		if (strcmp( temp, temp2 )!=0) strcat( filename, extend );
	}
}

void remove_extend( char *filename )
{
	char *aux;

	aux = strrchr( filename, '.' );
	if (aux) *aux='\0';
}

void change_extend( char *filename, char * extend )
{
	remove_extend( filename );
	add_extend( filename, extend );
}
		
