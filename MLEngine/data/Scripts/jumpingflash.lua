--jumpingflash.lua
JFlash = {}
JFlash.this = nil
JFlash.yvel = 0
JFlash.candouble = 0

function JFlash.Start(self)
	JFlash.this = self
end

function JFlash.Update(dt)
	JFlash.yvel = JFlash.yvel-(5*dt);
	
	
	ypos = JFlash.this.transform.position.y + JFlash.yvel
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
	
	JFlash.this.transform.position = Vector3(JFlash.this.transform.position.x, ypos, JFlash.this.transform.position.z)
end

function JFlash.Collision(other)

end

return JFlash