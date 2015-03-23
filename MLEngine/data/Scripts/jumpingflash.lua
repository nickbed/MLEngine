<<<<<<< HEAD
JFlash = {}
JFlash.this = nil
JFlash.yvel = 0
JFlash.candouble = 0
=======
--jumpingflash.lua
JFlash = {
	gameobject = nil,
	yvel = 0,
	candouble = 0,
>>>>>>> origin/RossBranch

	Start = function(owner)
		JFlash.gameobject = owner
	end,

	Update = function(dt)
		JFlash.yvel = JFlash.yvel - (5*dt)
	
	
		ypos = JFlash.gameobject.transform.position.y + JFlash.yvel
		if (ypos < 0) 
		then 
			JFlash.yvel = 0
			ypos = 0
		end
	
		if (key.Pressed("space")) 
		then
			if (ypos == 0) 
			then
				JFlash.yvel = JFlash.yvel + 1
				JFlash.candouble = 1
			elseif(JFlash.candouble == 1)
			then
				if (JFlash.yvel < 0 )
				then
					JFlash.yvel = 1
					JFlash.candouble = 0
				end
			end
		end
	
		JFlash.gameobject.transform.position = Vector3(JFlash.gameobject.transform.position.x, ypos, JFlash.gameobject.transform.position.z)
	end,

<<<<<<< HEAD
return JFlash
=======
	Collision = function(other)
		JFlash.yvel = 0
		JFlash.candouble = 1
	end
}
>>>>>>> origin/RossBranch
