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
#include <aw/graphics/gl/model.h>

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
uniform_location perspective_location;
uniform_location transform_location;
uniform_location screen_location;
uniform_location time_location;
uniform_location period_location;
uniform_location campos_location;

camera cam;

void initialize_program()
{
	std::vector<shader> shaderList;

	auto vsh = load_shader( gl::shader_type::vertex,   "vert6.glsl" );
	auto fsh = load_shader( gl::shader_type::fragment, "frag1.glsl" );

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
	perspective_location = program.uniform("perspective");
	transform_location   = program.uniform("transform");

	assert(transform_location != gl3::invalid_uniform);

	gl::use_program( handle(program) );

	cam.set_near_z(1.0f);
	cam.set_far_z(10000.0f);

	cam.set_aspect_ratio(1.0f);
	cam.set_fov( degrees<float>{90} );

	program["perspective"] = cam.projection_matrix();

	gl::use_program( 0 );
}

struct {
	optional<gl3::model> model;

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

		vert_data vd{ verts, 0, color_offset };
		mesh_data md{ indices };

		model = std::move(gl3::model( vd, md ));
	}
} butruck;


void initialize_scene()
{
	butruck.load();

	gl::enable(GL_CULL_FACE);
	gl::cull_face(GL_BACK);
	gl::front_face(GL_CCW);

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
	cam.set_aspect_ratio( float(x) / float(y) );
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

float xx,yy,zz;

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
	program[screen_location] = vec2{ float(hx), float(hy) };

	program[perspective_location] = cam.projection_matrix();
	program[period_location] = period.count();
	program[time_location]   = elapsed.count();


	float horiz = ((mx - (hx/2.0f)) / hx) * 2.0f;
	float vert  = ((my - (hy/2.0f)) / hx) * 2.0f;
	auto pitch = math::pitch_matrix( degrees<float>(90) * vert );
	auto yaw   = math::yaw_matrix( degrees<float>(180)  * horiz );

	mat4 cam = math::identity_matrix<float,4>;
	cam.get(2,3) = zz;
	cam.get(1,3) = yy;
	cam.get(0,3) = xx;

	mat4 rot = math::identity_matrix<float,4>;;
	rot = pitch * yaw;

	program[campos_location] = cam * rot;

	gl::bind_vertex_array(butruck.model->vao);

	auto offset = math::identity_matrix<float,4>;
	offset = math::yaw_matrix( degrees<float>( 180.0f ) );


	//program[transform_location] = offset;
//*/
	for (auto obj : butruck.model->objects)
		gl::draw_elements_base_vertex(GL_TRIANGLES, obj.num_elements, GL_UNSIGNED_SHORT, 0, obj.offset);

/*/	auto ix = 0;
	auto iy = 0;
	auto iz = 0;
	for (auto ix = -5; ix < 10; ix+=5)
	for (auto iy = -5; iy < 10; iy+=5)
	for (auto iz = 0; iz<100;++iz)
	{
		offset.get(2,3) = 5*iz;
		offset.get(1,3) = iy;
		offset.get(0,3) = ix;
		program[transform_location] = offset;
		for (auto obj : butruck.model->objects)
			gl::draw_elements_base_vertex(GL_TRIANGLES, obj.num_elements, GL_UNSIGNED_SHORT, 0, obj.offset);
	}/**/

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

	size_t ctr  = 0;
	size_t prev = 0;
	using namespace std::chrono;
	auto start = steady_clock::now();
	auto point = start;
	float delto = 0.5;
	int min = std::numeric_limits<int>::max();
	int max = 0;
	int avg = 0;
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
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::S)
					zz -= delto;
				if (event.key.code == sf::Keyboard::W)
					zz += delto;
				if (event.key.code == sf::Keyboard::Q)
					yy -= delto;
				if (event.key.code == sf::Keyboard::Z)
					yy += delto;
				if (event.key.code == sf::Keyboard::D)
					xx -= delto;
				if (event.key.code == sf::Keyboard::A)
					xx += delto;
				if (event.key.code == sf::Keyboard::R)
					delto += 0.5;
				if (event.key.code == sf::Keyboard::T)
					delto -= 0.5;
				std::cout << xx << ' ' << yy << ' ' << zz << ' ' << delto << '\n';
			}
		}
		++ctr;
		auto now = steady_clock::now();
		if ((now - point) >= seconds{1}) {
			duration<double> immed = now - point;
			duration<double> total = now - start;
			point = now;

			avg = int(ctr / total.count());
			int cur = int((ctr - prev) / immed.count());
			if (cur < min)
				min = cur;
			if (cur > max)
				max = cur;
			window.setTitle( "FPS: " + to_string(avg) + "avg " + to_string(cur) + "cur " + to_string(max) + "max " + to_string(min) + "min");
			prev  = ctr;
		}

		render();

		window.display();
	}

	std::cout << "FPS (min/max/avg): " << min << '/' << max << '/' << avg << '\n';
}
} // namespace aw

int main()
{
	return aw::main();
}
