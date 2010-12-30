/* Functions available for file Save action */

extern int  MB_OpenSaveFile(char *);
extern void MB_CloseSaveFile(void);
extern void MB_BeginSaveObject(char *, char *);
extern void MB_EndSaveObject(void);
extern void MB_SaveAttribute(char *, ULONG, ULONG);
