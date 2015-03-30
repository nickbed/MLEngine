--jump.lua
Enemy = {}
Enemy.target = nil
Enemy.loaded = 0
Enemy.speed = 15

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
	
	local tX = distX
	local tY = distZ
	
	if (distX < 0) then tX = distX * -1 end	--Make sure X is positive
	if (distZ < 0) then tY = distZ * -1 end	--Make sure Z is positive
	
	local totalDist = tX + tY
	
	if (distX + distZ < 50)
	then
		local xpercent = (Enemy.speed*(distX / totalDist)) * dt --Calculate X amount to move
		local zpercent = (Enemy.speed*(distZ / totalDist)) * dt --Calculate Z amount to move

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