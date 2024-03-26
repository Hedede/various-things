function print_table(table)
	for k,v in pairs(table) do
		if type(v) == "table" then
			print(k.."={")
			print_table(v)
			print('}')
		elseif type(v) == "string" then
			print(k..'="'..v..'"')
		else
			print(k.."="..tostring(v))
		end
	end
end
