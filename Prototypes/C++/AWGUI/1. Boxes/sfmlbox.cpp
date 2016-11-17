#include <algorithm>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

struct Style {
	sf::Vector2f size;
	sf::Color color;

	unsigned border;
	sf::Color bcolor;
};

struct Container;

struct Box {
	Box(Style const& style)
		: style(&style)
	{

	}

	sf::Vector2f size();

	Style const* style;
	Container* parent;
};

void DrawBox(sf::RenderWindow& window, Box const& box)
{
	Style const& style = *box.style;

	sf::RectangleShape rect(style.size);

	//rect.setPosition(box.pos);
	rect.setFillColor(style.color);
	if (style.border) {
		rect.setOutlineThickness(style.border);
		rect.setOutlineColor(style.bcolor);
	}
	window.draw(rect);
}

struct Container {
	struct Alloc {
		Box box;
		sf::Vector2f size;
		sf::Vector2f pos;
	};
	std::vector<Alloc> boxes;
	mutable bool dirty = false;

	void addBox(Box box)
	{
		boxes.push_back({box, box.size()});
		box.parent = this;
		dirty = true;
	}

	Alloc* findElem(Box* box)
	{
		auto pos = std::find_if(begin(boxes), end(boxes),
		[box] (Alloc& el) {
			return &el.box == box;
		});

		if (pos == end(boxes))
			return nullptr;
		return std::addressof(*pos);
	}

	sf::Vector2f elemSize(Box* box)
	{
		auto pos = findElem(box);
		if (pos)
			return pos->size;
		return {};
	}

	sf::Vector2f elemPos(Box* box)
	{
		auto pos = findElem(box);
		if (pos)
			return pos->pos;
		return {};
	}
};

sf::Vector2f Box::size()
{
	if (parent)
		return parent->elemSize(this);
	return style->size;
}

//#include <GL/gl.h>
#include "gl_ext45.h"
#include <iostream>
int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.core = true;
	settings.majorVersion = 4;
	settings.minorVersion = 1;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Boxes", sf::Style::Default, settings);

	Style style {
		{200, 100},
		{100, 255, 200},
		5,
		{sf::Color::Black}
	};

	Box box(style);

	Container con;

	// printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
	
	auto ryr = aw::gl::sys::load_functions_4_5();
	std::cout << ryr.num_missing() << '\n';

	sf::Font font;
	font.loadFromFile("/usr/share/fonts/TTF/Ubuntu-B.ttf");

	sf::Text text;
	text.setFont(font);
	text.setString("0 FPS");
	text.setCharacterSize(24);
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	text.setColor(sf::Color::White);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);

		DrawBox(window, box);

		window.draw(text);
		window.display();
	}

	return 0;
}
