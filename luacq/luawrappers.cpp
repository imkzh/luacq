#include "stdafx.h"

#ifdef __cplusplus
	#include "lua_src\lua.hpp"
#else
	#include "lua_src\lua.h"
	# include "lua_src\lualib.h"
	# include "lua_src\lauxlib.h"
#endif

#include "luawrappers.h"
#include "cqp.h"
 
extern int ac;
char InterfaceName[50] = {-1};
unsigned char IsInterfaceNameSet = 0;

int file_exists(char *filename)
{
	struct stat   buffer;
	return (stat(filename, &buffer) == 0);
}
 
void lua_throw(lua_State * state) {
	const char* message = lua_tostring(state, -1);
	puts(message);
	lua_pop(state, 1);
}

lua_State * lua_doInit(){
	lua_State *state = luaL_newstate();
	luaL_openlibs(state);
	luai_loadWrappers(state);
	char path[512];
	 
	strcpy_s(path, 512, _pgmptr);
	strcat_s(path, 512, "\\init.lua");
	if (file_exists(path)) {
		int result = luaL_loadfile(state, path);
		if (result != LUA_OK) {
			lua_throw(state);
			CQ_setFatal(ac, "Lua Engine failed to load initial script(.\\init.lua), please consider reinstall it.");
		}
		result = lua_pcall(state, 0, LUA_MULTRET, 0);
		if (result != LUA_OK) {
			lua_throw(state);
			CQ_setFatal(ac, "Lua Engine failed to Execute initial script (.\\init.lua) since it may corrupted, please consider reinstall it.");
			return 0;
		}
	}else{
		CQ_setFatal(ac, "Lua Engine has been disabled since it finds no initial script.");
	}


}

void luai_loadWrappers(lua_State * state) {

	//Program Interface.
	lua_register(state, "CQLuaI_SetInterfaceName", CQLuaI_SetInterfaceName);

	//API Wrappers
	lua_register(state, "CQLuaI_sendPrivateMsg", luai_sendPrivateMsg);
	lua_register(state, "CQLuaI_sendGroupMsg", luai_sendGroupMsg);
	lua_register(state, "CQLuaI_sendDiscussMsg", luai_sendDiscussMsg);
	lua_register(state, "CQLuaI_sendLike", luai_sendLike);
	lua_register(state, "CQLuaI_setGroupKick", luai_setGroupKick);
	lua_register(state, "CQLuaI_setGroupBan", luai_setGroupBan);
	lua_register(state, "CQLuaI_setGroupAdmin", luai_setGroupAdmin);
	lua_register(state, "CQLuaI_setGroupWholeBan", luai_setGroupWholeBan);
	lua_register(state, "CQLuaI_setGroupAnonymousBan", luai_setGroupAnonymousBan);
	lua_register(state, "CQLuaI_setGroupAnonymous", luai_setGroupAnonymous);
	lua_register(state, "CQLuaI_setGroupCard", luai_setGroupCard);
	lua_register(state, "CQLuaI_setGroupLeave", luai_setGroupLeave);
	lua_register(state, "CQLuaI_setGroupSpecialTitle", luai_setGroupSpecialTitle);
	lua_register(state, "CQLuaI_setDiscussLeave", luai_setDiscussLeave);
	lua_register(state, "CQLuaI_setFriendAddRequest", luai_setFriendAddRequest);
	lua_register(state, "CQLuaI_setGroupAddRequestV2", luai_setGroupAddRequestV2);
	lua_register(state, "CQLuaI_getGroupMemberInfoV2", luai_getGroupMemberInfoV2);
	lua_register(state, "CQLuaI_getStrangerInfo", luai_getStrangerInfo);
	lua_register(state, "CQLuaI_addLog", luai_addLog);
	lua_register(state, "CQLuaI_getCookies", luai_getCookies);
	lua_register(state, "CQLuaI_getCsrfToken", luai_getCsrfToken);
	lua_register(state, "CQLuaI_getLoginQQ", luai_getLoginQQ);
	lua_register(state, "CQLuaI_getLoginNick", luai_getLoginNick);
	lua_register(state, "CQLuaI_getAppDirectory", luai_getAppDirectory);
	lua_register(state, "CQLuaI_setFatal", luai_setFatal);

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_sendPrivateMsg*/
int CQLuaI_SetInterfaceName(lua_State * state) {

	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args > 0) {
		char * iName =(char *)lua_tostring(state, 0);
		strcpy_s(InterfaceName, 50, iName);
		IsInterfaceNameSet = 1;
	}

	// Push the return value on top of the stack. NOTE: We haven't popped the
	// input arguments to our function. To be honest, I haven't checked if we
	// must, but at least in stack machines like the JVM, the stack will be
	// cleaned between each function call.
	//lua_pushnumber(state, 123);
	// Let Lua know how many return values we've passed
	return 0;
}


#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_sendPrivateMsg*/
int luai_sendPrivateMsg(lua_State * state) {
	/*int32_t AuthCode, int64_t QQID, const char *msg*/

	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);

	for (int n = 1; n <= args; ++n) {
		printf("[CPP]  argument %d: '%s'\n", n, lua_tostring(state, n));
	}

	// Push the return value on top of the stack. NOTE: We haven't popped the
	// input arguments to our function. To be honest, I haven't checked if we
	// must, but at least in stack machines like the JVM, the stack will be
	// cleaned between each function call.
	lua_pushnumber(state, 123);
	// Let Lua know how many return values we've passed
	return 1;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_sendGroupMsg*/
int luai_sendGroupMsg(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, const char *msg*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_sendDiscussMsg*/
int luai_sendDiscussMsg(lua_State * state) {
	/*int32_t AuthCode, int64_t discussid, const char *msg*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_sendLike*/
int luai_sendLike(lua_State * state) {
	/*int32_t AuthCode, int64_t QQID*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupKick*/
int luai_setGroupKick(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, CQBOOL rejectaddrequest*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupBan*/
int luai_setGroupBan(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, int64_t duration*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupAdmin*/
int luai_setGroupAdmin(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, CQBOOL setadmin*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupWholeBan*/
int luai_setGroupWholeBan(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, CQBOOL enableban*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupAnonymousBan*/
int luai_setGroupAnonymousBan(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, const char *anomymous, int64_t duration*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupAnonymous*/
int luai_setGroupAnonymous(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, CQBOOL enableanomymous*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupCard*/
int luai_setGroupCard(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, const char *newcard*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupLeave*/
int luai_setGroupLeave(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, CQBOOL isdismiss*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupSpecialTitle*/
int luai_setGroupSpecialTitle(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, const char *newspecialtitle, int64_t duration*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setDiscussLeave*/
int luai_setDiscussLeave(lua_State * state) {
	/*int32_t AuthCode, int64_t discussid*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setFriendAddRequest*/
int luai_setFriendAddRequest(lua_State * state) {
	/*int32_t AuthCode, const char *responseflag, int32_t responseoperation, const char *remark*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupAddRequestV2*/
int luai_setGroupAddRequestV2(lua_State * state) {
	/*int32_t AuthCode, const char *responseflag, int32_t requesttype, int32_t responseoperation, const char *reason*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getGroupMemberInfoV2*/
int luai_getGroupMemberInfoV2(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, CQBOOL nocache*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getStrangerInfo*/
int luai_getStrangerInfo(lua_State * state) {
	/*int32_t AuthCode, int64_t QQID, CQBOOL nocache*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_addLog*/
int luai_addLog(lua_State * state) {
	/*int32_t AuthCode, int32_t priority, const char *category, const char *content*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getCookies*/
int luai_getCookies(lua_State * state) {
	/*int32_t AuthCode*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getCsrfToken*/
int luai_getCsrfToken(lua_State * state) {
	/*int32_t AuthCode*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getLoginQQ*/
int luai_getLoginQQ(lua_State * state) {
	/*int32_t AuthCode*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getLoginNick*/
int luai_getLoginNick(lua_State * state) {
	/*int32_t AuthCode*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getAppDirectory*/
int luai_getAppDirectory(lua_State * state) {
	/*int32_t AuthCode*/

}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setFatal*/
int luai_setFatal(lua_State * state) {
	/*int32_t AuthCode, const char *errorinfo*/

}


