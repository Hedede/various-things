#include <aw/graphics/gl/gl_ext33.h>
#include <aw/types/string_view.h>
#include <aw/utility/string/split.h>
#include <aw/math/matrix3.h>
#include <aw/math/math.h>

#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>

#include <aw/graphics/gl/shader.h>
#include <aw/graphics/gl/program.h>

#include <aw/graphics/gl/shader_file.h>

#include "renderer.h"

namespace aw::gl3 {
using namespace sv_literals;

struct program_handle {
	GLuint value;
	operator GLuint() { return value; }
};
optional<program> test_program;
uniform_location screen_location;
uniform_location time_location;
uniform_location period_location;
uniform_location campos_location;

void initialize_program()
{
	std::vector<shader> shaderList;

	auto vsh = load_shader( gl::shader_type::vertex,   "vert2.glsl" );
	auto fsh = load_shader( gl::shader_type::fragment, "frag.glsl" );

	if (vsh && fsh) {
		shaderList.push_back(std::move(*vsh));
		shaderList.push_back(std::move(*fsh));
	}

	test_program = program();
	test_program->link( shaderList );

	auto& program = *test_program;

	screen_location = program.uniform("screen");
	time_location   = program.uniform("time");
	period_location = program.uniform("period");
	campos_location = program.uniform("camera");

	gl::use_program( handle(program) );

	float frustum_scale = 1.0f;
	float zFar = 1.0f;
	float zNear = 3.0f;

	float s = frustum_scale;
	float z = (zFar + zNear) / (zNear - zFar);
	float w = 2 * zFar * zNear / (zNear - zFar);
	mat4 projmat = {
		s, 0, 0, 0,
		0, s, 0, 0,
		0, 0, z, w,
		0, 0, -1, 0,
	};

	program["perspective"] = projmat;

	program["frustum_scale"] = 1.0f;
	program["zNear"] = 1.0f;
	program["zFar"]  = 3.0f;
	gl::use_program( 0 );
}

GLuint pbo;
GLuint vao;


void initialize_scene()
{
	gl::gen_buffers( 1, &pbo );

	gl::bind_buffer( GL_ARRAY_BUFFER, pbo );
	gl::buffer_data( GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts, GL_STATIC_DRAW );
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
	auto& program = *test_program;
	gl::use_program( handle(program) );
	program[screen_location] = vec2{ float(hx), float(hy) };
	gl::use_program( 0 );
}

void clear()
{
	gl::clear_color( 0.0f, 0.0f, 0.0f, 0.0f );
	gl::clear( GL_COLOR_BUFFER_BIT );
}

void calc_positions()
{
	//static std::vector<float> vec;
	//vec.assign(std::begin(cube), std::end(cube));

	// gl::bind_buffer( GL_ARRAY_BUFFER, pbo );
	// gl::buffer_sub_data( GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vec.data() );
}

void render()
{
	using namespace std::chrono;
	static duration<double> period{1};
	static auto begin = steady_clock::now();
	auto now = steady_clock::now();
	duration<double> elapsed = now - begin;

	clear();

	auto& program = *test_program;
	gl::use_program( handle(program) );
	program[period_location] = period.count();
	program[time_location]   = elapsed.count();
	vec2 cam {
		(2.0f*mx) / hx - 1,
		(2.0f*my) / hy - 1
	};

	program[campos_location] = cam;

	//calc_positions();
	gl::bind_buffer( GL_ARRAY_BUFFER, pbo );
	gl::enable_vertex_attrib_array( 0 );
	gl::enable_vertex_attrib_array( 1 );
	gl::vertex_attrib_pointer( 0, 4, GL_FLOAT, false, 0, 0 );
	gl::vertex_attrib_pointer( 1, 4, GL_FLOAT, false, 0, sizeof(cube_verts)/2 );

	gl::draw_arrays( GL_TRIANGLES, 0, 36 );

	gl::disable_vertex_attrib_array( 0 );
	gl::use_program( 0 );
}

} // namespace aw::gl3

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

	gl::enable(GL_CULL_FACE);
	gl::cull_face(GL_BACK);
	gl::front_face(GL_CW);

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
