--Player.lua
Player = {}
Player.this = nil

function Player.Start(self)
	Player.this = self
end

function Player.Update(dt)
	
	Player.this.transform.position = Vector3(0,0,10)
	
	Player.this.transform.rotation = 
		Vector3(Player.this.transform.rotation.x+(dt*200),
		Player.this.transform.rotation.y+(dt*500), 
		Player.this.transform.rotation.z+(dt*96))
end

return Player