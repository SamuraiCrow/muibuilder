/*
**
**	MUIBuilder.library
**
**	$VER: MUIBuilder.library 1.0
**
**		(c) copyright 1994
**		    Eric Totel
**
*/

#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

BOOL	MB_Open		( void 				);
void	MB_Close	( void 				);
void	MB_GetA		( struct TagItem *TagList 	);
void	MB_Get		( Tag Tag1, ... 		);
void	MB_GetVarInfoA	( struct TagItem *TagList 	);
void	MB_GetVarInfo	( Tag Tag1, ... 		);
void	MB_GetNextCode	( ULONG *, char ** 		);
void	MB_GetNextNotify( ULONG*, char ** 		);
