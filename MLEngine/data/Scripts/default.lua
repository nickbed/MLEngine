--Default.lua
Default = {}
Default.elapsedTime = 0
Default.this = 0

function Default.Start(self)
	Default.this = self
	print("welcome back")
end

function Default.Update(dt)
	Default.elapsedTime = Default.elapsedTime + dt
	print(Default.elapsedTime)
end

return Default