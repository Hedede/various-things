#include <iostream>
#include <vector>
#include <string_view>
#include <chrono>
#include <aw/graphics/gl/api/gl_33.h>
#include <aw/graphics/gl/awgl/shader_func.h>
#include <aw/graphics/gl/utility/program_loader.h>
#include <aw/graphics/gl/program.h>
#include <aw/math/matrix4.h>
#include <QFile>
#include <circle.h>
namespace graphics {
using namespace std::string_view_literals;

float vertices[] = {
        0.1f, 0.1f, 0.0f, 1.0f,
        -0.1f, 0.1f, 0.0f, 1.0f,
        -0.1f, -0.1f, 0.0f, 1.0f,
        0.1f, -0.1f, 0.0f, 1.0f,
};

float vcolors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
};

using namespace aw;
namespace gl = ::gl;
void load()
{
	auto result = ::gl::sys::load_functions_3_3();
	std::cout << "GL loaded, missing: " << result.num_missing() << '\n';
}

std::optional<gl3::program> program;
GLuint vbo;
GLuint color_buffer;
GLuint vao;
aw::math::matrix4<float> view_matrix;
void update_view_matrix()
{
	std::cout << "view_matrix!\n";
	static auto loc_view_matrix = program->uniform( "view_matrix" );
	aw::gl::use_program(aw::gl::program_handle(*program));
	(*program)[loc_view_matrix] = view_matrix;
	std::cout << view_matrix.get(0,0) << ' ' << gl::get_error() << '\n';
	aw::gl::use_program(aw::gl::no_program);
}


void set_circle(aw::gl3::vec2 pos, float radius)
{
	vertices[0+0] = + radius;
	vertices[0+1] = + radius;
	vertices[4+0] = - radius;
	vertices[4+1] = + radius;
	vertices[8+0] = - radius;
	vertices[8+1] = - radius;

	vertices[12+0] = + radius;
	vertices[12+1] = - radius;

	gl::bind_buffer(GL_ARRAY_BUFFER, vbo);
	gl::buffer_data(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	gl::bind_buffer(GL_ARRAY_BUFFER, 0);

	aw::gl::use_program(aw::gl::program_handle(*program));
	(*program)["center"] = pos;
	(*program)["radius"] = radius;
}

std::vector<circle> circles;
std::vector<orbiter> orbiters;

void initialize()
{
	//----------------------------------------
	gl::clear_color(1.0f, 1.0f, 1.0f, 1.0f);
	//gl::enable(GL_CULL_FACE);
	gl::gen_buffers(1, &vbo);
	gl::gen_buffers(1, &color_buffer);
	gl::gen_vertex_arrays(1, &vao);
	gl::bind_vertex_array(vao);
	gl::bind_buffer(GL_ARRAY_BUFFER, vbo);
	gl::enable_vertex_attrib_array(0);
	gl::vertex_attrib_pointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	gl::bind_buffer(GL_ARRAY_BUFFER, color_buffer);
	gl::enable_vertex_attrib_array(1);
	gl::vertex_attrib_pointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	gl::buffer_data(GL_ARRAY_BUFFER, sizeof(vcolors), vcolors, GL_STATIC_DRAW);
	gl::bind_buffer(GL_ARRAY_BUFFER, 0);
	gl::bind_vertex_array(0);


	circle c1( 0.1 );
	float colours1[] = {
	        1.0f, 0.0f, 0.0f, 1.0f,
	        0.0f, 1.0f, 0.0f, 1.0f,
	        0.0f, 0.0f, 1.0f, 1.0f,
	        1.0f, 0.0f, 1.0f, 1.0f,
	};
	c1.set_colour(colours1);
	circles.push_back( std::move(c1) );

	circle c2( 0.1 );
	float colours2[] = {
	        0.0f, 0.0f, 0.0f, 1.0f,
	        0.0f, 0.0f, 0.0f, 1.0f,
	        0.0f, 0.0f, 0.0f, 1.0f,
	        1.0f, 1.0f, 1.0f, 1.0f,
	};
	c2.set_colour(colours2);
	circles.push_back( std::move(c2) );

	orbiter orb{ 1, 0 };
	orbiters.push_back(orb);

	view_matrix = aw::math::identity_matrix<float,4>;
	program = gl3::load_program( "test1.vsh", "test1.fsh" );
	set_circle( {0.0f,0.0f}, 0.1f );
	aw::gl::use_program(aw::gl::no_program);
	update_view_matrix();
	std::cout << "init!\n";
}
void clear()
{
}

float c1x = 0;
float c1y = 0;
float c1r = 0;
float c1o = 0;
float c2x = 0;
float c2y = 0;
float c2r = 0;
void render()
{
	gl::clear(GL_COLOR_BUFFER_BIT);
	gl::enable(GL_MULTISAMPLE);
	gl::enable(GL_BLEND);
	gl::blend_func( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;
	using namespace std::chrono;
	using namespace std::chrono_literals;
	static auto start1 = steady_clock::now();
	static auto start2 = steady_clock::now();
	auto cur = steady_clock::now();
	auto progress1 = (cur - start1) / 10.0s;
	auto progress2 = (cur - start2) / 2.0s;
	if (progress1 >= 1.0) start1 = cur;
	if (progress2 >= 1.0) start2 = cur;

	circles[0].position = gl3::vec2{ c1x, c1y };
	circles[0].rotation = math::degrees<float>{ c1r };
	circles[1].position = gl3::vec2{ c2x, c2y };
	circles[1].rotation = math::degrees<float>{ c2r };
	orbiters[0].angle = c1o;

	aw::gl::use_program(aw::gl::program_handle(*program));
	for (size_t i = 0; i < circles.size(); ++i) {
		auto& circle = circles[i];
		auto trafo = circle.transform();
		if (i < orbiters.size()) {
			trafo = orbiters[i].apply(trafo);
		}
		circle.render( *program, trafo );
	}
	aw::gl::use_program(aw::gl::no_program);
}

void reshape(int w, int h)
{
	auto ar = float(h) / float(w);
	view_matrix.get(0,0) = ar;
	update_view_matrix();
}
} // namespace graphics
