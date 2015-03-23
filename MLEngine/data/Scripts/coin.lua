--coin.lua
Coin = {}
Coin.rot = 0

function Coin.Start(this)
end

function Coin.Update(this, dt)
	Coin.rot = Coin.rot +(90*dt)
	if (Coin.rot > 360)
	then
		Coin.rot = Coin.rot - 360
	end
		
	this.transform.rotation = Vector3(this.transform.rotation.x, Coin.rot, this.transform.rotation.z)
end

function Coin.Collision(this, collision)
	--print(this.name)
	scene.RemoveObject(this.name)
end

return Coin