--jump.lua
Jump = {}
Jump.this = nil
Jump.yvel = 0
Jump.candouble = 0

function Jump.Start(self)
	Jump.this = self
end

function Jump.Update(dt)
	Jump.yvel = Jump.yvel-(5*dt);
	
	
	ypos = Jump.this.transform.position.y + Jump.yvel
	if (ypos < 1) 
	then 
		Jump.yvel = 0
		ypos = 1
	end
	
	if (key.Pressed("space")) 
	then
		if (ypos == 1) 
		then
			Jump.yvel = Jump.yvel + 1
			Jump.candouble = 1
		elseif(Jump.candouble == 1)
		then
			if (Jump.yvel < 1 )
			then
				Jump.yvel = 1
				Jump.candouble = 0
			end
		end
	end
	
	Jump.this.transform.position = Vector3(Jump.this.transform.position.x, ypos, Jump.this.transform.position.z)
end

function Jump.Collision()
		Jump.yvel = 0
		print ("spoooky")
end

return Jump