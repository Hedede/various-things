#include <SFML/Graphics.hpp>

struct Style {
	sf::Vector2f size;
	sf::Color color;

	unsigned border;
	sf::Color bcolor;
};

struct Box {
	Box(Style const& style)
		: style(&style)
	{

	}

	sf::Vector2f pos;
	Style const* style;
};

void DrawBox(sf::RenderWindow& window, Box const& box)
{
	Style const& style = *box.style;

	sf::RectangleShape rect(style.size);

	rect.setPosition(box.pos);
	rect.setFillColor(style.color);
	if (style.border) {
		rect.setOutlineThickness(style.border);
		rect.setOutlineColor(style.bcolor);
	}
	window.draw(rect);
}

struct Container {
	std::vector<Box> boxes;

	void addBox(Box box)
	{
		boxes.push_back(box);
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Boxes");

	Style style {
		{200, 100},
		{100, 255, 200},
		5,
		{sf::Color::Black}
	};

	Box box(style);
	box.pos = sf::Vector2f{100,100};


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);

		DrawBox(window, box);

		window.display();
	}

	return 0;
}
