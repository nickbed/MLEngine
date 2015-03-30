--jump.lua
Jump = {}
Jump.yvel = 0
Jump.allowed = 2

function Jump.Start(self)
end

function Jump.Update(this, dt)
	Jump.yvel = Jump.yvel-(5*dt);
	if(Jump.yvel < -0.65)
	then
		Jump.yvel = -0.65
	end
	if (key.Pressed("space")) 
	then
		if (Jump.allowed == 2) 
		then
			Jump.yvel = Jump.yvel + 1
			Jump.allowed = 1
		elseif(Jump.allowed == 1)
		then
			if (Jump.yvel < 0 )
			then
				Jump.yvel = 1
				Jump.allowed = 0
			end
		end
	end
	
	this.transform.position = Vector3.New(this.transform.position.x, this.transform.position.y + Jump.yvel, this.transform.position.z)
end

function Jump.Collision(this, collision)
		if (collision.Top==true)
		then
			--this.transform.position = Vector3.New(this.transform.position.x, this.transform.position.y, this.transform.position.z)
			Jump.yvel = 0
			Jump.allowed = 2
		end
end

function Jump.Reset()
	Jump.yvel = 0
	Jump.allowed = 2
end

return Jump