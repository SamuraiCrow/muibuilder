#include <string.h>
#include <clib/dos_protos.h>

void extract_dir( char *filename )
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
	char *aux;

	aux = strrchr( filename, '.' );
	if (!aux) strcat( filename, extend );
	else if (strcmp( aux, extend )!=0) strcat( filename, extend );
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
		
