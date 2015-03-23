--jump.lua
Jump = {}
Jump.yvel = 0
Jump.candouble = 0

function Jump.Start(self)
end

function Jump.Update(this, dt)
	Jump.yvel = Jump.yvel-(5*dt);
	
	
	ypos = this.transform.position.y + Jump.yvel
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
	
	this.transform.position = Vector3(this.transform.position.x, ypos, this.transform.position.z)
end

function Jump.Collision(this, collision)
		Jump.yvel = 0
		Jump.candouble = 1
		print (collision.AxisBox)
end

return Jump