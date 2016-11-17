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
#include <aw/graphics/gl/camera.h>
#include <aw/fileformat/obj/loader.h>
#include <aw/io/input_file_stream.h>

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

camera cam;

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

	cam.set_near_z(1.0f);
	cam.set_far_z(10.0f);

	cam.set_aspect_ratio(1.0f);
	cam.set_fov( degrees<float>{90} );

	program["perspective"] = cam.projection_matrix();

	gl::use_program( 0 );
}

struct model_data_contiguous {
	std::vector< float > data;
	size_t vertex_offset;
	size_t normal_offset;
	size_t color_offset;
	size_t texcoord_offset;
};

struct model_data_interleaved {
	struct vertex {
		float position[3];
		float normal[3];
		float color[4];
		float texcoord[3];
	};
	std::vector< vertex > data;
};

struct {
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	size_t num_elements;

	void load()
	{
		io::input_file_stream file{ "butruck.obj" };
		auto data = obj::mesh::parse( file );

		std::vector< float > verts;
		std::vector< u16 > indices;

		for (auto v : data.verts) {
			verts.push_back( v[0] );
			verts.push_back( v[1] );
			verts.push_back( v[2] );
		}

		size_t color_offset = verts.size()*sizeof(float);

		for (auto v : data.verts) {
			verts.push_back( 0.5 );
			verts.push_back( 0.5 );
			verts.push_back( 0.5 );
			verts.push_back( 1.0 );
		}

		for (auto t : data.faces) {
			indices.push_back( t.verts[0].index );
			indices.push_back( t.verts[1].index );
			indices.push_back( t.verts[2].index );
		}

		num_elements = indices.size();

		gl::gen_buffers( 1, &vbo );
		gl::bind_buffer( GL_ARRAY_BUFFER, vbo );
		gl::buffer_data( GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW );
		gl::bind_buffer( GL_ARRAY_BUFFER, 0 );

		gl::gen_buffers( 1, &ibo );
		gl::bind_buffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
		gl::buffer_data( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u16), indices.data(), GL_STATIC_DRAW );
		gl::bind_buffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

		gl::gen_vertex_arrays(1, &vao);
		gl::bind_vertex_array(vao);

		gl::bind_buffer( GL_ARRAY_BUFFER, vbo );
		gl::enable_vertex_attrib_array( 0 );
		gl::enable_vertex_attrib_array( 1 );
		gl::vertex_attrib_pointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		gl::vertex_attrib_pointer( 1, 4, GL_FLOAT, GL_FALSE, 0, color_offset );
		gl::bind_buffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
		gl::bind_vertex_array( 0 );
	}
} butruck;

void initialize_scene()
{
	butruck.load();

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
	cam.set_aspect_ratio( float(x) / float(y) );
	program["perspective"] = cam.projection_matrix();
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


	auto offset = math::identity_matrix<float,4>;

	gl::bind_vertex_array(butruck.vao);
	offset = math::yaw_matrix( degrees<float>( 180.0f ) );
	offset.get(2,3) = -5;
	offset.get(1,3) = -2;
	program["transform"] = offset;

	gl::draw_elements(GL_TRIANGLES, butruck.num_elements, GL_UNSIGNED_SHORT, 0);

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

	GLint num;
	gl::get_integerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &num);
	std::cout << "maxvert: " << num << '\n';

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
