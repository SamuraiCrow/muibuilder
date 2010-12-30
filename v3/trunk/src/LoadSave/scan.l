%{

#include <fcntl.h>
#include <stdio.h>

enum
{
  WAIT_DECLARATION,
  WAIT_TYPE,
  WAIT_OBJECT_NAME,
  WAIT_OPENED_BRACE,
  WAIT_NEXT_ITEM,
  WAIT_EQUAL,
  WAIT_VALUE,
  WAIT_END_LINE,
  STATE_ERROR
};

enum
{
  OBJECT,
  RESSOURCE
};

struct ObjectDeclaration
{
  char type[30];
  char label[80];
  Object *obj;
};

int num_lines = 1;
int state = STATE_ERROR;
int objecttype = -1;
struct ObjectDeclaration current;

void ErrorMessage(char *message)
  {
    printf("Error Line:%d: %s\n", num_lines, message);
  }

%}

ID  [^ \t\n;="]
ID2 [^"]

%%
"MUIBUILDER_FILE" {
  state = WAIT_DECLARATION;
}

"Object" {
  if (state != WAIT_DECLARATION)
    {
      ErrorMessage("Object declaration unexpected");
      return(state = STATE_ERROR);
    }
  objecttype  = OBJECT;
  state = WAIT_TYPE;
}

"Ressource" {
  if (state != WAIT_DECLARATION)
  {
    ErrorMessage("Ressource declaration unexpected");
    return(state = STATE_ERROR);
  }
  objecttype = RESSOURCE;
  state = WAIT_TYPE;
}

"=" {
  if (state != WAIT_EQUAL)
    {
      ErrorMessage("Equal sign misplaced");
      return(state = STATE_ERROR);
    }
  state = WAIT_VALUE;
}

"{" {
  if (state != WAIT_OPENED_BRACE)
    {
      ErrorMessage("Opening brace misplaced");
      return(state = STATE_ERROR);
    }
  current->obj = NewObject(NULL, current->type,
                           MBA_Object_Label, current->label,
                           TAG_DONE);
  state = WAIT_NEXT_ITEM;
}

"}" {
  if (state != WAIT_NEXT_ITEM)
    {
      ErrorMessage("Closing brace misplaced");
      return(state = STATE_ERROR);
    }
  state = WAIT_DECLARATION;
}

";" {
  if (state != WAIT_END_LINE)
    {
    ErrorMessage("End of attribute definition error");
    return(state = STATE_ERROR);
    }
  state = WAIT_NEXT_ITEM;
}

{ID}+ {
  switch(state)
    {
    case WAIT_TYPE:
      strncpy(current->type, yytext, 30)
      state = WAIT_OBJECT_NAME;
      break;
    case WAIT_OBJECT_NAME:
      strncpy(current->label, yytext, 80);
      state = WAIT_OPENED_BRACE;
      break;
    case WAIT_NEXT_ITEM:
      printf("Attribut = %s\n", yytext);
      state = WAIT_EQUAL;
      break;
    case WAIT_VALUE:
      printf("Value = %s\n", yytext);
      state = WAIT_END_LINE;
      break;
    case WAIT_END_LINE:
      ErrorMessage("Equal sign expected");
      break;
    default:
      ErrorMessage("Syntax Error");
      return(state = STATE_ERROR);
      break;
  }
}

\"{ID2}*\" {
  if (state != WAIT_VALUE)
    {
      ErrorMessage("String must be used as attribute value");
      return(state = STATE_ERROR);
    }
  printf("Value = %s\n", yytext);
  state = WAIT_END_LINE;
}

"/*".*"*/"

[ \t]

\n              num_lines++;

%%
