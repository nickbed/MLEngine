--object_spawner.lua
OSpawner = {
	iCount = 0,

	Start = function(self)
	end,

	Update = function(dt)
		if (key.Pressed("space")) 
		then
			print("instance_spawned " .. tostring(OSpawner.iCount))
			t = scene.NewObject("spawned_" .. tostring(OSpawner.iCount), true)
			mesh = staticmesh.New("tmesh")
			mesh.Model = "data\\models\\splash.obj"
			mesh.Texture = "data\\images\\Menu.tga"
		
			t.components.Add(t.components, "staticmesh", mesh)
		
			math.randomseed(os.clock())
			t.transform.position = Vector3(math.random(-50,50), ypos, math.random(-50,50))
		
			OSpawner.iCount = OSpawner.iCount + 1
		end
	end,

	Collision = function(other)
	end
}