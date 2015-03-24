--coin2.lua
Coin2 = {}
Coin2.this = nil
Coin2.rot = 0

function Coin2.Start(self)
	Coin2.this = self
end

function Coin2.Update(dt)
	Coin2.rot = Coin2.rot +(135*dt)
	if (Coin2.rot > 360)
	then
		Coin2.rot = Coin2.rot - 360
	end
		
	Coin2.this.transform.rotation = Vector3(Coin2.this.transform.rotation.x, Coin2.rot, Coin2.this.transform.rotation.z)
end

function Coin2.Collision(collision)
	print(Coin2.this.id)
	scene.RemoveObject(Coin2.this.id)
end

return Coin