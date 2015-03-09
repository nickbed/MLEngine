--object_spawner.lua
OSpawner = {}
OSpawner.iCount = 0

function OSpawner.Start(self)

end

function OSpawner.Update(dt)
	if (key.Pressed("q")) 
	then
		print("instance_spawned")
		t = scene.NewObject("spawned_" .. tostring(OSpawner.iCount), true)
		mesh = staticmesh.New("tmesh")
		mesh.Model = "data\\models\\splash.obj"
		mesh.Texture = "data\\images\\Menu.tga"
		
		t.components.Add(t.components, "staticmesh", mesh)
		
		t.transform.position = Vector3(math.random(-200,200), ypos, math.random(-200,200))
		
		OSpawner.iCount = OSpawner.iCount + 1
	end
end

function OSpawner.Collision(other)

end

return OSpawner