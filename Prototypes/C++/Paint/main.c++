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

	int x = 400, y = 400;

	sf::Color blip{250,250,250,255};
	auto shoot = [&] (auto&& f, int x, int y) {
		int d1 = std::round(4*dist(gen));
		int d2 = std::round(4*dist(gen));

		x += d1;
		y += d2;

		if (0 > x || x > 800) return;
		if (0 > y || y > 800) return;
		sf::Color pixel = background.getPixel(x,y);
		f(pixel);
		background.setPixel(x,y,pixel);
	};

	auto add = [&] (sf::Color& pixel) {
		pixel = sf::Color::White;
	};

	auto sub = [&] (sf::Color& pixel) {
		pixel = sf::Color::Black;
	};

	int oldx = sf::Mouse::getPosition(window).x;
	int oldy = sf::Mouse::getPosition(window).y;
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
			/*
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {*/
					/*
				}
			}*/
		}

		int x = sf::Mouse::getPosition(window).x;
		int y = sf::Mouse::getPosition(window).y;

		if (x!=oldx)std::cout << x << ' ' << oldx << '\n';
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		for (int i = 0; i < 100; ++i) {
			float xk = float(x - oldx)*i/999;
			float yk = float(y - oldy)*i/999;
			shoot(add, int(oldx+xk), int(oldy+yk));
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		for (int i = 0; i < 100; ++i) {
			float xk = float(x - oldx)*i/999;
			float yk = float(y - oldy)*i/999;
			shoot(sub, int(oldx+xk), int(oldy+yk));
		}

		oldx = x;
		oldy = y;

		//window.clear(sf::Color::Black);
		texture.loadFromImage(background);
		window.draw(sprite);
		window.display();
	}

	return 0;
}
