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

  $Id$

***************************************************************************/

#include <clib/exec_protos.h>
#include <exec/memory.h>
#include <stdio.h>
#include <string.h>

/* Interpreter of C-like strings */
void CInterpreter( char *src, char *dest )
{
	char	*end;
	char	*p,*q;
	int	num;

	end = src + strlen(src) + 1;
	q = dest;
	for ( p=src; p<end; p++, q++)
	{
		switch(*p)
		{
		case '\\':
			switch(*(++p))
			{
			case '"':
				*q='"';
				break;
			case '\n':
				q--;
				break;
			case 'n':
				*q = '\n';
				break;
			case 'r':
				*q = '\r';
				break;
			case 't':
				*q = '\t';
				break;
			case 'e':
				*q = '\033';
				break;
			case 'x':
				sscanf( (char *) ++p, "%2x", &num);
				break;
			default:
				if ((*p <= '9')&&(*p >= '0'))
				{
					sscanf((char *) p, "%3o", &num);
					*q = num;
					p += 2;
				}
				else
				{
					*q = *p;
				}
				break;
			}
			break;
		default:
			*q = *p;
		}
	}
}

/* Dynamic Copy of a string */
char *StringCopy(char *src)
{
  char *result;

  if (result = AllocVec(strlen(src)+1, MEMF_PUBLIC|MEMF_CLEAR))
    strcpy(result, src);

  return(result);
}

/* Dynamic String replacement */
void ReplaceString(char **old, char *new)
{
  char *aux;

  if (aux = StringCopy(new))
    {
      if (*old) FreeVec(*old);
      *old = aux;
    }
}
