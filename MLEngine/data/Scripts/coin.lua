--coin.lua
Coin = {}
Coin.rot = 135

function Coin.Start(this)
end

function Coin.Update(this, dt)	
	this.transform.rotation = Vector3.New(this.transform.rotation.x, this.transform.rotation.y + (Coin.rot*dt), this.transform.rotation.z)
end

function Coin.Collision(this, collision)
	scene.RemoveObject(this.name)
end

function Coin.Reset()

end

return Coin