// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "ui_local.h"

// this file is only included when building a dll
// syscalls.asm is included instead when building a qvm

static intptr_t (QDECL *syscall)( intptr_t arg, ... ) = (intptr_t (QDECL *)( intptr_t, ...))-1;

LIBEXPORT void dllEntry( intptr_t (QDECL *syscallptr)( intptr_t arg,... ) ) {
	syscall = syscallptr;
}

int PASSFLOAT( float x ) {
	floatint_t fi;
	fi.f = x;
	return fi.i;
}

void trap_Print( const char *string ) {
	syscall( UI_PRINT, string );
}

Q_NORETURN void trap_Error( const char *string ) {
	syscall( UI_ERROR, string );
	q_unreachable();
}

int trap_Milliseconds( void ) {
	return syscall( UI_MILLISECONDS ); 
}

void trap_Cvar_Register( vmCvar_t *cvar, const char *var_name, const char *value, int flags ) {
	syscall( UI_CVAR_REGISTER, cvar, var_name, value, flags );
}

void trap_Cvar_Update( vmCvar_t *cvar ) {
	syscall( UI_CVAR_UPDATE, cvar );
}

void trap_Cvar_Set( const char *var_name, const char *value ) {
	syscall( UI_CVAR_SET, var_name, value );
}

float trap_Cvar_VariableValue( const char *var_name ) {
	floatint_t fi;
	fi.i = syscall( UI_CVAR_VARIABLEVALUE, var_name );
	return fi.f;
}

void trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize ) {
	syscall( UI_CVAR_VARIABLESTRINGBUFFER, var_name, buffer, bufsize );
}

void trap_Cvar_SetValue( const char *var_name, float value ) {
	syscall( UI_CVAR_SETVALUE, var_name, PASSFLOAT( value ) );
}

void trap_Cvar_Reset( const char *name ) {
	syscall( UI_CVAR_RESET, name ); 
}

void trap_Cvar_Create( const char *var_name, const char *var_value, int flags ) {
	syscall( UI_CVAR_CREATE, var_name, var_value, flags );
}

void trap_Cvar_InfoStringBuffer( int bit, char *buffer, int bufsize ) {
	syscall( UI_CVAR_INFOSTRINGBUFFER, bit, buffer, bufsize );
}

int trap_Argc( void ) {
	return syscall( UI_ARGC );
}

void trap_Argv( int n, char *buffer, int bufferLength ) {
	syscall( UI_ARGV, n, buffer, bufferLength );
}

void trap_Cmd_ExecuteText( cbufExec_t exec_when, const char *text ) {
	syscall( UI_CMD_EXECUTETEXT, exec_when, text );
}

int trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode ) {
	return syscall( UI_FS_FOPENFILE, qpath, f, mode );
}

void trap_FS_Read( void *buffer, int len, fileHandle_t f ) {
	syscall( UI_FS_READ, buffer, len, f );
}

void trap_FS_Write( const void *buffer, int len, fileHandle_t f ) {
	syscall( UI_FS_WRITE, buffer, len, f );
}

void trap_FS_FCloseFile( fileHandle_t f ) {
	syscall( UI_FS_FCLOSEFILE, f );
}

int trap_FS_GetFileList(  const char *path, const char *extension, char *listbuf, int bufsize ) {
	return syscall( UI_FS_GETFILELIST, path, extension, listbuf, bufsize );
}

qhandle_t trap_R_RegisterModel( const char *name ) {
	return syscall( UI_R_REGISTERMODEL, name );
}

qhandle_t trap_R_RegisterSkin( const char *name ) {
	return syscall( UI_R_REGISTERSKIN, name );
}

qhandle_t trap_R_RegisterFont( const char *fontName )
{
	return syscall( UI_R_REGISTERFONT, fontName);
}

int	trap_R_Font_StrLenPixels(const char *text, const int iFontIndex, const float scale)
{
	return syscall( UI_R_FONT_STRLENPIXELS, text, iFontIndex, PASSFLOAT(scale));
}

int trap_R_Font_StrLenChars(const char *text)
{
	return syscall( UI_R_FONT_STRLENCHARS, text);
}

int trap_R_Font_HeightPixels(const int iFontIndex, const float scale)
{
	return syscall( UI_R_FONT_STRHEIGHTPIXELS, iFontIndex, PASSFLOAT(scale));
}

void trap_R_Font_DrawString(int ox, int oy, const char *text, const float *rgba, const int setIndex, int iCharLimit, const float scale)
{
	syscall( UI_R_FONT_DRAWSTRING, ox, oy, text, rgba, setIndex, iCharLimit, PASSFLOAT(scale));
}


/* 1.04 */
qboolean trap_Language_IsAsian_1_04(void)
{
	return syscall( UI_LANGUAGE_ISASIAN );
}

qboolean trap_Language_UsesSpaces_1_04(void)
{
	return syscall( UI_LANGUAGE_USESSPACES );
}

unsigned int trap_AnyLanguage_ReadCharFromString_1_04( const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation )
{
	return syscall( UI_ANYLANGUAGE_READCHARFROMSTRING, psText, piAdvanceCount, pbIsTrailingPunctuation);
}

/* 1.02 */
unsigned int trap_AnyLanguage_ReadCharFromString_1_02( const char **ppText )
{
	return syscall( UI_ANYLANGUAGE_READCHARFROMSTRING_1_02, ppText);
}


qhandle_t trap_R_RegisterShaderNoMip( const char *name ) {
	char buf[1024];
	
	if (name[0] == '*') {
		trap_Cvar_VariableStringBuffer(name+1, buf, sizeof(buf));
		if (buf[0]) {
			return syscall( UI_R_REGISTERSHADERNOMIP, &buf );
		}
	}
	return syscall( UI_R_REGISTERSHADERNOMIP, name );
}

//added so I don't have to store a string containing the path of
//the shader icon for a class -rww
void trap_R_ShaderNameFromIndex(char *name, int index)
{
	syscall( UI_R_SHADERNAMEFROMINDEX, name, index );
}

void trap_R_ClearScene( void ) {
	syscall( UI_R_CLEARSCENE );
}

void trap_R_AddRefEntityToScene( const refEntity_t *re ) {
	syscall( UI_R_ADDREFENTITYTOSCENE, re );
}

void trap_R_AddPolyToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts ) {
	syscall( UI_R_ADDPOLYTOSCENE, hShader, numVerts, verts );
}

void trap_R_AddLightToScene( const vec3_t org, float intensity, float r, float g, float b ) {
	syscall( UI_R_ADDLIGHTTOSCENE, org, PASSFLOAT(intensity), PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b) );
}

void trap_R_RenderScene( const refdef_t *fd ) {
	syscall( UI_R_RENDERSCENE, fd );
}

void trap_R_SetColor( const float *rgba ) {
	syscall( UI_R_SETCOLOR, rgba );
}

void trap_R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader ) {
	syscall( UI_R_DRAWSTRETCHPIC, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1), PASSFLOAT(s2), PASSFLOAT(t2), hShader );
}

/* 1.04 */
void	trap_R_ModelBounds_1_04( clipHandle_t model, vec3_t mins, vec3_t maxs ) {
	syscall( UI_R_MODELBOUNDS, model, mins, maxs );
}

/* 1.02 */
void	trap_R_ModelBounds_1_02( clipHandle_t model, vec3_t mins, vec3_t maxs ) {
	syscall( UI_R_MODELBOUNDS_1_02, model, mins, maxs );
}

void trap_UpdateScreen( void ) {
	syscall( UI_UPDATESCREEN );
}

int trap_CM_LerpTag( orientation_t *tag, clipHandle_t mod, int startFrame, int endFrame, float frac, const char *tagName ) {
	return syscall( UI_CM_LERPTAG, tag, mod, startFrame, endFrame, PASSFLOAT(frac), tagName );
}

void trap_S_StartLocalSound( sfxHandle_t sfx, int channelNum ) {
	syscall( UI_S_STARTLOCALSOUND, sfx, channelNum );
}

sfxHandle_t	trap_S_RegisterSound( const char *sample ) {
	return syscall( UI_S_REGISTERSOUND, sample );
}

void trap_Key_KeynumToStringBuf_real( int keynum, char *buf, int buflen ) {
	syscall( UI_KEY_KEYNUMTOSTRINGBUF, keynum, buf, buflen );
}

void trap_Key_GetBindingBuf_real( int keynum, char *buf, int buflen ) {
	syscall( UI_KEY_GETBINDINGBUF, keynum, buf, buflen );
}

void trap_Key_SetBinding_real( int keynum, const char *binding ) {
	syscall( UI_KEY_SETBINDING, keynum, binding );
}

qboolean trap_Key_IsDown_real( int keynum ) {
	return syscall( UI_KEY_ISDOWN, keynum );
}

qboolean trap_Key_GetOverstrikeMode( void ) {
	return syscall( UI_KEY_GETOVERSTRIKEMODE );
}

void trap_Key_SetOverstrikeMode( qboolean state ) {
	syscall( UI_KEY_SETOVERSTRIKEMODE, state );
}

void trap_Key_ClearStates( void ) {
	syscall( UI_KEY_CLEARSTATES );
}

int trap_Key_GetCatcher( void ) {
	return syscall( UI_KEY_GETCATCHER );
}

void trap_Key_SetCatcher( int catcher ) {
	syscall( UI_KEY_SETCATCHER, catcher );
}

void trap_GetClipboardData( char *buf, int bufsize ) {
	syscall( UI_GETCLIPBOARDDATA, buf, bufsize );
}

void trap_GetClientState( uiClientState_t *state ) {
	syscall( UI_GETCLIENTSTATE, state );
}

void trap_GetGlconfig( vmglconfig_t *glconfig ) {
	syscall( UI_GETGLCONFIG, glconfig );
}

int trap_GetConfigString( int index, char* buff, int buffsize ) {
	return syscall( UI_GETCONFIGSTRING, index, buff, buffsize );
}


/* 1.04 */
int	trap_LAN_GetServerCount_1_04( int source ) {
	return syscall( UI_LAN_GETSERVERCOUNT, source );
}

void trap_LAN_GetServerAddressString_1_04( int source, int n, char *buf, int buflen ) {
	syscall( UI_LAN_GETSERVERADDRESSSTRING, source, n, buf, buflen );
}

void trap_LAN_GetServerInfo_1_04( int source, int n, char *buf, int buflen ) {
	syscall( UI_LAN_GETSERVERINFO, source, n, buf, buflen );
}

int trap_LAN_GetServerPing_1_04( int source, int n ) {
	return syscall( UI_LAN_GETSERVERPING, source, n );
}

int trap_LAN_ServerStatus_1_04( const char *serverAddress, char *serverStatus, int maxLen ) {
	return syscall( UI_LAN_SERVERSTATUS, serverAddress, serverStatus, maxLen );
}

void trap_LAN_SaveCachedServers_1_04() {
	syscall( UI_LAN_SAVECACHEDSERVERS );
}

void trap_LAN_LoadCachedServers_1_04() {
	syscall( UI_LAN_LOADCACHEDSERVERS );
}

void trap_LAN_ResetPings_1_04(int n) {
	syscall( UI_LAN_RESETPINGS, n );
}


/* 1.02 */
int	trap_LAN_GetServerCount_1_02( int source ) {
	return syscall( UI_LAN_GETSERVERCOUNT_1_02, source );
}

void trap_LAN_GetServerAddressString_1_02( int source, int n, char *buf, int buflen ) {
	syscall( UI_LAN_GETSERVERADDRESSSTRING_1_02, source, n, buf, buflen );
}

void trap_LAN_GetServerInfo_1_02( int source, int n, char *buf, int buflen ) {
	syscall( UI_LAN_GETSERVERINFO_1_02, source, n, buf, buflen );
}

int trap_LAN_GetServerPing_1_02( int source, int n ) {
	return syscall( UI_LAN_GETSERVERPING_1_02, source, n );
}

int trap_LAN_ServerStatus_1_02( const char *serverAddress, char *serverStatus, int maxLen ) {
	return syscall( UI_LAN_SERVERSTATUS_1_02, serverAddress, serverStatus, maxLen );
}

void trap_LAN_SaveCachedServers_1_02() {
	syscall( UI_LAN_SAVECACHEDSERVERS_1_02 );
}

void trap_LAN_LoadCachedServers_1_02() {
	syscall( UI_LAN_LOADCACHEDSERVERS_1_02 );
}

void trap_LAN_ResetPings_1_02(int n) {
	syscall( UI_LAN_RESETPINGS_1_02, n );
}


int trap_LAN_GetPingQueueCount( void ) {
	return syscall( UI_LAN_GETPINGQUEUECOUNT );
}

void trap_LAN_ClearPing( int n ) {
	syscall( UI_LAN_CLEARPING, n );
}

void trap_LAN_GetPing( int n, char *buf, int buflen, int *pingtime ) {
	syscall( UI_LAN_GETPING, n, buf, buflen, pingtime );
}

void trap_LAN_GetPingInfo( int n, char *buf, int buflen ) {
	syscall( UI_LAN_GETPINGINFO, n, buf, buflen );
}


/* 1.04 */
void trap_LAN_MarkServerVisible_1_04( int source, int n, qboolean visible ) {
	syscall( UI_LAN_MARKSERVERVISIBLE, source, n, visible );
}

int trap_LAN_ServerIsVisible_1_04( int source, int n) {
	return syscall( UI_LAN_SERVERISVISIBLE, source, n );
}

qboolean trap_LAN_UpdateVisiblePings_1_04( int source ) {
	return syscall( UI_LAN_UPDATEVISIBLEPINGS, source );
}

int trap_LAN_AddServer_1_04(int source, const char *name, const char *addr) {
	return syscall( UI_LAN_ADDSERVER, source, name, addr );
}

void trap_LAN_RemoveServer_1_04(int source, const char *addr) {
	syscall( UI_LAN_REMOVESERVER, source, addr );
}

int trap_LAN_CompareServers_1_04( int source, int sortKey, int sortDir, int s1, int s2 ) {
	return syscall( UI_LAN_COMPARESERVERS, source, sortKey, sortDir, s1, s2 );
}

/* 1.02 */
void trap_LAN_MarkServerVisible_1_02( int source, int n, qboolean visible ) {
	syscall( UI_LAN_MARKSERVERVISIBLE_1_02, source, n, visible );
}

int trap_LAN_ServerIsVisible_1_02( int source, int n) {
	return syscall( UI_LAN_SERVERISVISIBLE_1_02, source, n );
}

qboolean trap_LAN_UpdateVisiblePings_1_02( int source ) {
	return syscall( UI_LAN_UPDATEVISIBLEPINGS_1_02, source );
}

int trap_LAN_AddServer_1_02(int source, const char *name, const char *addr) {
	return syscall( UI_LAN_ADDSERVER_1_02, source, name, addr );
}

void trap_LAN_RemoveServer_1_02(int source, const char *addr) {
	syscall( UI_LAN_REMOVESERVER_1_02, source, addr );
}

int trap_LAN_CompareServers_1_02( int source, int sortKey, int sortDir, int s1, int s2 ) {
	return syscall( UI_LAN_COMPARESERVERS_1_02, source, sortKey, sortDir, s1, s2 );
}


int trap_MemoryRemaining( void ) {
	return syscall( UI_MEMORY_REMAINING );
}

#ifdef USE_CD_KEY

void trap_GetCDKey( char *buf, int buflen ) {
	syscall( UI_GET_CDKEY, buf, buflen );
}

void trap_SetCDKey( char *buf ) {
	syscall( UI_SET_CDKEY, buf );
}

qboolean trap_VerifyCDKey( const char *key, const char *chksum) {
	return syscall( UI_VERIFY_CDKEY, key, chksum);
}

#endif // USE_CD_KEY


/* 1.04 */
int trap_PC_AddGlobalDefine_1_04( char *define ) {
	return syscall( UI_PC_ADD_GLOBAL_DEFINE, define );
}

int trap_PC_LoadSource_1_04( const char *filename ) {
	return syscall( UI_PC_LOAD_SOURCE, filename );
}

int trap_PC_FreeSource_1_04( int handle ) {
	return syscall( UI_PC_FREE_SOURCE, handle );
}

int trap_PC_ReadToken_1_04( int handle, pc_token_t *pc_token ) {
	return syscall( UI_PC_READ_TOKEN, handle, pc_token );
}

int trap_PC_SourceFileAndLine_1_04( int handle, char *filename, int *line ) {
	return syscall( UI_PC_SOURCE_FILE_AND_LINE, handle, filename, line );
}

int trap_PC_LoadGlobalDefines_1_04 ( const char* filename )
{
	return syscall ( UI_PC_LOAD_GLOBAL_DEFINES, filename );
}

void trap_PC_RemoveAllGlobalDefines_1_04 ( void )
{
	syscall ( UI_PC_REMOVE_ALL_GLOBAL_DEFINES );
}

void trap_S_StopBackgroundTrack_1_04( void ) {
	syscall( UI_S_STOPBACKGROUNDTRACK );
}

void trap_S_StartBackgroundTrack_1_04( const char *intro, const char *loop, qboolean bReturnWithoutStarting) {
	syscall( UI_S_STARTBACKGROUNDTRACK, intro, loop, bReturnWithoutStarting );
}

int trap_RealTime_1_04(qtime_t *qtime) {
	return syscall( UI_REAL_TIME, qtime );
}

// this returns a handle.  arg0 is the name in the format "idlogo.roq", set arg1 to NULL, alteredstates to qfalse (do not alter gamestate)
int trap_CIN_PlayCinematic_1_04( const char *arg0, int xpos, int ypos, int width, int height, int bits) {
  return syscall(UI_CIN_PLAYCINEMATIC, arg0, xpos, ypos, width, height, bits);
}
 
// stops playing the cinematic and ends it.  should always return FMV_EOF
// cinematics must be stopped in reverse order of when they are started
e_status trap_CIN_StopCinematic_1_04(int handle) {
  return syscall(UI_CIN_STOPCINEMATIC, handle);
}


// will run a frame of the cinematic but will not draw it.  Will return FMV_EOF if the end of the cinematic has been reached.
e_status trap_CIN_RunCinematic_1_04 (int handle) {
  return syscall(UI_CIN_RUNCINEMATIC, handle);
}

// draws the current frame
void trap_CIN_DrawCinematic_1_04 (int handle) {
  syscall(UI_CIN_DRAWCINEMATIC, handle);
}

// allows you to resize the animation dynamically
void trap_CIN_SetExtents_1_04 (int handle, int x, int y, int w, int h) {
  syscall(UI_CIN_SETEXTENTS, handle, x, y, w, h);
}

void	trap_R_RemapShader_1_04( const char *oldShader, const char *newShader, const char *timeOffset ) {
	syscall( UI_R_REMAP_SHADER, oldShader, newShader, timeOffset );
}

/* 1.02 */
int trap_PC_AddGlobalDefine_1_02( char *define ) {
	return syscall( UI_PC_ADD_GLOBAL_DEFINE_1_02, define );
}

int trap_PC_LoadSource_1_02( const char *filename ) {
	return syscall( UI_PC_LOAD_SOURCE_1_02, filename );
}

int trap_PC_FreeSource_1_02( int handle ) {
	return syscall( UI_PC_FREE_SOURCE_1_02, handle );
}

int trap_PC_ReadToken_1_02( int handle, pc_token_t *pc_token ) {
	return syscall( UI_PC_READ_TOKEN_1_02, handle, pc_token );
}

int trap_PC_SourceFileAndLine_1_02( int handle, char *filename, int *line ) {
	return syscall( UI_PC_SOURCE_FILE_AND_LINE_1_02, handle, filename, line );
}

int trap_PC_LoadGlobalDefines_1_02 ( const char* filename )
{
	return syscall ( UI_PC_LOAD_GLOBAL_DEFINES_1_02, filename );
}

void trap_PC_RemoveAllGlobalDefines_1_02 ( void )
{
	syscall ( UI_PC_REMOVE_ALL_GLOBAL_DEFINES_1_02 );
}

void trap_S_StopBackgroundTrack_1_02( void ) {
	syscall( UI_S_STOPBACKGROUNDTRACK_1_02 );
}

void trap_S_StartBackgroundTrack_1_02( const char *intro, const char *loop, qboolean bReturnWithoutStarting) {
	syscall( UI_S_STARTBACKGROUNDTRACK_1_02, intro, loop, bReturnWithoutStarting );
}

int trap_RealTime_1_02(qtime_t *qtime) {
	return syscall( UI_REAL_TIME_1_02, qtime );
}

// this returns a handle.  arg0 is the name in the format "idlogo.roq", set arg1 to NULL, alteredstates to qfalse (do not alter gamestate)
int trap_CIN_PlayCinematic_1_02( const char *arg0, int xpos, int ypos, int width, int height, int bits) {
  return syscall(UI_CIN_PLAYCINEMATIC_1_02, arg0, xpos, ypos, width, height, bits);
}
 
// stops playing the cinematic and ends it.  should always return FMV_EOF
// cinematics must be stopped in reverse order of when they are started
e_status trap_CIN_StopCinematic_1_02(int handle) {
  return syscall(UI_CIN_STOPCINEMATIC_1_02, handle);
}

// will run a frame of the cinematic but will not draw it.  Will return FMV_EOF if the end of the cinematic has been reached.
e_status trap_CIN_RunCinematic_1_02 (int handle) {
  return syscall(UI_CIN_RUNCINEMATIC_1_02, handle);
}

// draws the current frame
void trap_CIN_DrawCinematic_1_02 (int handle) {
  syscall(UI_CIN_DRAWCINEMATIC_1_02, handle);
}

// allows you to resize the animation dynamically
void trap_CIN_SetExtents_1_02 (int handle, int x, int y, int w, int h) {
  syscall(UI_CIN_SETEXTENTS_1_02, handle, x, y, w, h);
}

void	trap_R_RemapShader_1_02( const char *oldShader, const char *newShader, const char *timeOffset ) {
	syscall( UI_R_REMAP_SHADER, oldShader, newShader, timeOffset );
}

qboolean trap_SP_Register(char *file ) 
{
	return syscall( UI_SP_REGISTER,file );
}

int trap_SP_GetNumLanguages( void )
{
	return syscall( UI_SP_GETNUMLANGUAGES );
}

void trap_GetLanguageName( const int languageIndex, char *buffer )
{
	syscall( UI_SP_GETLANGUAGENAME, languageIndex, buffer);
}

int trap_SP_GetStringTextString(const char *text, char *buffer, int bufferLength)
{
	return syscall( UI_SP_GETSTRINGTEXTSTRING, text, buffer, bufferLength );
}
/*
Ghoul2 Insert Start
*/
void trap_G2_ListModelSurfaces(void *ghlInfo)
{
	syscall( UI_G2_LISTSURFACES, ghlInfo);
}

void trap_G2_ListModelBones(void *ghlInfo, int frame)
{
	syscall( UI_G2_LISTBONES, ghlInfo, frame);
}

void trap_G2_SetGhoul2ModelIndexes(void *ghoul2, qhandle_t *modelList, qhandle_t *skinList)
{
	syscall( UI_G2_SETMODELS, ghoul2, modelList, skinList);
}

qboolean trap_G2_HaveWeGhoul2Models(void *ghoul2)
{
	return (qboolean)(syscall(UI_G2_HAVEWEGHOULMODELS, ghoul2));
}

void trap_G2API_GiveMeVectorFromMatrix(mdxaBone_t *boltMatrix, int flags, vec3_t vec)
{
	syscall(UI_G2_GIVEMEVECTORFROMMATRIX, boltMatrix, flags, vec);
}

qboolean trap_G2API_GetBoltMatrix(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix,
								const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale)
{
	return (qboolean)(syscall(UI_G2_GETBOLT, ghoul2, modelIndex, boltIndex, matrix, angles, position, frameNum, modelList, scale));
}

qboolean trap_G2API_GetBoltMatrix_NoReconstruct(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix,
								const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale)
{ //Same as above but force it to not reconstruct the skeleton before getting the bolt position
	return (qboolean)(syscall(UI_G2_GETBOLT_NOREC, ghoul2, modelIndex, boltIndex, matrix, angles, position, frameNum, modelList, scale));
}

qboolean trap_G2API_GetBoltMatrix_NoRecNoRot(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix,
								const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale)
{ //Same as above but force it to not reconstruct the skeleton before getting the bolt position
	return (qboolean)(syscall(UI_G2_GETBOLT_NOREC_NOROT, ghoul2, modelIndex, boltIndex, matrix, angles, position, frameNum, modelList, scale));
}

int trap_G2API_InitGhoul2Model(void **ghoul2Ptr, const char *fileName, int modelIndex, qhandle_t customSkin,
						  qhandle_t customShader, int modelFlags, int lodBias)
{
	return syscall(UI_G2_INITGHOUL2MODEL, ghoul2Ptr, fileName, modelIndex, customSkin, customShader, modelFlags, lodBias);
}

qboolean trap_G2API_SetSkin(void *ghoul2, int modelIndex, qhandle_t customSkin, qhandle_t renderSkin)
{
	return syscall(UI_G2_SETSKIN, ghoul2, modelIndex, customSkin, renderSkin);
}

void trap_G2API_CollisionDetect ( 
	CollisionRecord_t *collRecMap, 
	void* ghoul2, 
	const vec3_t angles, 
	const vec3_t position,
	int frameNumber, 
	int entNum, 
	const vec3_t rayStart, 
	const vec3_t rayEnd, 
	const vec3_t scale, 
	int traceFlags, 
	int useLod,
	float fRadius
	)
{
	syscall ( UI_G2_COLLISIONDETECT, collRecMap, ghoul2, angles, position, frameNumber, entNum, rayStart, rayEnd, scale, traceFlags, useLod, PASSFLOAT(fRadius) );
}

void trap_G2API_CollisionDetectCache ( 
	CollisionRecord_t *collRecMap, 
	void* ghoul2, 
	const vec3_t angles, 
	const vec3_t position,
	int frameNumber, 
	int entNum, 
	const vec3_t rayStart, 
	const vec3_t rayEnd, 
	const vec3_t scale, 
	int traceFlags, 
	int useLod,
	float fRadius
	)
{
	syscall ( UI_G2_COLLISIONDETECTCACHE, collRecMap, ghoul2, angles, position, frameNumber, entNum, rayStart, rayEnd, scale, traceFlags, useLod, PASSFLOAT(fRadius) );
}

void trap_G2API_CleanGhoul2Models(void **ghoul2Ptr)
{
	syscall(UI_G2_CLEANMODELS, ghoul2Ptr);
}

qboolean trap_G2API_SetBoneAngles(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags,
								const int up, const int right, const int forward, qhandle_t *modelList,
								int blendTime , int currentTime )
{
	return (syscall(UI_G2_ANGLEOVERRIDE, ghoul2, modelIndex, boneName, angles, flags, up, right, forward, modelList, blendTime, currentTime));
}

qboolean trap_G2API_SetBoneAnim(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame,
							  const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime )
{
	return syscall(UI_G2_PLAYANIM, ghoul2, modelIndex, boneName, startFrame, endFrame, flags, PASSFLOAT(animSpeed), currentTime, PASSFLOAT(setFrame), blendTime);
}

qboolean trap_G2API_GetBoneAnim(void *ghoul2, const char *boneName, const int currentTime, float *currentFrame,
						   int *startFrame, int *endFrame, int *flags, float *animSpeed, int *modelList, const int modelIndex)
{
	return syscall(UI_G2_GETBONEANIM, ghoul2, boneName, currentTime, currentFrame, startFrame, endFrame, flags, animSpeed, modelList, modelIndex);
}

qboolean trap_G2API_GetBoneFrame(void *ghoul2, const char *boneName, const int currentTime, float *currentFrame, int *modelList, const int modelIndex)
{
	return syscall(UI_G2_GETBONEFRAME, ghoul2, boneName, currentTime, currentFrame, modelList, modelIndex);
}

void trap_G2API_GetGLAName(void *ghoul2, int modelIndex, char *fillBuf)
{
	syscall(UI_G2_GETGLANAME, ghoul2, modelIndex, fillBuf);
}

int trap_G2API_CopyGhoul2Instance(void *g2From, void *g2To, int modelIndex)
{
	return syscall(UI_G2_COPYGHOUL2INSTANCE, g2From, g2To, modelIndex);
}

void trap_G2API_CopySpecificGhoul2Model(void *g2From, int modelFrom, void *g2To, int modelTo)
{
	syscall(UI_G2_COPYSPECIFICGHOUL2MODEL, g2From, modelFrom, g2To, modelTo);
}

void trap_G2API_DuplicateGhoul2Instance(void *g2From, void **g2To)
{
	syscall(UI_G2_DUPLICATEGHOUL2INSTANCE, g2From, g2To);
}

qboolean trap_G2API_HasGhoul2ModelOnIndex(void *ghlInfo, int modelIndex)
{
	return syscall(UI_G2_HASGHOUL2MODELONINDEX, ghlInfo, modelIndex);
}

qboolean trap_G2API_RemoveGhoul2Model(void *ghlInfo, int modelIndex)
{
	return syscall(UI_G2_REMOVEGHOUL2MODEL, ghlInfo, modelIndex);
}

int	trap_G2API_AddBolt(void *ghoul2, int modelIndex, const char *boneName)
{
	return syscall(UI_G2_ADDBOLT, ghoul2, modelIndex, boneName);
}

void trap_G2API_SetBoltInfo(void *ghoul2, int modelIndex, int boltInfo)
{
	syscall(UI_G2_SETBOLTON, ghoul2, modelIndex, boltInfo);
}

qboolean trap_G2API_SetRootSurface(void *ghoul2, const int modelIndex, const char *surfaceName)
{
	return syscall(UI_G2_SETROOTSURFACE, ghoul2, modelIndex, surfaceName);
}

qboolean trap_G2API_SetSurfaceOnOff(void *ghoul2, const char *surfaceName, const int flags)
{
	return syscall(UI_G2_SETSURFACEONOFF, ghoul2, surfaceName, flags);
}

qboolean trap_G2API_SetNewOrigin(void *ghoul2, const int boltIndex)
{
	return syscall(UI_G2_SETNEWORIGIN, ghoul2, boltIndex);
}

int trap_G2API_GetTime(void)
{
	return syscall(UI_G2_GETTIME);
}

void trap_G2API_SetTime(int time, int clock)
{
	syscall(UI_G2_SETTIME, time, clock);
}

//rww - RAGDOLL_BEGIN
void trap_G2API_SetRagDoll(void *ghoul2, sharedRagDollParams_t *params)
{
	syscall(UI_G2_SETRAGDOLL, ghoul2, params);
}

void trap_G2API_AnimateG2Models(void *ghoul2, int time, sharedRagDollUpdateParams_t *params)
{
	syscall(UI_G2_ANIMATEG2MODELS, ghoul2, time, params);
}
//rww - RAGDOLL_END

qboolean trap_G2API_SetBoneIKState(void *ghoul2, int time, const char *boneName, int ikState, sharedSetBoneIKStateParams_t *params)
{
	return syscall(UI_G2_SETBONEIKSTATE, ghoul2, time, boneName, ikState, params);
}

qboolean trap_G2API_IKMove(void *ghoul2, int time, sharedIKMoveParams_t *params)
{
	return syscall(UI_G2_IKMOVE, ghoul2, time, params);
}

void trap_G2API_GetSurfaceName(void *ghoul2, int surfNumber, int modelIndex, char *fillBuf)
{
	syscall(UI_G2_GETSURFACENAME, ghoul2, surfNumber, modelIndex, fillBuf);
}

qboolean trap_G2API_AttachG2Model(void *ghoul2From, int modelIndexFrom, void *ghoul2To, int toBoltIndex, int toModel)
{
	return syscall(UI_G2_ATTACHG2MODEL, ghoul2From, modelIndexFrom, ghoul2To, toBoltIndex, toModel);
}
/*
Ghoul2 Insert End
*/


void *trap_Z_Malloc(int iSize, memtag_t eTag, qboolean bZeroit)
{
	return (void *) syscall(UI_Z_MALLOC, iSize, eTag, bZeroit);
}

int trap_Z_MemSize(memtag_t eTag)
{
	return syscall(UI_Z_MEMSIZE, eTag);
}

void trap_Z_TagFree(memtag_t eTag)
{
	syscall(UI_Z_TAGFREE, eTag);
}

void trap_Z_Free(void *ptr)
{
	syscall(UI_Z_FREE, ptr);
}

int trap_Z_Size(void *pvAddress)
{
	return syscall(UI_Z_SIZE, pvAddress);
}

void *trap_Z_Realloc(void *pvAddress, int iNewSize, qboolean bZeroit)
{
	return (void *) syscall(UI_Z_REALLOC, pvAddress, iNewSize, bZeroit);
}

int trap_FS_CreateFileList(const char *path, const char *extension)
{
	return syscall(UI_FS_CREATE_FILE_LIST, path, extension);
}

void trap_FS_CloseFileList(void)
{
	syscall(UI_FS_CLOSE_FILE_LIST);
}

void trap_FS_GetNextFile(char *path, int count)
{
	syscall(UI_FS_GET_NEXT_FILE, path, count);
}

/* JK2MV Syscalls */
qboolean trap_MVAPI_ControlFixes( int fixes )
{
	return syscall( MVAPI_CONTROL_FIXES, fixes );
}

int trap_MVAPI_GetVersion( void )
{
	return syscall( MVAPI_GET_VERSION );
}

int trap_FS_FLock( fileHandle_t h, flockCmd_t cmd, qboolean nb )
{
	return syscall( MVAPI_FS_FLOCK, h, cmd, nb );
}

void trap_MVAPI_SetVersion( mvversion_t version )
{
	syscall( MVAPI_SET_VERSION, version );
}

void trap_R_AddRefEntityToScene2( const refEntity_t *re )
{
	syscall( UI_MVAPI_R_ADDREFENTITYTOSCENE2, re );
}

void trap_MVAPI_SetVirtualScreen( float w, float h )
{
	syscall( UI_MVAPI_SETVIRTUALSCREEN, PASSFLOAT(w), PASSFLOAT(h) );
}

void trap_CL_ContinueCurrentDownload(dldecision_t decision) {
	syscall(UI_MVAPI_CONTINUE_DOWNLOAD, decision);
}

int trap_FS_GetDLList(dlfile_t *files, int maxfiles) {
	return syscall(UI_MVAPI_GETDLLIST, files, maxfiles);
}

qboolean trap_FS_RMDLPrefix(const char *qpath) {
	return syscall(UI_MVAPI_RMDLPREFIX, qpath);
}

qboolean trap_UI_DeleteDLFile(const dlfile_t *file) {
	return syscall(UI_MVAPI_DELDLFILE, file);
}

void trap_MVAPI_Print( int flags, const char *string )
{
	syscall( MVAPI_PRINT, string );
}
