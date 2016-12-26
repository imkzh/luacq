#ifndef __LUA_WRAPPERS_H__
#define __LUA_WRAPPERS_H__

	lua_State * lua_doInit();
	void luai_loadWrappers(lua_State * state);

#ifdef __cplusplus
	extern "C"
#endif
	int CQLuaI_SetInterfaceName(lua_State * state);

	// API Wrapper functions.
#ifdef __cplusplus
	extern "C"
#endif
	int luai_sendPrivateMsg(lua_State *);

#ifdef __cplusplus
	extern "C"
#endif
	int luai_sendGroupMsg(lua_State *);

#ifdef __cplusplus
	extern "C"
#endif
	int luai_sendDiscussMsg(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_sendLike(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setGroupKick(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setGroupBan(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setGroupAdmin(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setGroupWholeBan(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setGroupAnonymousBan(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setGroupAnonymous(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setGroupCard(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setGroupLeave(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setGroupSpecialTitle(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setDiscussLeave(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setFriendAddRequest(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setGroupAddRequestV2(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_getGroupMemberInfoV2(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_getStrangerInfo(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_addLog(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_getCookies(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_getCsrfToken(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_getLoginQQ(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_getLoginNick(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_getAppDirectory(lua_State *);
#ifdef __cplusplus
	extern "C"
#endif
	int luai_setFatal(lua_State *);

#endif