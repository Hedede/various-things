function replace(a, b)
	return a .. "_" .. b:lower();
end

function to_snake_gl(name)
	local patterns = {
		"([a-z])(u?i[0-9][a-z]+)$",
		"([a-z])([0-9][A-Z]?[0-9a-z]*[a-z])$",
		"(.)([A-Z][a-z]+)",
		"([a-z])([0-9][A-Z]+)",
		"([a-z])([A-Z])"
	};

	for _,pattern in ipairs(patterns) do
		name = name:gsub(pattern, replace);
	end

	return name:lower();
end

function to_snake(name)
	local patterns = {
		"(.)([A-Z][a-z]+)",
		"([a-z])([0-9][A-Z]+)",
		"([a-z])([A-Z][0-9A-Z]*)"
	};

	for _,pattern in ipairs(patterns) do
		name = name:gsub(pattern, replace);
	end

	return name:lower();
end

function test(func, strs)
	for _, p in ipairs(strs) do
		local snake  = func(p[1]);
		local expect = p[2]
		if (snake ~= expect) then
			print(snake .. " !" .. expect);
		else
			print(snake);
		end
	end
end

test(to_snake, {
	{ "getSomeSnacks", "get_some_snacks" },
	{ "aZ", "a_z" },
	{ "Vec3", "vec3" },
	{ "vec3", "vec3" },
	{ "D3DCompile", "d3d_compile" },
	{ "D2D1Vec3Length", "d2d1_vec3_length" },
	{ "Sound3DCreate", "sound_3d_create" },
	{ "LoadXMLMap", "load_xml_map" },
	{ "ParseXML", "parse_xml" },
	{ "HDFParser", "hdf_parser" },
	{ "HDFToXML", "hdf_to_xml" },
	{ "VarI8FromUI8", "var_i8_from_ui8" },
	{ "VarUI8FromBool", "var_ui8_from_bool" },
	{ "LoadLibraryW", "load_library_w" },
	{ "Int32ToBytes", "int32_to_bytes" },
	{ "BytesFromInt32", "bytes_from_int32" },
});

test(to_snake_gl, {
	{ "glTexImage3D", "gl_tex_image_3d" },
	{ "glVertexAttrib4Nbv", "gl_vertex_attrib_4nbv" },
	{ "glUniformMatrix4x3fv", "gl_uniform_matrix_4x3fv" }
});
