// Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.

#include "pch.h" 
#include "play.h"
#include "Client.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <list>
#include <string.h>

using namespace std;
using namespace sf;
bool bb = false; 
void wait();
void Setbb();

int main()
{
	int ban;
	game g;
	int win1 = 0, win2 = 0, resp;
	char prr[20];
	do {
		ban = 0;
		win1 = g.Inicio();
		if (win1 == 1) {
			win2 = g.Name(prr);
			if (win2 == 1)
				ban = g.single(prr);
		}
		if (win1 == 2) {
			win2 = g.Name(prr);

			if (win2 == 1) {
				bool quit = false;
				sf::Thread* thread = 0;
				ban = client(prr);
				if (ban == 0) {
					thread = new sf::Thread(&wait);
					thread->launch();
					resp = CountDown();
					if (resp == true) {
						Setbb();
						thread = new sf::Thread(&Client_Game);
						thread->launch();
						thread = new sf::Thread(&Client_DoStuff);
						thread->launch();
						while (!quit)
						{
							GetInput();
						}
					}
				}
			}
		}
	} while (ban==1);
	return 0;
	//GAME OVER REGRESAR UNA VARIABLE Y PONER UN CICLO, SI ESA VARIABLE ES TRUE REPITA
}

void wait() {
	std::string y;
	string x;
	int win = 0;
	Font font;
	if (!font.loadFromFile("arial.ttf")) cout << "can't load font" << endl;
	Font font1;
	if (!font1.loadFromFile("FrescoStamp.ttf"))	cout << "can't load font" << endl;
	Text text("", font1);
	text.setCharacterSize(23);
	text.setFillColor(Color::White);
	text.setPosition(129, 415);

	//SetName Window 
	sf::RenderWindow window_SetName(sf::VideoMode(512, 512), "TETRIS2", sf::Style::Default);
	sf::Texture SetNamebackground;
	sf::Sprite SetNamebackgroundImage;
	window_SetName.setVisible(false);

	//Play button
	sf::Texture playButton;
	sf::Sprite playButtonImage;
	if (!playButton.loadFromFile("images/play.png"))
		std::cout << "Can't find the image" << std::endl;
	playButtonImage.setPosition(189, 225);
	float playButtonWidth = playButtonImage.getLocalBounds().width;
	float playButtonHeight = playButtonImage.getLocalBounds().height;
	playButtonImage.setTexture(playButton);

	if (!SetNamebackground.loadFromFile("images/SetName.jpg"))
		std::cout << "Can't find the image" << std::endl;
	SetNamebackgroundImage.setTexture(SetNamebackground);

	window_SetName.setVisible(true);

	while (window_SetName.isOpen()) {
		sf::Event Event;
		while (window_SetName.pollEvent(Event)) {
			switch (Event.type) {

			case sf::Event::Closed:
				window_SetName.close();
				break;

			case sf::Event::MouseMoved: {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window_SetName);
				sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
				if (playButtonImage.getGlobalBounds().contains(mousePosF)) {
					playButtonImage.setColor(sf::Color(80, 120, 120));
				}
				else {
					playButtonImage.setColor(sf::Color(255, 255, 255));
				}
			}
										break;
			}
		}
		window_SetName.clear();
		window_SetName.draw(SetNamebackgroundImage);
		window_SetName.draw(playButtonImage);
		window_SetName.display();
		if (bb) {
			window_SetName.close();
			bb = false;
			//return win = 0;
		}
	}
}

void Setbb() {
	bb = true;
}