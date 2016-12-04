#include <array>
std::array<std::array<int,800>,800> map;

#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
int main()
{
	sf::RenderWindow window{sf::VideoMode(800, 800), "Dist", sf::Style::Default};

	sf::Image background;
	background.create(800,800,sf::Color::Black);

	std::random_device rd;
	std::mt19937 gen{ rd() }; // not properly seeded, don't care
	std::normal_distribution<double> dist{ 0, 1 };

	sf::Texture texture;
	texture.loadFromImage(background);
	sf::Sprite sprite;
	sprite.setTexture(texture, true);

	int x = 400, y = 400, d = 40;

	sf::Color blip{50,50,50,255};
	auto shoot = [&] (auto&& f, int x, int y) {
		int d1 = std::round(d*dist(gen));
		int d2 = std::round(d*dist(gen));

		x += d1;
		y += d2;

		if (0 > x || x > 800) return;
		if (0 > y || y > 800) return;
		sf::Color pixel = background.getPixel(x,y);
		f(pixel);
		background.setPixel(x,y,pixel);
	};

	auto add = [&] (sf::Color& pixel) {
		pixel += blip;
	};

	bool pressed = false;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					x = event.mouseButton.x;
					y = event.mouseButton.y;
				}
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Num1) {
					++d;
					std::cout << "spread: " << d << '\n';
				}
				else if (event.key.code == sf::Keyboard::Num2) {
					--d;
					std::cout << "spread: " << d << '\n';
				}
				else if (event.key.code == sf::Keyboard::Space) {
					shoot(add, x, y);
				}
				else if (event.key.code == sf::Keyboard::Q) {
					sprite.setTextureRect({350,350,450,450});
					sprite.setScale({8,8});
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			shoot(add, x, y);

		//window.clear(sf::Color::Black);
		texture.loadFromImage(background);
		window.draw(sprite);
		window.display();
	}

	return 0;
}
