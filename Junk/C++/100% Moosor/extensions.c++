void enumerate_extensions()
{
	GLint n = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);

	std::vector<string_view> ext;
	for (GLint i=0; i<n; i++) {
		const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
		ext.push_back(extension);
	}

	std::sort(begin(ext), end(ext));
}
