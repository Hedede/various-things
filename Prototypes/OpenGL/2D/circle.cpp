#include <aw/graphics/gl/awgl/api.h>
#include <aw/graphics/gl/program.h>
#include <aw/math/matrix4.h>
#include "circle.h"

namespace graphics {
using namespace aw;
namespace gl = aw::gl;
circle::circle(float radius)
        : radius{ radius }
{
	gl::gen_vertex_arrays(1, &vao);
	gl::gen_buffers(1, &vbo);

	gl::bind_buffer(GL_ARRAY_BUFFER, vbo);
	const auto r = radius;
	float vertices[] = {
	        r,  r, 0.f, 1.f,
	        -r,  r, 0.f, 1.f,
	        -r, -r, 0.f, 1.f,
	        r, -r, 0.f, 1.f,
	        0.f, 0.f, 0.f, 1.f,
	        0.f, 0.f, 0.f, 1.f,
	        0.f, 0.f, 0.f, 1.f,
	        0.f, 0.f, 0.f, 1.f
	};
	gl::buffer_data(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	gl::bind_vertex_array(vao);
	gl::enable_vertex_attrib_array(0);
	gl::vertex_attrib_pointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	gl::enable_vertex_attrib_array(1);
	gl::vertex_attrib_pointer(1, 4, GL_FLOAT, GL_FALSE, 0, 64);

	gl::bind_buffer(GL_ARRAY_BUFFER, 0);
}

circle::circle(circle&& other)
        : position{ other.position },
          rotation{ other.rotation },
          radius{ other.radius }
{
	vbo = other.vbo;
	vao = other.vao;
	other.vao = 0;
	other.vbo = 0;
}

circle& circle::operator=(circle&& other)
{
	cleanup();
	vbo = other.vbo;
	vao = other.vao;
	other.vao = 0;
	other.vbo = 0;
	return *this;
}

void circle::update_radius(float r)
{
	radius = r;

	float vertices[] = {
	        r,  r, 0.f, 1.f,
	        -r,  r, 0.f, 1.f,
	        -r, -r, 0.f, 1.f,
	        r, -r, 0.f, 1.f,
	};

	gl::bind_buffer(GL_ARRAY_BUFFER, vbo);
	gl::buffer_sub_data(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	gl::bind_buffer(GL_ARRAY_BUFFER, 0);
}

void circle::set_colour(float (&colours)[16])
{
	gl::bind_buffer(GL_ARRAY_BUFFER, vbo);
	gl::buffer_sub_data(GL_ARRAY_BUFFER, sizeof(colours), sizeof(colours), colours);
	gl::bind_buffer(GL_ARRAY_BUFFER, 0);
}

aw::gl3::mat4 circle::transform() const
{
	auto T = math::identity_matrix<float,4>;
	T.get(0,3) = position.x();
	T.get(1,3) = position.y();
	auto R = math::identity_matrix<float,4>;
	R = math::roll_matrix( rotation );
	return T * R;
}

void circle::render( gl3::program& program, gl3::mat4 transform )
{
	program["transform"] = transform;
	program["radius"]    = radius;

	gl::bind_vertex_array(vao);
	gl::draw_arrays(GL_TRIANGLE_FAN, 0, 4);
	gl::bind_vertex_array(0);
}

void circle::cleanup()
{
	if (vao != 0)
		gl::delete_vertex_arrays(1,&vao);
	if (vbo != 0)
		gl::delete_buffers(1,&vbo);

}

aw::gl3::mat4 orbiter::apply(aw::gl3::mat4 T1)
{
	auto O = math::identity_matrix<float,4>;
	O = math::roll_matrix( math::degrees<float>{ angle } );
	auto Oi = math::transpose( O );

	auto x = circles[center].position.x();
	auto y = circles[center].position.y();
	auto T2 = math::identity_matrix<float,4>;
	T2.get(0,3) = x;
	T2.get(1,3) = y;
	auto T2i = math::identity_matrix<float,4>;
	T2i.get(0,3) = -x;
	T2i.get(1,3) = -y;
	return  T2 * O * T2i * T1 * Oi;
}

} // namespace graphics
