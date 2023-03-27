local module = {}

function module.new()
	local External = {}
	local Internal = {}
	Internal.folder = Instance.new("Folder")
	Internal.folder.Parent = game.Lighting.ObjectPools
	Internal.folder.Name = "justanotherobjectpool"
	
	function External.Add(tab : table)
		assert(typeof(tab) == "table","[ObjectPool]: Bad parameter #1 isn't a table")
		for _,v in tab do
			v.Parent = Internal.folder
		end
	end
	function External.Get(obj : string)
		assert(typeof(obj) == "string","[ObjectPool]: Bad parameter #1 isn't a string")
		for _,v in Internal.folder:GetChildren() do
			if v.Name == obj then
				return v
			end
		end
		warn(string.format("[ObjectPool]: Unable to get object <string>. Name: %s",obj))
		return {}
	end
	function External.Teardown()
		Internal.folder:Destroy()
	end
	
	return External
end

return module
