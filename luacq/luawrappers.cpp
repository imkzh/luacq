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

#include "iconv.h"

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
		Debug_Write("lua_throw: lua reported error but returned <Null String>");
		puts("lua_throw: lua reported error but returned <Null String>");
	}

	lua_pop(state, 1);
}

void gbk2utf(const char * source, char ** dest) {
	size_t datlen = strlen(source);
	size_t bufflen = 2048;
	char * message = (char *)calloc(bufflen, 1);
 
	*dest = message;

	char * pIn = (char*)source;
	char * pOut = (char*)message;

	iconv_t conv = iconv_open("UTF-8", "GBK");
	iconv(conv, (const char **)&pIn, &datlen, &pOut, &bufflen);
	iconv_close(conv);
}

void utf2gbk(const char * source, char ** dest) {
	size_t datlen = strlen(source);
	size_t bufflen = 2048;
	char * message = (char *)calloc(bufflen, 1);

	*dest = message;

	char * pIn = (char*)source;
	char * pOut = (char*)message;

	iconv_t conv = iconv_open("GBK", "UTF-8");
	iconv(conv, (const char **)&pIn, &datlen, &pOut, &bufflen);
	iconv_close(conv);
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
			Debug_Write("[LUA] ");
			Debug_Write((char *)lua_tostring(state, i));
			Debug_Write("\n");
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

	return 0;
}

 
/*Lua interface for CQ_sendPrivateMsg*/
int luai_sendPrivateMsg(lua_State * state) {
	/*int32_t AuthCode, int64_t QQID, const char *msg*/
	Debug_Write("[CPP] LuaWrapper::SendPrivateMessage\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 2) {
		int64_t toID = lua_tointeger(state, 1);
		char * msg = (char *)lua_tostring(state, 2);
		size_t slen = strlen(msg);

		char * gbkmsg;
		utf2gbk(msg, &gbkmsg);

		int ret = CQ_sendPrivateMsg(ac, toID, gbkmsg);
		free(gbkmsg);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::SendPrivateMessage -> Arguments Check Failed(2 args required).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::SendPrivateMessage -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_sendGroupMsg*/
int luai_sendGroupMsg(lua_State * state) { 
	/*int32_t AuthCode, int64_t groupid, const char *msg*/
	Debug_Write("[CPP] LuaWrapper::sendGroupMsg\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 2) {
		int64_t toID = lua_tointeger(state, 1);
		char * msg = (char *)lua_tostring(state, 2);
		size_t slen = strlen(msg);

		char * gbkmsg;
		utf2gbk(msg, &gbkmsg);

		int ret = CQ_sendGroupMsg(ac, toID, gbkmsg);
		free(gbkmsg);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::sendGroupMsg -> Arguments Check Failed(2 args required).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::sendGroupMsg -> Wrapper Return.\n");
	return 0;
}
 
 
/*Lua interface for CQ_sendDiscussMsg*/
int luai_sendDiscussMsg(lua_State * state) {
	/*int32_t AuthCode, int64_t discussid, const char *msg*/
	Debug_Write("[CPP] LuaWrapper::sendDiscussMsg\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 2) {
		int64_t toID = lua_tointeger(state, 1);
		char * msg = (char *)lua_tostring(state, 2);
		size_t slen = strlen(msg);

		char * gbkmsg;
		utf2gbk(msg, &gbkmsg);
		
		int ret = CQ_sendDiscussMsg(ac, toID, gbkmsg);
		free(gbkmsg);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::sendDiscussMsg -> Arguments Check Failed(2 Args required).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::sendDiscussMsg -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_sendLike*/
int luai_sendLike(lua_State * state) {
	/*int32_t AuthCode, int64_t QQID*/
	Debug_Write("[CPP] LuaWrapper::sendLike\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 1) {
		int64_t toID = lua_tointeger(state, 1);
 
		int ret = CQ_sendLike(ac, toID);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::sendLike -> Arguments Check Failed(required 1 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::sendLike -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_setGroupKick*/
int luai_setGroupKick(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, CQBOOL rejectaddrequest*/
	Debug_Write("[CPP] LuaWrapper::setGroupKick\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 3) {
		int64_t groupID = lua_tointeger(state, 1);
		int64_t infectedID = lua_tointeger(state, 2);
		int32_t refusefurther = lua_tointeger(state, 3);
 
		int ret = CQ_setGroupKick(ac, groupID, infectedID, refusefurther);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::setGroupKick -> Arguments Check Failed(required 3 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::setGroupKick -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_setGroupBan*/
int luai_setGroupBan(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, int64_t duration*/
	Debug_Write("[CPP] LuaWrapper::setGroupBan\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 3) {
		int64_t groupID = lua_tointeger(state, 1);
		int64_t infectedID = lua_tointeger(state, 2);
		int64_t duration = lua_tointeger(state, 3);

		int ret = CQ_setGroupBan(ac, groupID, infectedID, duration);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::setGroupBan -> Arguments Check Failed(required 3 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::setGroupBan -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_setGroupAdmin*/
int luai_setGroupAdmin(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, CQBOOL setadmin*/
	Debug_Write("[CPP] LuaWrapper::setGroupAdmin\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 3) {
		int64_t groupID = lua_tointeger(state, 1);
		int64_t infectedID = lua_tointeger(state, 2);
		int64_t isAdmin = lua_tointeger(state, 3);

		int ret = CQ_setGroupAdmin(ac, groupID, infectedID, isAdmin);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::setGroupAdmin -> Arguments Check Failed(required 3 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::setGroupAdmin -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_setGroupWholeBan*/
int luai_setGroupWholeBan(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, CQBOOL enableban*/
	Debug_Write("[CPP] LuaWrapper::setGroupWholeBan\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 2) {
		int64_t groupID = lua_tointeger(state, 1);
		int64_t enabled = lua_tointeger(state, 2);

		int ret = CQ_setGroupWholeBan(ac, groupID, enabled);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::setGroupWholeBan -> Arguments Check Failed(required 2 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::setGroupWholeBan -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_setGroupAnonymousBan*/
int luai_setGroupAnonymousBan(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, const char *anomymous, int64_t duration*/
	Debug_Write("[CPP] LuaWrapper::setGroupAnonymousBan\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 3) {
		int64_t groupID = lua_tointeger(state, 1);
		char * name = (char *)lua_tostring(state, 2);
		int64_t duration = lua_tointeger(state, 3);
 
		char * gbkname;
		utf2gbk(name, &gbkname);	

		int ret = CQ_setGroupAnonymousBan(ac, groupID, gbkname, duration);
		free(gbkname);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::setGroupAnonymousBan -> Arguments Check Failed(required 3 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::setGroupAnonymousBan -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_setGroupAnonymous*/
int luai_setGroupAnonymous(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, CQBOOL enableanomymous*/
	Debug_Write("[CPP] LuaWrapper::setGroupAnonymous\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 2) {
		int64_t groupID = lua_tointeger(state, 1);
		char * name = (char *)lua_tostring(state, 2);
		int64_t duration = lua_tointeger(state, 3);

		char * gbkname;
		utf2gbk(name, &gbkname);

		int ret = CQ_setGroupAnonymousBan(ac, groupID, gbkname, duration);
		free(gbkname);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::setGroupAnonymous -> Arguments Check Failed(required 2 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::setGroupAnonymous -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_setGroupCard*/
int luai_setGroupCard(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, const char *newcard*/
	Debug_Write("[CPP] LuaWrapper::setGroupCard\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 2) {
		int64_t groupID = lua_tointeger(state, 1);
		int64_t infectedID = lua_tointeger(state, 2);
		char * name = (char *)lua_tostring(state, 3);

		char * gbkname;
		utf2gbk(name, &gbkname);

		int ret = CQ_setGroupCard(ac, groupID, infectedID, gbkname);
		free(gbkname);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::setGroupCard -> Arguments Check Failed(required 3 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::setGroupCard -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_setGroupLeave*/
int luai_setGroupLeave(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, CQBOOL isdismiss*/
	Debug_Write("[CPP] LuaWrapper::setGroupLeave\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 2) {
		int64_t groupID = lua_tointeger(state, 1);
		int32_t isdismiss = lua_tointeger(state, 2);
 
		int ret = CQ_setGroupLeave(ac, groupID, isdismiss);
 
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::setGroupLeave -> Arguments Check Failed(required 2 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::setGroupLeave -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_setGroupSpecialTitle*/
int luai_setGroupSpecialTitle(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, const char *newspecialtitle, int64_t duration*/
	Debug_Write("[CPP] LuaWrapper::setGroupSpecialTitle\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 4) {
		int64_t groupID = lua_tointeger(state, 1);
		int64_t infectedID = lua_tointeger(state, 2);
		char * title = (char *)lua_tostring(state, 3);
		int64_t duration = lua_tointeger(state, 4);

		char * gbktitle;
		utf2gbk(title, &gbktitle);

		int ret = CQ_setGroupSpecialTitle(ac, groupID, infectedID, gbktitle, duration);
		free(gbktitle);

		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::setGroupSpecialTitle -> Arguments Check Failed(required 4 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::setGroupSpecialTitle -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_setDiscussLeave*/
int luai_setDiscussLeave(lua_State * state) {
	/*int32_t AuthCode, int64_t discussid*/
	Debug_Write("[CPP] LuaWrapper::setGroupSpecialTitle\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 1) {
		int64_t discussID = lua_tointeger(state, 1);
	 
		int ret = CQ_setDiscussLeave(ac, discussID);

		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::setGroupSpecialTitle -> Arguments Check Failed(required 1 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::setGroupSpecialTitle -> Wrapper Return.\n");
	return 1;
}

 
/*Lua interface for CQ_setFriendAddRequest*/
int luai_setFriendAddRequest(lua_State * state) {
	/*int32_t AuthCode, const char *responseflag, int32_t responseoperation, const char *remark*/
	Debug_Write("[CPP] luai_setFriendAddRequest currently Not Implemented\n");
	return 0;
}

 
/*Lua interface for CQ_setGroupAddRequestV2*/
int luai_setGroupAddRequestV2(lua_State * state) {
	/*int32_t AuthCode, const char *responseflag, int32_t requesttype, int32_t responseoperation, const char *reason*/
	Debug_Write("[CPP] luai_setGroupAddRequestV2 currently Not Implemented\n");
	return 0;
}

 
/*Lua interface for CQ_getGroupMemberInfoV2*/
int luai_getGroupMemberInfoV2(lua_State * state) {
	/*int32_t AuthCode, int64_t groupid, int64_t QQID, CQBOOL nocache*/
	Debug_Write("[CPP] luai_getGroupMemberInfoV2 currently Not Implemented\n");
	return 0;
}

 
/*Lua interface for CQ_getStrangerInfo*/
int luai_getStrangerInfo(lua_State * state) {
	/*int32_t AuthCode, int64_t QQID, CQBOOL nocache*/
	Debug_Write("[CPP] luai_getStrangerInfo currently Not Implemented\n");
	return 0;
}

 
/*Lua interface for CQ_addLog*/
int luai_addLog(lua_State * state) {
	/*int32_t AuthCode, int32_t priority, const char *category, const char *content*/
	Debug_Write("You are Calling luai_addlog, Use 'CQLuaIf_DebugWrite(...)' which supports arbitrary amount of arguments input instead.\n");
	return 0;
}

 
/*Lua interface for CQ_getCookies*/
int luai_getCookies(lua_State * state) {
	/*int32_t AuthCode*/
	lua_pushstring(state, CQ_getCookies(ac));
	return 0;
}

 
/*Lua interface for CQ_getCsrfToken*/
int luai_getCsrfToken(lua_State * state) {
	/*int32_t AuthCode*/
	lua_pushnumber(state, CQ_getCsrfToken(ac));
	return 1;
}

 
/*Lua interface for CQ_getLoginQQ*/
int luai_getLoginQQ(lua_State * state) {
	/*int32_t AuthCode*/
	lua_pushnumber(state, CQ_getLoginQQ(ac));
	return 1;
}

 
/*Lua interface for CQ_getLoginNick*/
int luai_getLoginNick(lua_State * state) {
	/*int32_t AuthCode*/
	const char * path = CQ_getLoginNick(ac);

	char * utfName;
	gbk2utf(path, &utfName);

	lua_pushstring(state, utfName);
	free(utfName);
	return 1;
}

 
/*Lua interface for CQ_getAppDirectory*/
int luai_getAppDirectory(lua_State * state) {
	/*int32_t AuthCode*/
	const char * path = CQ_getAppDirectory(ac);

	char * utfpath;
	gbk2utf(path, &utfpath);
	
	lua_pushstring(state, utfpath);
	free(utfpath);
	return 1;
}

 
/*Lua interface for CQ_setFatal*/
int luai_setFatal(lua_State * state) {
	/*int32_t AuthCode, const char *errorinfo*/
	Debug_Write("[CPP] LuaWrapper::setFatal\n");
	// The number of function arguments will be on top of the stack.
	int args = lua_gettop(state);
	if (args == 1) {
		char * fatalmsg = (char *)lua_tostring(state, 1);
		Debug_Write("[CPP][LUA_FATAL]");
		Debug_Write(fatalmsg);
		char * gbkmsg;
		utf2gbk(fatalmsg, &gbkmsg);

		int ret = CQ_setFatal(ac, gbkmsg);
		free(gbkmsg);
		lua_pushnumber(state, ret);
	}
	else {
		lua_pop(state, args);
		Debug_Write("[CPP] LuaWrapper::setFatal -> Arguments Check Failed(required 1 Arg(s)).\n");
		lua_pushnumber(state, -1);
	}

	Debug_Write("[CPP] LuaWrapper::setFatal -> Wrapper Return.\n");
	return 1;
}


