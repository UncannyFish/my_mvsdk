// Copyright (C) 1999-2000 Id Software, Inc.
//
#ifndef __UI_PUBLIC_H__
#define __UI_PUBLIC_H__

#define UI_API_VERSION	7
#define UI_API_VERSION_1_02	6

typedef struct {
	connstate_t		connState;
	int				connectPacketCount;
	int				clientNum;
	char			servername[MAX_STRING_CHARS];
	char			updateInfoString[MAX_STRING_CHARS];
	char			messageString[MAX_STRING_CHARS];
} uiClientState_t;

typedef enum {
	UI_ERROR,
	UI_PRINT,
	UI_MILLISECONDS,
	UI_CVAR_SET,
	UI_CVAR_VARIABLEVALUE,
	UI_CVAR_VARIABLESTRINGBUFFER,
	UI_CVAR_SETVALUE,
	UI_CVAR_RESET,
	UI_CVAR_CREATE,
	UI_CVAR_INFOSTRINGBUFFER,
	UI_ARGC,
	UI_ARGV,
	UI_CMD_EXECUTETEXT,
	UI_FS_FOPENFILE,
	UI_FS_READ,
	UI_FS_WRITE,
	UI_FS_FCLOSEFILE,
	UI_FS_GETFILELIST,
	UI_R_REGISTERMODEL,
	UI_R_REGISTERSKIN,
	UI_R_REGISTERSHADERNOMIP,
	UI_R_CLEARSCENE,
	UI_R_ADDREFENTITYTOSCENE,
	UI_R_ADDPOLYTOSCENE,
	UI_R_ADDLIGHTTOSCENE,
	UI_R_RENDERSCENE,
	UI_R_SETCOLOR,
	UI_R_DRAWSTRETCHPIC,
	UI_UPDATESCREEN,
	UI_CM_LERPTAG,
	UI_CM_LOADMODEL,
	UI_S_REGISTERSOUND,
	UI_S_STARTLOCALSOUND,
	UI_KEY_KEYNUMTOSTRINGBUF,
	UI_KEY_GETBINDINGBUF,
	UI_KEY_SETBINDING,
	UI_KEY_ISDOWN,
	UI_KEY_GETOVERSTRIKEMODE,
	UI_KEY_SETOVERSTRIKEMODE,
	UI_KEY_CLEARSTATES,
	UI_KEY_GETCATCHER,
	UI_KEY_SETCATCHER,
	UI_GETCLIPBOARDDATA,
	UI_GETGLCONFIG,
	UI_GETCLIENTSTATE,
	UI_GETCONFIGSTRING,
	UI_LAN_GETPINGQUEUECOUNT,
	UI_LAN_CLEARPING,
	UI_LAN_GETPING,
	UI_LAN_GETPINGINFO,
	UI_CVAR_REGISTER,
	UI_CVAR_UPDATE,
	UI_MEMORY_REMAINING,
	UI_GET_CDKEY,
	UI_SET_CDKEY,
	UI_VERIFY_CDKEY,
	UI_R_REGISTERFONT,
	UI_R_FONT_STRLENPIXELS,
	UI_R_FONT_STRLENCHARS,
	UI_R_FONT_STRHEIGHTPIXELS,
	UI_R_FONT_DRAWSTRING,
	UI_LANGUAGE_ISASIAN,
	UI_LANGUAGE_USESSPACES,
	UI_ANYLANGUAGE_READCHARFROMSTRING,
	UI_R_MODELBOUNDS,
	UI_PC_ADD_GLOBAL_DEFINE,
	UI_PC_LOAD_SOURCE,
	UI_PC_FREE_SOURCE,
	UI_PC_READ_TOKEN,
	UI_PC_SOURCE_FILE_AND_LINE,
	UI_PC_LOAD_GLOBAL_DEFINES,
	UI_PC_REMOVE_ALL_GLOBAL_DEFINES,
	UI_S_STOPBACKGROUNDTRACK,
	UI_S_STARTBACKGROUNDTRACK,
	UI_REAL_TIME,
	UI_LAN_GETSERVERCOUNT,
	UI_LAN_GETSERVERADDRESSSTRING,
	UI_LAN_GETSERVERINFO,
	UI_LAN_MARKSERVERVISIBLE,
	UI_LAN_UPDATEVISIBLEPINGS,
	UI_LAN_RESETPINGS,
	UI_LAN_LOADCACHEDSERVERS,
	UI_LAN_SAVECACHEDSERVERS,
	UI_LAN_ADDSERVER,
	UI_LAN_REMOVESERVER,
	UI_CIN_PLAYCINEMATIC,
	UI_CIN_STOPCINEMATIC,
	UI_CIN_RUNCINEMATIC,
	UI_CIN_DRAWCINEMATIC,
	UI_CIN_SETEXTENTS,
	UI_R_REMAP_SHADER,
	UI_LAN_SERVERSTATUS,
	UI_LAN_GETSERVERPING,
	UI_LAN_SERVERISVISIBLE,
	UI_LAN_COMPARESERVERS,
	UI_MEMSET = 100,
	UI_MEMCPY,
	UI_STRNCPY,
	UI_SIN,
	UI_COS,
	UI_ATAN2,
	UI_SQRT,
	UI_FLOOR,
	UI_ANGLEVECTORS,
	UI_PERPENDICULARVECTOR,
	UI_CEIL,
	UI_TESTPRINTINT,
	UI_TESTPRINTFLOAT,
	UI_ACOS,
	UI_ASIN,
	UI_MATRIXMULTIPLY,
	UI_SP_REGISTER = 200,
	UI_SP_GETSTRINGTEXTSTRING,
	UI_G2_ANGLEOVERRIDE,
	UI_R_SHADERNAMEFROMINDEX,
	UI_SP_GETNUMLANGUAGES,
	UI_SP_GETLANGUAGENAME,
	UI_G2_LISTSURFACES,
	UI_G2_LISTBONES,
	UI_G2_SETMODELS,
	UI_G2_HAVEWEGHOULMODELS,
	UI_G2_GIVEMEVECTORFROMMATRIX,
	UI_G2_GETBOLT,
	UI_G2_GETBOLT_NOREC,
	UI_G2_GETBOLT_NOREC_NOROT,
	UI_G2_INITGHOUL2MODEL,
	UI_G2_COLLISIONDETECT,
	UI_G2_COLLISIONDETECTCACHE,
	UI_G2_CLEANMODELS,
	UI_G2_PLAYANIM,
	UI_G2_GETBONEANIM,
	UI_G2_GETBONEFRAME,
	UI_G2_GETGLANAME,
	UI_G2_COPYGHOUL2INSTANCE,
	UI_G2_COPYSPECIFICGHOUL2MODEL,
	UI_G2_DUPLICATEGHOUL2INSTANCE,
	UI_G2_HASGHOUL2MODELONINDEX,
	UI_G2_REMOVEGHOUL2MODEL,
	UI_G2_ADDBOLT,
	UI_G2_SETBOLTON,
	UI_G2_SETROOTSURFACE,
	UI_G2_SETSURFACEONOFF,
	UI_G2_SETNEWORIGIN,
	UI_G2_GETTIME,
	UI_G2_SETTIME,
	UI_G2_SETRAGDOLL,
	UI_G2_ANIMATEG2MODELS,
	UI_G2_SETBONEIKSTATE,
	UI_G2_IKMOVE,
	UI_G2_GETSURFACENAME,
	UI_G2_SETSKIN,
	UI_G2_ATTACHG2MODEL,
	UI_Z_MALLOC,
	UI_Z_MEMSIZE,
	UI_Z_TAGFREE,
	UI_Z_FREE,
	UI_Z_SIZE,
	UI_Z_REALLOC
} uiImport_t;

typedef enum {
	UI_ERROR_1_02,
	UI_PRINT_1_02,
	UI_MILLISECONDS_1_02,
	UI_CVAR_SET_1_02,
	UI_CVAR_VARIABLEVALUE_1_02,
	UI_CVAR_VARIABLESTRINGBUFFER_1_02,
	UI_CVAR_SETVALUE_1_02,
	UI_CVAR_RESET_1_02,
	UI_CVAR_CREATE_1_02,
	UI_CVAR_INFOSTRINGBUFFER_1_02,
	UI_ARGC_1_02,
	UI_ARGV_1_02,
	UI_CMD_EXECUTETEXT_1_02,
	UI_FS_FOPENFILE_1_02,
	UI_FS_READ_1_02,
	UI_FS_WRITE_1_02,
	UI_FS_FCLOSEFILE_1_02,
	UI_FS_GETFILELIST_1_02,
	UI_R_REGISTERMODEL_1_02,
	UI_R_REGISTERSKIN_1_02,
	UI_R_REGISTERSHADERNOMIP_1_02,
	UI_R_CLEARSCENE_1_02,
	UI_R_ADDREFENTITYTOSCENE_1_02,
	UI_R_ADDPOLYTOSCENE_1_02,
	UI_R_ADDLIGHTTOSCENE_1_02,
	UI_R_RENDERSCENE_1_02,
	UI_R_SETCOLOR_1_02,
	UI_R_DRAWSTRETCHPIC_1_02,
	UI_UPDATESCREEN_1_02,
	UI_CM_LERPTAG_1_02,
	UI_CM_LOADMODEL_1_02,
	UI_S_REGISTERSOUND_1_02,
	UI_S_STARTLOCALSOUND_1_02,
	UI_KEY_KEYNUMTOSTRINGBUF_1_02,
	UI_KEY_GETBINDINGBUF_1_02,
	UI_KEY_SETBINDING_1_02,
	UI_KEY_ISDOWN_1_02,
	UI_KEY_GETOVERSTRIKEMODE_1_02,
	UI_KEY_SETOVERSTRIKEMODE_1_02,
	UI_KEY_CLEARSTATES_1_02,
	UI_KEY_GETCATCHER_1_02,
	UI_KEY_SETCATCHER_1_02,
	UI_GETCLIPBOARDDATA_1_02,
	UI_GETGLCONFIG_1_02,
	UI_GETCLIENTSTATE_1_02,
	UI_GETCONFIGSTRING_1_02,
	UI_LAN_GETPINGQUEUECOUNT_1_02,
	UI_LAN_CLEARPING_1_02,
	UI_LAN_GETPING_1_02,
	UI_LAN_GETPINGINFO_1_02,
	UI_CVAR_REGISTER_1_02,
	UI_CVAR_UPDATE_1_02,
	UI_MEMORY_REMAINING_1_02,
	UI_GET_CDKEY_1_02,
	UI_SET_CDKEY_1_02,
	UI_VERIFY_CDKEY_1_02,
	UI_R_REGISTERFONT_1_02,
	UI_R_FONT_STRLENPIXELS_1_02,
	UI_R_FONT_STRLENCHARS_1_02,
	UI_R_FONT_STRHEIGHTPIXELS_1_02,
	UI_R_FONT_DRAWSTRING_1_02,
	UI_ANYLANGUAGE_READCHARFROMSTRING_1_02,
	UI_R_MODELBOUNDS_1_02,
	UI_PC_ADD_GLOBAL_DEFINE_1_02,
	UI_PC_LOAD_SOURCE_1_02,
	UI_PC_FREE_SOURCE_1_02,
	UI_PC_READ_TOKEN_1_02,
	UI_PC_SOURCE_FILE_AND_LINE_1_02,
	UI_PC_LOAD_GLOBAL_DEFINES_1_02,
	UI_PC_REMOVE_ALL_GLOBAL_DEFINES_1_02,

	UI_S_STOPBACKGROUNDTRACK_1_02,
	UI_S_STARTBACKGROUNDTRACK_1_02,
	UI_REAL_TIME_1_02,
	UI_LAN_GETSERVERCOUNT_1_02,
	UI_LAN_GETSERVERADDRESSSTRING_1_02,
	UI_LAN_GETSERVERINFO_1_02,
	UI_LAN_MARKSERVERVISIBLE_1_02,
	UI_LAN_UPDATEVISIBLEPINGS_1_02,
	UI_LAN_RESETPINGS_1_02,
	UI_LAN_LOADCACHEDSERVERS_1_02,
	UI_LAN_SAVECACHEDSERVERS_1_02,
	UI_LAN_ADDSERVER_1_02,
	UI_LAN_REMOVESERVER_1_02,
	UI_CIN_PLAYCINEMATIC_1_02,
	UI_CIN_STOPCINEMATIC_1_02,
	UI_CIN_RUNCINEMATIC_1_02,
	UI_CIN_DRAWCINEMATIC_1_02,
	UI_CIN_SETEXTENTS_1_02,
	UI_R_REMAP_SHADER_1_02,
	UI_LAN_SERVERSTATUS_1_02,
	UI_LAN_GETSERVERPING_1_02,
	UI_LAN_SERVERISVISIBLE_1_02,
	UI_LAN_COMPARESERVERS_1_02,

	UI_MEMSET_1_02 = 100,
	UI_MEMCPY_1_02,
	UI_STRNCPY_1_02,
	UI_SIN_1_02,
	UI_COS_1_02,
	UI_ATAN2_1_02,
	UI_SQRT_1_02,
	UI_FLOOR_1_02,
	UI_ANGLEVECTORS_1_02,
	UI_PERPENDICULARVECTOR_1_02,
	UI_CEIL_1_02,

	UI_TESTPRINTINT_1_02,
	UI_TESTPRINTFLOAT_1_02,

	UI_ACOS_1_02,
	UI_ASIN_1_02,
	UI_MATRIXMULTIPLY_1_02,

	UI_SP_REGISTER_1_02 = 200,
	UI_SP_GETSTRINGTEXTSTRING_1_02,

/*
Ghoul2 Insert Start
*/
	UI_G2_ANGLEOVERRIDE_1_02,
/*
Ghoul2 Insert End
*/
} uiImport_1_02_t;

typedef enum {
	UIMENU_NONE,
	UIMENU_MAIN,
	UIMENU_INGAME,
	UIMENU_PLAYERCONFIG,
	UIMENU_TEAM,
	UIMENU_POSTGAME,
	UIMENU_PLAYERFORCE,
	UIMENU_MV_DOWNLOAD_POPUP,
	UIMENU_SIEGEMESSAGE,
	UIMENU_SIEGEOBJECTIVES,
	UIMENU_VOICECHAT,
	UIMENU_CLOSEALL,
	UIMENU_CLASSSEL
} uiMenuCommand_t;

#define SORT_HOST			0
#define SORT_MAP			1
#define SORT_CLIENTS		2
#define SORT_GAME			3
#define SORT_PING			4

typedef enum {
	UI_GETAPIVERSION = 0,	// system reserved

	UI_INIT,
//	void	UI_Init( void );

	UI_SHUTDOWN,
//	void	UI_Shutdown( void );

	UI_KEY_EVENT,
//	void	UI_KeyEvent( int key );

	UI_MOUSE_EVENT,
//	void	UI_MouseEvent( int dx, int dy );

	UI_REFRESH,
//	void	UI_Refresh( int time );

	UI_IS_FULLSCREEN,
//	qboolean UI_IsFullscreen( void );

	UI_SET_ACTIVE_MENU,
//	void	UI_SetActiveMenu( uiMenuCommand_t menu );

	UI_CONSOLE_COMMAND,
//	qboolean UI_ConsoleCommand( int realTime );

	UI_DRAW_CONNECT_SCREEN,
//	void	UI_DrawConnectScreen( qboolean overlay );
	UI_HASUNIQUECDKEY,
// if !overlay, the background will be drawn, otherwise it will be
// overlayed over whatever the cgame has drawn.
// a GetClientState syscall will be made to get the current strings

	UI_MENU_RESET
} uiExport_t;

#endif
