#include "builder2.h"

BOOL Requester( char * title, char * dir, char * file )
{
	struct	FileRequester *req;
	struct	Window*	win = NULL;
	BOOL	result = FALSE;

	if (req=MUI_AllocAslRequestTags(ASL_FileRequest,TAG_DONE))
        {
		get( WI_current, MUIA_Window_Window, &win );
                if (MUI_AslRequestTags(req,
                        ASLFO_PrivateIDCMP   ,TRUE,
                        ASLFO_TitleText      ,title,
                        ASLFR_InitialFile    , file,
                        ASLFR_InitialDrawer  , dir,
			ASLFO_Window	     , win,
			ASLFR_RejectIcons    , TRUE,
                        TAG_DONE))
                {
                	strncpy( dir , req->fr_Drawer, 255);
                	strncpy( file, req->fr_File  , 255);
			result = TRUE;
		}
	}
        MUI_FreeAslRequest(req);
	return (result);
}

BOOL SaveRequester( char * title, char * dir, char * file )
{
	struct	FileRequester *req;
	struct	Window*	win = NULL;
	BOOL	result = FALSE;

	if (req=MUI_AllocAslRequestTags(ASL_FileRequest,TAG_DONE))
        {
		get( WI_current, MUIA_Window_Window, &win );
                if (MUI_AslRequestTags(req,
                        ASLFO_PrivateIDCMP   ,TRUE,
                        ASLFO_TitleText      ,title,
                        ASLFR_InitialFile    , file,
                        ASLFR_InitialDrawer  , dir,
			ASLFO_Window	     , win,
			ASLFR_RejectIcons    , TRUE,
			ASLFR_DoSaveMode     , TRUE,
                        TAG_DONE))
                {
                	strncpy( dir , req->fr_Drawer, 255);
                	strncpy( file, req->fr_File  , 255);
			result = TRUE;
		}
	}
        MUI_FreeAslRequest(req);
	return (result);
}
