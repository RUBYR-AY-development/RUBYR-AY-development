-- serverside of the 'Input' script 

--// MAIN CODE
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

--////////////////
--// KEYBOARD CODE
--////////////////

local module = {}

local Promise = require(path.to.promise)

local next_hook_id

function module.keyNew(key : Enum.KeyCode,assignedPlayer : Player)
	local External = {}
	local Internal = {}
	next_hook_id = game.ReplicatedStorage.Values.Shared.HookID[assignedPlayer.Name].next_hook_id
	
	Internal._hook_folder = Instance.new("Folder")
	Internal._hook_folder.Parent = game.ReplicatedStorage.Remotes.ClientToServer.InputHooks.Key[assignedPlayer.Name]
	Internal._hook_folder.Name = next_hook_id.Value
	
	Internal._hook_pressed_event = Instance.new("RemoteEvent")
	Internal._hook_pressed_event.Name = "Pressed"
	Internal._hook_pressed_event.Parent = Internal._hook_folder
	
	Internal._hook_released_event = Instance.new("RemoteEvent")
	Internal._hook_released_event.Name = "Released"
	Internal._hook_released_event.Parent = Internal._hook_folder
	
	Internal._hook_id = next_hook_id.Value

	Internal._pressed_enabled = false
	Internal._pressed_f = nil

	Internal._released_enabled = false
	Internal._released_f = nil

	--// promise establishment
	Internal._promise_handler = function()
		return Promise.new(function(resolve,reject,onCancel)
			local pressedEvent = Internal._hook_pressed_event.OnServerEvent:Connect(function(player,params)
				if player.Name == assignedPlayer.Name and Internal._pressed_enabled then
					Internal.Pressed(params)
				end
			end)
			local releasedEvent = Internal._hook_released_event.OnServerEvent:Connect(function(player,params)
				if player.Name == assignedPlayer.Name and Internal._released_enabled then
					Internal.Released(params)
				end
			end)

			--print("[Input.keyNew()]: Established successfully.")

			next_hook_id.Value += 1
			if onCancel(function()
					pressedEvent = nil
					releasedEvent = nil
					Internal._hook_folder:Destroy()
					--inputBegan:Disconnect()
					--inputReleased:Disconnect()
					--print("[Input.keyNew()]: Disconnected.")
				end) then return end
		end)
	end
	Internal._promise = Internal._promise_handler()
	:catch(function()
		warn("[InputHook.keyNew()]: Unable to establish.")
		Internal._hook_folder:Destroy()
	end)


	--// internal func
	function Internal.Pressed(params)
		Internal._pressed_f(params)
	end
	function Internal.Released(params)
		Internal._released_f(params)
	end

	--// external func
	function External.Pressed(f)
		Internal._pressed_f = f
		Internal._pressed_enabled = true
	end
	function External.Released(f)
		Internal._released_f = f
		Internal._released_enabled = true
	end
	function External.Teardown()
		Internal._promise:cancel()
	end

	return External
end
return module

--///////////////
--// MOUSE CODE
--///////////////

local module = {}

local Promise = require(path.to.Promise)

local next_hook_id

function module.mouseNew(mouseType : Enum.UserInputType,assignedPlayer : Player)
	local External = {}
	local Internal = {}
	next_hook_id = game.ReplicatedStorage.Values.Shared.HookID[assignedPlayer.Name].next_hook_id
	
	Internal._hook_folder = Instance.new("Folder")
	Internal._hook_folder.Parent = game.ReplicatedStorage.Remotes.ClientToServer.InputHooks[assignedPlayer.Name]
	Internal._hook_folder.Name = next_hook_id.Value
	
	Internal._hook_mousetype = Instance.new("StringValue")
	Internal._hook_mousetype.Name = "mouseType"
	Internal._hook_mousetype.Value = tostring(mouseType)
	Internal._hook_mousetype.Parent = Internal._hook_folder
	
	Internal._hook_pressed_event = Instance.new("RemoteEvent")
	Internal._hook_pressed_event.Name = "Pressed"
	Internal._hook_pressed_event.Parent = Internal._hook_folder
	
	Internal._hook_released_event = Instance.new("RemoteEvent")
	Internal._hook_released_event.Name = "Released"
	Internal._hook_released_event.Parent = Internal._hook_folder
	
	Internal._hook_id = next_hook_id.Value

	Internal._pressed_enabled = false
	Internal._pressed_f = nil

	Internal._released_enabled = false
	Internal._released_f = nil

	--// promise establishment
	Internal._promise_handler = function()
		return Promise.new(function(resolve,reject,onCancel)
			local pressedEvent = Internal._hook_pressed_event.OnServerEvent:Connect(function(player,params)
				if player.Name == assignedPlayer.Name and Internal._pressed_enabled then
					Internal.Pressed(params)
				end
			end)
			local releasedEvent = Internal._hook_released_event.OnServerEvent:Connect(function(player,params)
				if player.Name == assignedPlayer.Name and Internal._released_enabled then
					Internal.Released(params)
				end
			end)

			--print("[Input.keyNew()]: Established successfully.")

			next_hook_id.Value += 1
			if onCancel(function()
					pressedEvent = nil
					releasedEvent = nil
					Internal._hook_folder:Destroy()
					--inputBegan:Disconnect()
					--inputReleased:Disconnect()
					--print("[Input.keyNew()]: Disconnected.")
				end) then return end
		end)
	end
	Internal._promise = Internal._promise_handler()
	:catch(function()
		warn("[InputHook.mouseNew()]: Unable to establish.")
		Internal._hook_folder:Destroy()
	end)


	--// internal func
	function Internal.Pressed(params)
		Internal._pressed_f(params)
	end
	function Internal.Released(params)
		Internal._released_f(params)
	end

	--// external func
	function External.Pressed(f)
		Internal._pressed_f = f
		Internal._pressed_enabled = true
	end
	function External.Released(f)
		Internal._released_f = f
		Internal._released_enabled = true
	end
	function External.Teardown()
		Internal._promise:cancel()
	end

	return External
end
return module
