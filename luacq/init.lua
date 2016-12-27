CQ = {}
CQ.__index = CQ

CQLuaIf_DebugWrite("Init.lua is Loading...")

CQ.handlerClasses = {
               ["Startup"] =  0, -- 应用启动
                  ["Exit"] =  1, -- 应用程序即将退出
                ["Enable"] =  2, -- 应用启用
               ["Disable"] =  3, -- 应用被禁用
            ["PrivateMsg"] =  4, -- 收到私聊消息
              ["GroupMsg"] =  5, -- 收到群消息
            ["DiscussMsg"] =  6, -- 收到讨论组消息
         ["GroupAdminSet"] =  7, -- 管理员变动 subTypes: [1] = 被取消管理员, [2] = 被设置管理员
  ["GroupMemberDecreased"] =  8, -- 群员减少事件 subTypes: [1] = 群员离开, [2] = 群员被踢, [3] = 自己(即登录号)被踢
  ["GroupMemberIncreased"] =  9, -- 群员增加事件 subTypes: [1] = 管理员已同意, [2] = 管理员邀请
           ["FriendAdded"] = 10, -- 好友已添加
   ["FriendAddRequesting"] = 11, -- 收到添加好友请求
    ["GroupAddRequesting"] = 12  -- subTypes: [1] = 他人申请入群, [2] = 自己(即登录号)收到入群邀请
}
--[==[
	
	Event Startup();
	Event Exit();
	Event Enable();
	Event Disable();
	Event PrivateMsg(msgType, senderID, sendTime, Msg, Font);
	Event GroupMsg(msgType, groupID, senderID, Anonymous, sendTime, Msg, Font);
	Event DiscussMsg(msgType, discussID, senderID, sendTime, Msg, Font);
	Event GroupAdminSet(operationType, groupID, infectedID, sendTime);
	Event GroupMemberDecreased(infoType, groupID, sendTime, infectedID, operatorID);
	Event GroupMemberIncreased(infoType, groupID, sendTime, infectedID, operatorID);
	Event FriendAdded(infoType, sendTime, infectedID);
	Event FriendAddRequesting(requestType, sendTime, senderID, Msg) returns RESPONSE;
	Event GroupAddRequesting(requestType, sendTime, senderID, Msg) returns RESPONSE;

]==]--
function CQ.new()
	local self = setmetatable({}, CQ)
	self.RegisteredHanelers = 0
	self.EventHandlers = {}

	function self.RegisterHandler(instance, hClass, description, func)
		CQLuaIf_DebugWrite(string.format("CQ.RegisterHandler(%s,%s,%s)", hClass,description,func))
		local tbl = {
			class = hClass, 
			desc = description, 
			handler = func
		}
		table.insert(instance.EventHandlers, tbl)
		instance.RegisteredHanelers = instance.RegisteredHanelers + 1
	end


	function self.PostMessage(hClass, ...)
		local args = table.pack(...)

		CQLuaIf_DebugWrite("PostMessage Called. with ".. (args.n) .. " Extra args: ")

		-- hClass = tonumber(args[0])
		CQLuaIf_DebugWrite("   hClass = " .. hClass .."")
		for i=1,args.n do
			CQLuaIf_DebugWrite("[EXTRA_ARG]    "..tostring(args[i]).."")
		end

		for i,evh in ipairs(self.EventHandlers) do
			CQLuaIf_DebugWrite("For...")
			CQLuaIf_DebugWrite("HandlerType: "..tostring(evh.class).."")
			if evh.class==hClass then
				CQLuaIf_DebugWrite("Calling Handler: "..tostring(evh.desc).."")
				local ret = evh.handler(...)
				CQLuaIf_DebugWrite("Handler done(".. tostring(ret) ..").")
				return ret

 				--[[
					if     hClass == CQ.handlerClasses["Startup"] then
						evh[1](...)
					elseif hClass == CQ.handlerClasses["Exit"] then
						evh[1](...)
					elseif hClass == CQ.handlerClasses["Enable"] then
						evh[1]()
					elseif hClass == CQ.handlerClasses["Disable"] then
						evh[1]()
					elseif hClass == CQ.handlerClasses["PrivateMsg"] then
						evh[1]()
					elseif hClass == CQ.handlerClasses["GroupMsg"] then
						evh[1]()
					elseif hClass == CQ.handlerClasses["DiscussMsg"] then
						evh[1]()
					elseif hClass == CQ.handlerClasses["System_GroupAdmin"] then
						evh[1]()
					elseif hClass == CQ.handlerClasses["System_GroupMemberDecrease"] then
						evh[1]()
					elseif hClass == CQ.handlerClasses["System_GroupMemberIncrease"] then
						evh[1]()
					elseif hClass == CQ.handlerClasses["Friend_Add"] then
						evh[1]()
					elseif hClass == CQ.handlerClasses["Request_AddFriend"] then
						evh[1]()
					elseif hClass == CQ.handlerClasses["Request_AddGroup"] then
						evh[1]()
					end
				]]--
			end
		end
	end
	
	return self
end

CQAPI = CQ.new()
CQLuaIf_SetInterfaceName("CQAPI") -- calls C interface defined in C code to set this new instance as interface.

function aha(msgType, senderID, sendTime, Msg, Font)
	CQLuaIf_DebugWrite("before CQLuaIf_sendPrivateMsg()")
	-- CQLuaIf_sendPrivateMsg  
	if pcall(CQLuaIf_sendPrivateMsg, senderID, string.format("(Handler Called) Hello from %s\n你跟我说了:%s", _VERSION, Msg)) then
		CQLuaIf_DebugWrite("CQLuaIf_sendPrivateMsg() ->Done")
	else
		CQLuaIf_DebugWrite("CQLuaIf_sendPrivateMsg() ->failed")
	end
 
	CQLuaIf_DebugWrite("after CQLuaIf_sendPrivateMsg(),return")
	return 1
	-- io.write(string.format("(Handler Called) Hello from %s\n", _VERSION))
end

CQAPI:RegisterHandler(CQ.handlerClasses["PrivateMsg"],"A simple onPrivateMessage Handler.",aha)

