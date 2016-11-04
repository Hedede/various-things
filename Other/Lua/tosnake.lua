function to_snake(name)
	local CAPSInCamel = "(.)([A-Z][a-z]+)";
	local CamelCase   = "([a-z0-9])([A-Z])";
	return name:gsub(CAPSInCamel,"%1_%2"):gsub(CamelCase, "%1_%2"):lower();
end

print(to_snake("getSomeSnacks"));
print(to_snake("aZ"));
print(to_snake('HDFParser'));
print(to_snake('HDFToXML'));
print(to_snake('LoadLibraryW'));
