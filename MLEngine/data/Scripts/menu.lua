--Menu.lua
Menu = {}

function Menu.Start(this)
end

function Menu.Update(this, dt)	
	if (key.Pressed("z")) 
	then
		scene.LoadNew("data\\scenes\\TomsWorld.scn")
	end
	if (key.Pressed("x")) 
	then
		scene.LoadNew("data\\scenes\\xc_sandyspines.scn")
	end
	if (key.Pressed("c")) 
	then
		scene.LoadNew("data\\scenes\\ice.scn")
	end
	if (key.Pressed("v")) 
	then
		scene.LoadNew("data\\scenes\\cliff.scn")
	end
end

function Menu.Collision(this, collision)
end

function Menu.Reset()

end

return Menu