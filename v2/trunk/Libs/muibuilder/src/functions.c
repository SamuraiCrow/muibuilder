#include "builder.h"

char *Version = "$VER:muibuilder.library 1.1 (07.12.94)";

typedef struct Var
{
  char *Type;
  char *Name;
  char *Init;
  ULONG Size;
};

/* renseignements sur le fichier en memoire */
int	length;
char	*adr_declarations;

/* donnees dans le fichiers */
char *File	= NULL;
char *Catalog	= NULL;
char *GetString	= NULL;
BOOL Code,Env,Locale,Declarations, Notifications, GenerateAppli;

/* localisation dans le fichier */
char	*variables	= NULL;
char	*inits		= NULL;
char	*MUIcode	= NULL;
char	*MUIcodeBegin	= NULL;
char	*Notifycode	= NULL;
char	*NotifycodeBegin= NULL;
char	*CodeEnd   	= NULL;
char	*NotifyCodeEnd	= NULL;
struct Var *Vars        = NULL;
int	varnum		= 0;

/* droit d'utiliser les fonctions */
BOOL	rights	= FALSE;

__asm __saveds BOOL LIBMB_Open( void )
{
	BPTR	TMPfile;
	BOOL	success = TRUE;
	__aligned struct  FileInfoBlock   Info;
	char 	*adr_file;

	char    *ptrvar, *ptrinit, *InitPtr;
	ULONG   size;
	int     i;

	DOSBase     = OpenLibrary ( "dos.library",0 );
	UtilityBase = OpenLibrary ( "utility.library", 0 );

	if ((!DOSBase)||(!UtilityBase)) return(FALSE);


	Notifycode       = NULL;
	MUIcode          = NULL;
	adr_declarations = NULL;
	File		 = NULL;

	/* fichier concernant les options */
	if (TMPfile = Open( "T:MUIBuilder1.tmp", MODE_OLDFILE ))
	{
		ExamineFH( TMPfile, &Info );
		length = Info.fib_Size;
		adr_file = AllocVec(length, MEMF_PUBLIC|MEMF_CLEAR);
		if (adr_file) Read( TMPfile, adr_file, length);
		Close( TMPfile );

		/* Initialisations */
		if (adr_file)
		  {
		    File = adr_file;
		    GetString = File + strlen( File ) + 1;
		    Catalog = GetString + strlen( GetString ) + 1;
		    adr_file = Catalog + strlen( Catalog ) + 1;
		    Env 		= ( '1' == *adr_file++ );
		    Declarations 	= ( '1' == *adr_file++ );
		    Code 		= ( '1' == *adr_file++ );
		    Locale 		= ( '1' == *adr_file++ );
		    Notifications	= ( '1' == *adr_file++ );
		    GenerateAppli	= ( '1' == *adr_file++ );
		  }
		else success = FALSE;
	}	
	else	success = FALSE;

	/* fichier concernant les declarations de variables */
	if (TMPfile = Open( "T:MUIBuilder2.tmp", MODE_OLDFILE ))
        {
                ExamineFH( TMPfile, &Info );
                length 		 = Info.fib_Size;
                adr_declarations = AllocVec(length,MEMF_PUBLIC|MEMF_CLEAR);
                if (adr_declarations) Read( TMPfile, adr_declarations, length);
                Close( TMPfile );

		/* Initialisations */
		if (adr_declarations)
		  {
		    adr_file 	= adr_declarations;
		    varnum 	= atoi( adr_file );
		    variables 	= adr_file + strlen( adr_file ) + 1;
		  }
		else success = FALSE;
        }
	else	success = FALSE;

	/* fichier concernant les initialisations des variables */
	if (TMPfile = Open( "T:MUIBuilder3.tmp", MODE_OLDFILE ))
        {
                ExamineFH( TMPfile, &Info );
                length           = Info.fib_Size;
                inits = AllocVec(length,MEMF_PUBLIC|MEMF_CLEAR);
                if (inits) Read( TMPfile, inits, length);
                Close( TMPfile );
		success = (inits != NULL);
        }
	else	success = FALSE;

	if (success)
	  {
	    Vars = AllocVec(varnum*sizeof(struct Var),MEMF_PUBLIC|MEMF_CLEAR);
	    ptrvar  = variables;
	    ptrinit = inits;
	    for(i=0;i<varnum;i++)
	      {
		size = 0;
		InitPtr = ptrinit;
		switch (*ptrvar)
		  {
		  case TYPEVAR_TABSTRING:
		    do
		      {
			size++;
			ptrinit = ptrinit + strlen(ptrinit) + 1;
		      }
		    while(strlen(ptrinit)!=0);
		    ptrinit++;
		    break;
		  case TYPEVAR_STRING:
		    size = strlen(ptrinit);
		  default:
		    ptrinit = ptrinit + strlen(ptrinit) + 1;
		    break;
		  }
		Vars[i].Name = ptrvar+1;
		Vars[i].Size = size;
		Vars[i].Type = ptrvar++;
		Vars[i].Init = InitPtr;
		ptrvar = ptrvar + strlen(ptrvar) + 1;;
	      }
	  }

	/* fichier concernant le 'code MUI' */
	if (TMPfile = Open( "T:MUIBuilder4.tmp", MODE_OLDFILE ))
        {
                ExamineFH( TMPfile, &Info );
                length           = Info.fib_Size;
                MUIcode = AllocVec(length,MEMF_PUBLIC|MEMF_CLEAR);
                if (MUIcode) Read( TMPfile, MUIcode, length);           
                Close( TMPfile );
		if (MUIcode)
		  {
		    CodeEnd = (char*) MUIcode + length;
		    MUIcodeBegin = MUIcode;
		  }
		else success = FALSE;
        }
        else    success = FALSE;

	/* fichier concernant les Notifications */
	if (TMPfile = Open( "T:MUIBuilder5.tmp", MODE_OLDFILE ))
        {
                ExamineFH( TMPfile, &Info );
                length = Info.fib_Size;
                Notifycode = AllocVec(length,MEMF_PUBLIC|MEMF_CLEAR);
                if (Notifycode) Read( TMPfile, Notifycode, length);           
                Close( TMPfile );		
		if (Notifycode)
		  {
		    NotifycodeBegin = Notifycode;
		    NotifyCodeEnd = (char*) Notifycode + length;
		  }
		else
		  {
		    Notifycode = 0;
		    NotifyCodeEnd = 0;
		  }
        }

	rights = success;
	if (DOSBase) CloseLibrary( DOSBase );

	if (!success)
	  {
	    if (Notifycode) 		FreeVec(Notifycode);
	    if (MUIcode) 		FreeVec(MUIcode);
	    if (inits) 			FreeVec(inits);
	    if (adr_declarations) 	FreeVec(adr_declarations);
	    if (File) 			FreeVec(File);
	  }
	return (success);
}

__asm __saveds void LIBMB_Close( void )
{
  if (rights)
    {
      if (File)			FreeVec( File );
      if (adr_declarations )	FreeVec( adr_declarations );
      if (inits)		FreeVec( inits );
      if (MUIcodeBegin)		FreeVec( MUIcodeBegin );
      if (NotifycodeBegin)	FreeVec( NotifycodeBegin );
      if (Vars)                 FreeVec( Vars );
    }
  if (UtilityBase)	CloseLibrary( UtilityBase );
}

__asm __saveds void LIBMB_GetA( register __a1 struct TagItem *TagList )
{
	BOOL	*TagBool;
	char	**TagString;
	ULONG	*TagInt;

	if (!rights) return;


	/* On regarde si les tags sont presents */
	TagBool = (BOOL *) GetTagData(MUIB_Code, NULL, TagList );
	if (TagBool) *TagBool = Code;

	TagBool = (BOOL *) GetTagData(MUIB_Environment, NULL, TagList );
	if (TagBool) *TagBool = Env;

	TagBool = (BOOL *) GetTagData(MUIB_Locale, NULL, TagList );
	if (TagBool) *TagBool = Locale;

	TagBool = (BOOL *) GetTagData(MUIB_Declarations, NULL, TagList );
	if (TagBool) *TagBool = Declarations;

	TagBool = (BOOL *) GetTagData(MUIB_Notifications, NULL, TagList );
	if (TagBool) *TagBool = Notifications;

	TagBool = (BOOL *) GetTagData(MUIB_Application, NULL, TagList );
	if (TagBool) *TagBool = GenerateAppli;

	TagString = (char **) GetTagData(MUIB_FileName, NULL, TagList );
	if (TagString) *TagString = File;

	TagString = (char **) GetTagData(MUIB_CatalogName, NULL, TagList );
	if (TagString) *TagString = Catalog;

	TagString = (char **) GetTagData(MUIB_GetStringName, NULL, TagList );
	if (TagString) *TagString = GetString;

	TagInt = (ULONG *) GetTagData(MUIB_VarNumber, NULL, TagList );
	if (TagInt) *TagInt = varnum;

}

__asm __saveds void LIBMB_GetVarInfoA(	register __d0 ULONG  varnb,
					register __a1 struct TagItem *TagList )
{
	char    **TagString;
        ULONG   *TagInt;

	if (!rights) return;

	/* on regarde les tags demandes */
	TagInt = (ULONG *) GetTagData(MUIB_VarType, NULL, TagList );
        if (TagInt) *TagInt = (ULONG) *(Vars[varnb].Type);

	TagString = (char **) GetTagData(MUIB_VarName, NULL, TagList );
        if (TagString) *TagString = Vars[varnb].Name;

	TagInt = (ULONG *) GetTagData(MUIB_VarSize, NULL, TagList );  
        if (TagInt) *TagInt = Vars[varnb].Size;

	TagInt = (ULONG *) GetTagData(MUIB_VarInitPtr, NULL, TagList );
        if (TagInt) *TagInt = (ULONG) Vars[varnb].Init;


}

__asm __saveds void LIBMB_GetNextCode( register __a0 ULONG *type,
				       register __a1 char **code )
{
	char	*typecode;

	if (!rights) return;

	if (MUIcode>=CodeEnd)
	{
		*type = -1;
		*code = NULL;
	}
	else
	{
		typecode = MUIcode;
		MUIcode  = (char*) MUIcode + 1;
		*type = (ULONG) *typecode;
        	*code = MUIcode;
        	MUIcode = (char*) MUIcode + strlen( (char*) MUIcode ) + 1;
	}
}

__asm __saveds void LIBMB_GetNextNotify( register __a0 ULONG *type,
				         register __a1 char **code )
{
	char	*typecode;

	if (!rights) return;


	if (Notifycode>=NotifyCodeEnd)
	{
		*type = -1;
		*code = NULL;
	}
	else
	{
		typecode = Notifycode;
		Notifycode  = (char*) Notifycode + 1;
		*type = *typecode;
        	*code = Notifycode;
        	Notifycode = (char*) Notifycode + strlen( (char*) Notifycode ) + 1;
	}
}
