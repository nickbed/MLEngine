--jump.lua
Enemy = {}
Enemy.target = nil
Enemy.loaded = 0
Enemy.speed = 5

function Enemy.Start(self)
end

function Enemy.Update(this, dt)
	if (Enemy.loaded == 0)
	then	
		Enemy.target = scene.FindObject("robot1")
		Enemy.loaded = 1
	end
	local distX = this.transform.position.x - Enemy.target.transform.position.x
	local distZ = this.transform.position.z - Enemy.target.transform.position.z
	
	if (distX + distZ < 75)
	then
		local xpercent = (Enemy.speed*(distX / (distX + distZ))) * dt
		local zpercent = (Enemy.speed*(distZ / (distX + distZ))) * dt
		if (xpercent > 0.025)
		then
			xpercent = 0.05
		end
		if (zpercent > 0.025)
		then
			zpercent = 0.025
		end
		this.transform.position = Vector3.New(this.transform.position.x - xpercent, this.transform.position.y, this.transform.position.z - zpercent)
	end
end

function Enemy.Collision(this, collision)
	
end

function Enemy.Reset()
	Enemy.target = nil
	Enemy.loaded = 0
end

return Enemy