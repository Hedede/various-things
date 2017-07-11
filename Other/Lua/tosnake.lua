function to_snake(name)
	local patterns = {
		"(.)([A-Z][a-z]+)",
		"([a-z])([0-9][A-Z])",
		"([a-z])([A-Z])"
	};

	for _,pattern in ipairs(patterns) do
		name = name:gsub(pattern, "%1_%2");
	end

	return name:lower();
end

print(to_snake("getSomeSnacks"));
print(to_snake("aZ"));
print(to_snake("Vec3"));
print(to_snake('HDFParser'));
print(to_snake('HDFToXML'));
print(to_snake('LoadLibraryW'));
print(to_snake('glTexImage3D'));
print(to_snake('Int32ToBytes'));
print(to_snake('BytesFromInt32'));

