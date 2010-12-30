#include "builder2.h"
#include <exec/memory.h>

int	nb_modules = 0;
extern	char * PopModules[50];

BOOL RecordFileInfoBlock(struct FileInfoBlock *fib)
{
	char *aux;

	if (fib->fib_FileName == strstr( fib->fib_FileName, "GenCode" ) )
	{
		aux = AllocVec(strlen(fib->fib_FileName)+1, MEMF_PUBLIC|MEMF_CLEAR);
		if (aux)
		{
			strcpy(aux, &((char*)fib->fib_FileName)[7]);
			PopModules[nb_modules++] = aux;
			PopModules[nb_modules] = NULL;
		}
	}
	return(TRUE);
}


static void ScanDir(BPTR DirLock)
{
	struct FileInfoBlock *fib;
	BOOL Ok = TRUE;
	BPTR OldDir;
	char Name[32], errbuf[80];
	extern void ErrorMessageEasy(char *);

	if (!(fib = AllocMem(sizeof(struct FileInfoBlock), MEMF_PUBLIC|MEMF_CLEAR)))
		return;
	if (Examine(DirLock, fib)) {
		strcpy(Name, fib->fib_FileName);
		OldDir = CurrentDir(DirLock);
		if (fib->fib_DirEntryType > 0) {
			while (ExNext(DirLock, fib) && (Ok = RecordFileInfoBlock(fib)));
			if (Ok && IoErr() != ERROR_NO_MORE_ENTRIES) {
				Fault(IoErr(), NULL, errbuf, 79);
				ErrorMessageEasy(GetMUIBuilderString(MSG_ErrorReading));
			}
		}
		CurrentDir(OldDir);
	}
	FreeMem(fib, sizeof(struct FileInfoBlock));
}


void ScanAvailableModules(void)
{
	BPTR DirLock;

	if (DirLock = Lock("PROGDIR:Modules", ACCESS_READ)) {
		ScanDir(DirLock);
		UnLock(DirLock);
	}
}

