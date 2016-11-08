#include <aw/graphics/gl/gl_ext33.h>
#include <aw/types/string_view.h>
#include <aw/utility/string/split.h>
#include <aw/math/matrix3.h>
#include <aw/math/matrix4.h>
#include <aw/math/math.h>
#include <aw/math/angle.h>

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

float cot(float x)
{
	return cos(x) / sin(x);
}

float calc_fov(math::radians<float> fov)
{
	return cot(fov.count() / 2.0f);
}

const float frustum_scale = calc_fov( math::degrees<float>{90} );

mat4 projmat;
void initialize_program()
{
	std::vector<shader> shaderList;

	auto vsh = load_shader( gl::shader_type::vertex,   "vert4.glsl" );
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

	float zFar = 3.0f;
	float zNear = 1.0f;

	float s = frustum_scale;
	float z = (zFar + zNear) / (zNear - zFar);
	float w = 2 * zFar * zNear / (zNear - zFar);
	projmat = {
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

GLuint vao1;
GLuint vao2;

GLuint vbo;
GLuint ibo;


void initialize_scene()
{
	gl::gen_buffers( 1, &vbo );
	gl::bind_buffer( GL_ARRAY_BUFFER, vbo );
	gl::buffer_data( GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW );
	gl::bind_buffer( GL_ARRAY_BUFFER, 0 );

	gl::gen_buffers( 1, &ibo );
	gl::bind_buffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	gl::buffer_data( GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW );
	gl::bind_buffer( GL_ELEMENT_ARRAY_BUFFER, 0 );


	gl::gen_vertex_arrays(1, &vao1);
	gl::bind_vertex_array(vao1);

	size_t colorDataOffset = sizeof(float) * 3 * numberOfVertices;

	gl::bind_buffer( GL_ARRAY_BUFFER, vbo );
	gl::enable_vertex_attrib_array( 0 );
	gl::enable_vertex_attrib_array( 1 );
	gl::vertex_attrib_pointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	gl::vertex_attrib_pointer( 1, 4, GL_FLOAT, GL_FALSE, 0, colorDataOffset );
	gl::bind_buffer( GL_ELEMENT_ARRAY_BUFFER, ibo );

	gl::bind_vertex_array( 0 );

	gl::enable(GL_CULL_FACE);
	gl::cull_face(GL_BACK);
	gl::front_face(GL_CW);

	gl::enable(GL_DEPTH_TEST);
	gl::depth_mask(GL_TRUE);
	gl::depth_func(GL_LEQUAL);
	gl::depth_range(0.0f, 1.0f);
}

int mx, my;
int hx, hy;


void reshape(int x, int y)
{
	gl::viewport(0, 0, x, y);

	hx = x;
	hy = y;

	auto& program = *test_program;

	gl::use_program( handle(program) );
	program[screen_location] = vec2{ float(x), float(y) };
	projmat.get(0,0) = frustum_scale / (float(x) / y);
	projmat.get(1,1) = frustum_scale;
	program["perspective"] = projmat;
	gl::use_program( 0 );
}

void clear()
{
	gl::clear_color( 0.0f, 0.0f, 0.0f, 0.0f );
	gl::clear_depth( 1.0f );
	gl::clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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


	auto offset = math::make_identity<float,4>();

	gl::bind_vertex_array(vao1);
	program["transform"] = offset;
	gl::draw_elements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);

	offset.get(2,3) = -0.1;
	program["transform"] = offset;
	gl::draw_elements_base_vertex(GL_TRIANGLES, ARRAY_COUNT(indexData),
		GL_UNSIGNED_SHORT, 0, numberOfVertices / 2);

	offset.get(2,3) = -0.1;
	offset = math::roll_matrix( 45.0f );
	program["transform"] = offset;
	gl::draw_elements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);

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
	reshape(800, 600);

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
