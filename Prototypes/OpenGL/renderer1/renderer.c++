#include <aw/graphics/gl/gl_ext33.h>
#include <aw/types/string_view.h>
#include <aw/utility/string/split.h>
#include <aw/math/matrix3.h>
#include <aw/math/math.h>

#include <iostream>
#include <algorithm>
#include <vector>

#include <aw/graphics/gl/shader.h>
#include "program.h"

#include <aw/graphics/gl/shader_file.h>

namespace aw::gl3 {
using namespace sv_literals;

optional<program> test_program;

void initialize_program()
{
	std::vector<shader> shaderList;

	auto vsh = load_shader( gl::shader_type::vertex,   "vert.glsl" );
	auto fsh = load_shader( gl::shader_type::fragment, "frag.glsl" );

	if (vsh && fsh) {
		shaderList.push_back(std::move(*vsh));
		shaderList.push_back(std::move(*fsh));
	}

	test_program = program();
	test_program->link( shaderList );
}

GLuint pbo;
GLuint vao;
/*const*/ float vertex_positions[] = {
     0.75f,  0.75f, 0.0f, 1.0f,
     0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
    -0.00f,  0.35f, 0.0f, 1.0f,
    -0.00f, -0.35f, 0.0f, 1.0f,
    -0.35f,  0.35f, 0.0f, 1.0f,

     1.0f,   0.0f,  0.0f, 1.0f,
     0.0f,   1.0f,  0.0f, 1.0f,
     0.0f,   0.0f,  1.0f, 1.0f,
     1.0f,   1.0f,  1.0f, 1.0f,
     1.0f,   1.0f,  1.0f, 1.0f,
     1.0f,   1.0f,  1.0f, 1.0f,
};

void initialize_scene()
{
	gl::gen_buffers( 1, &pbo );

	gl::bind_buffer( GL_ARRAY_BUFFER, pbo );
	gl::buffer_data( GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STREAM_DRAW );
	gl::bind_buffer( GL_ARRAY_BUFFER, 0 );
	
	gl::gen_vertex_arrays(1, &vao);
	gl::bind_vertex_array(vao);
}

int mx, my;
int fx, fy, hx, hy;
void reshape(int x, int y)
{
	float w = float(x)/y;
	float v = 4.0/3.0;

	int x2 = x;
	int y2 = y;
	if (w > v)
		x2 *= v / w;
	if (v > w)
		y2 *= w / v;

	int xd = (x - x2)/2;
	int yd = (y - y2)/2;

	fx = xd;
	fy = yd;
	hx = x2;
	hy = y2;

	gl::viewport(xd, yd, x2, y2);
}

void clear()
{
	gl::clear_color( 0.0f, 0.0f, 0.0f, 0.0f );
	gl::clear( GL_COLOR_BUFFER_BIT );
}

void calc_positions()
{
	static std::vector<float> vec;
	vec.assign(std::begin(vertex_positions), std::end(vertex_positions));
	vec[20] = (2.0*mx) / hx - 1;
	vec[21] = (2.0*my) / hy - 1;

	gl::bind_buffer( GL_ARRAY_BUFFER, pbo );
	gl::buffer_sub_data( GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vec.data() );
}

void render()
{
	clear();
	gl::use_program( handle(*test_program) );

	calc_positions();
	gl::enable_vertex_attrib_array( 0 );
	gl::enable_vertex_attrib_array( 1 );
	gl::vertex_attrib_pointer( 0, 4, GL_FLOAT, false, 0, 0 );
	gl::vertex_attrib_pointer( 1, 4, GL_FLOAT, false, 0, 6*16 );

	gl::draw_arrays( GL_TRIANGLES, 0, 6 );

	gl::disable_vertex_attrib_array( 0 );
	gl::use_program( 0 );
}

} // namespace aw::gl3

#include <chrono>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <aw/utility/to_string.h>
namespace aw {
int main()
{
	using namespace gl3;
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.attributeFlags = sf::ContextSettings::Core;
	settings.majorVersion = 3;
	settings.minorVersion = 3;

	sf::Window window(sf::VideoMode(800, 600), "GL tut", sf::Style::Default, settings);

	auto result = gl::sys::load_functions_3_3();
	std::cout << "GL loaded, missing: " << result.num_missing() << '\n';

	initialize_program();
	initialize_scene();

	size_t ctr = 0;
	using namespace std::chrono;
	auto start = steady_clock::now();
	auto point = start;
	while (window.isOpen()) {
		sf::Event event;
		int x,y;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized) {
				reshape(event.size.width, event.size.height);
				x = event.size.width;
				y = event.size.height;
			}
			if (event.type == sf::Event::MouseMoved) {
				mx = sf::Mouse::getPosition(window).x;
				my = sf::Mouse::getPosition(window).y;

				my = window.getSize().y - my;

				mx = math::clamp( mx, fx, fx+hx );
				my = math::clamp( my, fy, fy+hy );

				mx -= fx;
				my -= fy;
			}
		}

		++ctr;
		auto now = steady_clock::now();
		if ((now - point) >= seconds{1}) {
			point = now;
			duration<double> dur = now - start;
			window.setTitle( "FPS: " + to_string(int(ctr / dur.count())) );
		}

		render();

		window.display();
	}
}
} // namespace aw

int main()
{
	return aw::main();
}
