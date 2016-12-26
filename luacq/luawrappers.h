#ifndef __LUA_WRAPPERS_H__
#define __LUA_WRAPPERS_H__

#ifdef __cplusplus
	#define ExternC extern "C"
#else
	#define ExternC
#endif


	lua_State * lua_doInit();
	void luai_loadWrappers(lua_State * state);
	void lua_throw(lua_State * state);

	ExternC int CQLuaI_DebugWrite(lua_State * state);
	ExternC int CQLuaI_SetInterfaceName(lua_State * state);

	// API Wrapper functions.
	ExternC int luai_sendPrivateMsg(lua_State *);
	ExternC int luai_sendGroupMsg(lua_State *);
	ExternC int luai_sendDiscussMsg(lua_State *);
	ExternC int luai_sendLike(lua_State *);
	ExternC int luai_setGroupKick(lua_State *);
	ExternC int luai_setGroupBan(lua_State *);
	ExternC int luai_setGroupAdmin(lua_State *);
	ExternC int luai_setGroupWholeBan(lua_State *);
	ExternC int luai_setGroupAnonymousBan(lua_State *);
	ExternC int luai_setGroupAnonymous(lua_State *);
	ExternC int luai_setGroupCard(lua_State *);
	ExternC int luai_setGroupLeave(lua_State *);
	ExternC int luai_setGroupSpecialTitle(lua_State *);
	ExternC int luai_setDiscussLeave(lua_State *);
	ExternC int luai_setFriendAddRequest(lua_State *);
	ExternC int luai_setGroupAddRequestV2(lua_State *);
	ExternC int luai_getGroupMemberInfoV2(lua_State *);
	ExternC int luai_getStrangerInfo(lua_State *);
	ExternC int luai_addLog(lua_State *);
	ExternC int luai_getCookies(lua_State *);
	ExternC int luai_getCsrfToken(lua_State *);
	ExternC int luai_getLoginQQ(lua_State *);
	ExternC int luai_getLoginNick(lua_State *);
	ExternC int luai_getAppDirectory(lua_State *);
	ExternC int luai_setFatal(lua_State *);

#endif