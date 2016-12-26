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
#include "DebugLogging.h"

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
	if (message[0] == 0) {
		Debug_Write((char*)message);
		puts(message);
	}else{
		Debug_Write("lua_throw: lua reported error with <Null String>");
		puts("lua_throw: lua reported error with <Null String>");
	}

	lua_pop(state, 1);
}

lua_State * lua_doInit(){
	lua_State *state = luaL_newstate();
	luaL_openlibs(state);
	luai_loadWrappers(state);
 
	if (file_exists(".\\init.lua")) {
		Debug_Write("init.lua found\n");
		Debug_Write("lua_doInit()::luaL_loadfile()\n");
		int result = luaL_loadfile(state, ".\\init.lua");
		Debug_Write(" => function call done.\n");

		if (result != LUA_OK) {
			Debug_Write("fatal: luaL_loadfile() failed.\n");
			lua_throw(state);
			CQ_setFatal(ac, "Lua Engine failed to load initial script(.\\init.lua), please consider reinstall it.");
		}
 
		Debug_Write("lua_doInit()::Lua_pcall()\n");

		lua_pcall(state, 0, 0, 0);

		if (result != LUA_OK) {
			lua_throw(state);
			CQ_setFatal(ac, "Lua Engine failed to Execute initial script (.\\init.lua) since it may corrupted, please consider reinstall it.");
			return 0;
		}
	}else{
		Debug_Write("Lua Engine has been disabled since it finds no initial script.\n");
		CQ_setFatal(ac, "Lua Engine has been disabled since it finds no initial script.");
	}

	return state;

}

void luai_loadWrappers(lua_State * state) {

	//Program Interface.
	lua_register(state, "CQLuaIf_SetInterfaceName", CQLuaI_SetInterfaceName);
	lua_register(state, "CQLuaIf_DebugWrite", CQLuaI_DebugWrite);

	//API Wrappers
	lua_register(state, "CQLuaIf_sendPrivateMsg", luai_sendPrivateMsg);
	lua_register(state, "CQLuaIf_sendGroupMsg", luai_sendGroupMsg);
	lua_register(state, "CQLuaIf_sendDiscussMsg", luai_sendDiscussMsg);
	lua_register(state, "CQLuaIf_sendLike", luai_sendLike);
	lua_register(state, "CQLuaIf_setGroupKick", luai_setGroupKick);
	lua_register(state, "CQLuaIf_setGroupBan", luai_setGroupBan);
	lua_register(state, "CQLuaIf_setGroupAdmin", luai_setGroupAdmin);
	lua_register(state, "CQLuaIf_setGroupWholeBan", luai_setGroupWholeBan);
	lua_register(state, "CQLuaIf_setGroupAnonymousBan", luai_setGroupAnonymousBan);
	lua_register(state, "CQLuaIf_setGroupAnonymous", luai_setGroupAnonymous);
	lua_register(state, "CQLuaIf_setGroupCard", luai_setGroupCard);
	lua_register(state, "CQLuaIf_setGroupLeave", luai_setGroupLeave);
	lua_register(state, "CQLuaIf_setGroupSpecialTitle", luai_setGroupSpecialTitle);
	lua_register(state, "CQLuaIf_setDiscussLeave", luai_setDiscussLeave);
	lua_register(state, "CQLuaIf_setFriendAddRequest", luai_setFriendAddRequest);
	lua_register(state, "CQLuaIf_setGroupAddRequestV2", luai_setGroupAddRequestV2);
	lua_register(state, "CQLuaIf_getGroupMemberInfoV2", luai_getGroupMemberInfoV2);
	lua_register(state, "CQLuaIf_getStrangerInfo", luai_getStrangerInfo);
	lua_register(state, "CQLuaIf_addLog", luai_addLog);
	lua_register(state, "CQLuaIf_getCookies", luai_getCookies);
	lua_register(state, "CQLuaIf_getCsrfToken", luai_getCsrfToken);
	lua_register(state, "CQLuaIf_getLoginQQ", luai_getLoginQQ);
	lua_register(state, "CQLuaIf_getLoginNick", luai_getLoginNick);
	lua_register(state, "CQLuaIf_getAppDirectory", luai_getAppDirectory);
	lua_register(state, "CQLuaIf_setFatal", luai_setFatal);

}

int CQLuaI_DebugWrite(lua_State * state) {
	int args = lua_gettop(state);
	if (args > 0) {
		for (int i = 1; i <= args; i++) {
			Debug_Write((char *)lua_tostring(state, i));
		}
	}
	return 0;
}


int CQLuaI_SetInterfaceName(lua_State * state) {

	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args > 0) {
		char * iName =(char *)lua_tostring(state, 1);
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
	Debug_Write("CPP - LuaWrapper::SendPrivateMessage\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 2) {
		int64_t toID = lua_tointeger(state, 1);
		char * msg = (char *)lua_tostring(state, 2);
		Debug_Write("CPP - LuaWrapper::SendPrivateMessage -> Calling CQAPI\n");
		char s[30];
		sprintf(s, "   arg: toID: %ld\n", toID);
		Debug_Write(s);
		sprintf(s, "   arg: msg: %s\n", msg);
		Debug_Write(s);

		int ret = CQ_sendPrivateMsg(ac, toID, msg);
		
		sprintf(s, "retval = %d\n", ret);
		Debug_Write("CPP - LuaWrapper::SendPrivateMessage -> API Called.\n");
		Debug_Write("CPP - LuaWrapper::SendPrivateMessage -> returned ");
		Debug_Write(s);

		//lua_pushnumber(state, 0);
	}
	else {
		Debug_Write("CPP - LuaWrapper::SendPrivateMessage -> Arguments Check Failed.\n");
		//lua_pushnumber(state, -1);
	}

	// Push the return value on top of the stack. NOTE: We haven't popped the
	// input arguments to our function. To be honest, I haven't checked if we
	// must, but at least in stack machines like the JVM, the stack will be
	// cleaned between each function call.
	// Let Lua know how many return values we've passed
	Debug_Write("CPP - LuaWrapper::SendPrivateMessage -> Wrapper Return.\n"); 
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_sendGroupMsg*/
int luai_sendGroupMsg(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, const char *msg*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_sendDiscussMsg*/
int luai_sendDiscussMsg(lua_State * state) {
	/*int32_t AuthCode, int64_t discussid, const char *msg*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_sendLike*/
int luai_sendLike(lua_State * state) {
	/*int32_t AuthCode, int64_t QQID*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupKick*/
int luai_setGroupKick(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, CQBOOL rejectaddrequest*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupBan*/
int luai_setGroupBan(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, int64_t duration*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupAdmin*/
int luai_setGroupAdmin(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, CQBOOL setadmin*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupWholeBan*/
int luai_setGroupWholeBan(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, CQBOOL enableban*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupAnonymousBan*/
int luai_setGroupAnonymousBan(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, const char *anomymous, int64_t duration*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupAnonymous*/
int luai_setGroupAnonymous(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, CQBOOL enableanomymous*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupCard*/
int luai_setGroupCard(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, const char *newcard*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupLeave*/
int luai_setGroupLeave(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, CQBOOL isdismiss*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupSpecialTitle*/
int luai_setGroupSpecialTitle(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, const char *newspecialtitle, int64_t duration*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setDiscussLeave*/
int luai_setDiscussLeave(lua_State * state) {
	/*int32_t AuthCode, int64_t discussid*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setFriendAddRequest*/
int luai_setFriendAddRequest(lua_State * state) {
	/*int32_t AuthCode, const char *responseflag, int32_t responseoperation, const char *remark*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setGroupAddRequestV2*/
int luai_setGroupAddRequestV2(lua_State * state) {
	/*int32_t AuthCode, const char *responseflag, int32_t requesttype, int32_t responseoperation, const char *reason*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getGroupMemberInfoV2*/
int luai_getGroupMemberInfoV2(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, CQBOOL nocache*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getStrangerInfo*/
int luai_getStrangerInfo(lua_State * state) {
	/*int32_t AuthCode, int64_t QQID, CQBOOL nocache*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_addLog*/
int luai_addLog(lua_State * state) {
	/*int32_t AuthCode, int32_t priority, const char *category, const char *content*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getCookies*/
int luai_getCookies(lua_State * state) {
	/*int32_t AuthCode*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getCsrfToken*/
int luai_getCsrfToken(lua_State * state) {
	/*int32_t AuthCode*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getLoginQQ*/
int luai_getLoginQQ(lua_State * state) {
	/*int32_t AuthCode*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getLoginNick*/
int luai_getLoginNick(lua_State * state) {
	/*int32_t AuthCode*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_getAppDirectory*/
int luai_getAppDirectory(lua_State * state) {
	/*int32_t AuthCode*/
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
/*Lua interface for CQ_setFatal*/
int luai_setFatal(lua_State * state) {
	/*int32_t AuthCode, const char *errorinfo*/
	return 0;
}


