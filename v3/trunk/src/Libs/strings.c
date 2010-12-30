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
