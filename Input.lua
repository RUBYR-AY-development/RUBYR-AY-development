--// This file will be split up into 3 seperate modules:
--// the first is named 'Input'
--// the second is named 'Keyboard' that will be inside 'Input'
--// the third is named 'Mouse' that will be inside 'Input'

-- MAIN INPUT CODE
local Input = {}

do
	for _,v in script:GetChildren() do
		local mod = require(v)
		for k,v in mod do
			Input[k] = v
		end
	end
end

return Input

--//////////////
-- KEYBOARD CODE
--//////////////
local module = {}
local User_Input_Service = game:GetService("UserInputService")

local Promise = require(path.to.Promise)

local next_hook_id = game.ReplicatedStorage.Values.Client.next_hook_id -- important for client/server connection

local player = game.Players.LocalPlayer

function module.keyNew(key)
	local External = {}
	local Internal = {}
	Internal._hook_id = next_hook_id.Value
	
	
	Internal._pressed_enabled = false
	Internal._pressed_f = nil
	Internal._pressed_gpe = false
	Internal._pressed_params = {}
	
	Internal._released_enabled = false
	Internal._released_f = nil
	Internal._released_gpe = false
	Internal._released_params = {}
	
	--// promise establishment
	Internal._promise_handler = function()
		return Promise.new(function(resolve,reject,onCancel)
			local inputBegan = User_Input_Service.InputBegan:Connect(function(_keycode,gpe)
				if _keycode.KeyCode == key and Internal._pressed_enabled and gpe == Internal._pressed_gpe then
					Internal.Pressed()
				end
			end)
			local inputReleased = User_Input_Service.InputEnded:Connect(function(_keycode,gpe)
				if _keycode.KeyCode == key and Internal._released_enabled and gpe == Internal._released_gpe then
					Internal.Released()
				end
			end)
			
			--print("[Input.keyNew()]: Established successfully.")
			next_hook_id.Value += 1
			if onCancel(function()
					inputBegan:Disconnect()
					inputReleased:Disconnect()
					--print("[Input.keyNew()]: Disconnected.")
				end) then return end
		end)
	end
	Internal._promise = Internal._promise_handler():catch(function()
		warn("[Input.keyNew()]: Unable to establish.")
	end)
	
	--// internal func
	function Internal.Pressed()
		Internal._pressed_f()
		if game.ReplicatedStorage.Remotes.ClientToServer.InputHooks.Key[player.Name][tostring(Internal._hook_id)] then
			local remoteHook = game.ReplicatedStorage.Remotes.ClientToServer.InputHooks.Key[player.Name][tostring(Internal._hook_id)]
			remoteHook.Pressed:FireServer(Internal._pressed_params)
		end
	end
	function Internal.Released()
		Internal._released_f()
		if game.ReplicatedStorage.Remotes.ClientToServer.InputHooks.Key[player.Name][tostring(Internal._hook_id)] then
			local remoteHook = game.ReplicatedStorage.Remotes.ClientToServer.InputHooks.Key[player.Name][tostring(Internal._hook_id)]
			remoteHook.Released:FireServer(Internal._released_params)
		end
	end
	
	--// external func
	function External.Pressed(f,gpe,params)
		assert(typeof(params) == "table","[Input.mouseNew]: bad parameter #3 isn't a table")
		Internal._pressed_f = f
		Internal._pressed_params = params
		Internal._pressed_enabled = true
	end
	function External.Released(f,gpe,params)
		assert(typeof(params) == "table","[Input.mouseNew]: bad parameter #3 isn't a table")
		Internal._released_f = f
		Internal._released_params = params
		Internal._released_enabled = true
	end
	function External.Teardown()
		Internal._promise:cancel()
	end
	
	return External
end

return module

--//////////////
-- MOUSE CODE
--//////////////

local module = {}
local User_Input_Service = game:GetService("UserInputService")

local Promise = require(path.to.Promise)

local next_hook_id = game.ReplicatedStorage.Values.Client.next_hook_id -- important for client/server connection

local player = game.Players.LocalPlayer

function module.mouseNew(mouseType)
	local External = {}
	local Internal = {}
	Internal._hook_id = next_hook_id.Value
	
	
	Internal._pressed_enabled = false
	Internal._pressed_f = nil
	Internal._pressed_params = {}

	Internal._released_enabled = false
	Internal._released_f = nil
	Internal._released_params = {}
	
	local player = game.Players.LocalPlayer
	local mouse = player:GetMouse()
	
	Internal._promise_handler = function()
		return Promise.new(function(resolve,reject,onCancel)
			local inputBegan = User_Input_Service.InputBegan:Connect(function(_keycode)
				if _keycode.UserInputType == mouseType and Internal._pressed_enabled and _keycode.UserInputState == Enum.UserInputState.Begin then
					Internal.Pressed()
				end
			end)
			local inputEnded = User_Input_Service.InputEnded:Connect(function(_keycode)
				if _keycode.UserInputType == mouseType and Internal._released_enabled and _keycode.UserInputState == Enum.UserInputState.End then
					Internal.Released()
				end
			end)

			--print("[Input.mouseNew()]: Established successfully.")
			next_hook_id.Value += 1
			if onCancel(function()
					
					--print("[Input.mouseNew()]: Disconnected.")
				end) then return end
		end)
	end
	Internal._promise = Internal._promise_handler():catch(function()
		warn("[Input.mouseNew()]: Unable to establish.")
	end)
	
	--// internal func
	function Internal.Pressed()
		Internal._pressed_f()
		if game.ReplicatedStorage.Remotes.ClientToServer.InputHooks.Mouse[player.Name][tostring(Internal._hook_id)] then
			local remoteHook = game.ReplicatedStorage.Remotes.ClientToServer.InputHooks.Mouse[player.Name][tostring(Internal._hook_id)]
			if remoteHook.mouseType.Value == tostring(mouseType) then
				remoteHook.Pressed:FireServer(Internal._pressed_params)
			end
		end
	end
	function Internal.Released()
		Internal._released_f()
		if game.ReplicatedStorage.Remotes.ClientToServer.InputHooks.Mouse[player.Name][tostring(Internal._hook_id)] then
			local remoteHook = game.ReplicatedStorage.Remotes.ClientToServer.InputHooks.Mouse[player.Name][tostring(Internal._hook_id)]
			if remoteHook.mouseType.Value == tostring(mouseType) then
				remoteHook.Released:FireServer(Internal._released_params)
			end
		end
	end
	
	--// external func
	function External.Pressed(f,params)
		assert(typeof(params) == "table","[Input.mouseNew]: bad parameter #2 isn't a table")
		Internal._pressed_f = f
		Internal._pressed_params = params
		Internal._pressed_enabled = true
	end
	function External.Released(f,params)
		assert(typeof(params) == "table","[Input.mouseNew]: bad parameter #2 isn't a table")
		Internal._released_f = f
		Internal._released_params = params
		Internal._released_enabled = true
	end
	function External.Teardown()
		Internal._promise:cancel()
	end
	
	--// funnies cuz y not
	function External.getX()
		return mouse.X
	end
	function External.getY()
		return mouse.Y
	end
	function External.CF3D()
		return mouse.Hit
	end
	function External.Target()
		return mouse.Target
	end
	
	return External
end

return module
