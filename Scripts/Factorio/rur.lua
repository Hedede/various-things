function delete_chunks(x1,x2,y1,y2)
	for x=x1,x2 do
		for y=y1,y2 do
			game.surfaces.nauvis.delete_chunk{x,y}
		end
	end
end


function remove_ore(r, name)
	local pos=game.player.position
	local surface=game.player.surface

	local filter={
		area={{pos.x-r,pos.y-r},{pos.x+r,pos.y+r}},
		type="resource"
	}
	for k,v in pairs(surface.find_entities_filtered(filter)) do
		if v.name:match(name) then
			v.destroy()
		end
	end
end
