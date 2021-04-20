#include "pch.h"
#include "Graph.h"
#include "Client.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <sstream>
#include <string.h>


using namespace sf;
using namespace std;

//struct Point {int x, y;} a[4], b[4], c[4];
//const int M = 20;
//const int N = 10;
//int field[M][N] = { 0 };
/*int figures[7][4] =
{
	1,3,5,7, // I
	0,2,3,5, // Z
	1,3,2,4, // S
	1,3,2,5, // T
	0,1,3,5, // L
	1,3,5,4, // J
	2,3,4,5, // O
};*/

Graph::Graph() {
	bb = false;
}

Graph::~Graph() {}

int Graph::Inicio() {
	int nam = 0;
	sf::RenderWindow menuwindow(sf::VideoMode(512, 512), "TETRIS", sf::Style::Default);
	sf::Texture Menubackground;
	sf::Sprite MenubackgroundImage;

	if (!Menubackground.loadFromFile("images/menu.png"))
		std::cout << "Can't find the image" << std::endl;
	MenubackgroundImage.setTexture(Menubackground);

	//1 Player button
	sf::Texture oneplayerButton;
	sf::Sprite oneplayerButtonImage;
	if (!oneplayerButton.loadFromFile("images/1 player.png"))
		std::cout << "Can't find the image" << std::endl;
	oneplayerButtonImage.setPosition(189, 225);
	float oneplayerButtonWidth = oneplayerButtonImage.getLocalBounds().width;
	float oneplayerButtonHeight = oneplayerButtonImage.getLocalBounds().height;
	oneplayerButtonImage.setTexture(oneplayerButton);

	//Multiplayer button
	sf::Texture multiplayerButton;
	sf::Sprite multiplayerButtonImage;
	if (!multiplayerButton.loadFromFile("images/multiplayer.png"))
		std::cout << "Can't find the image" << std::endl;
	multiplayerButtonImage.setPosition(189, 270);
	float multiplayerButtonWidth = multiplayerButtonImage.getLocalBounds().width;
	float multiplayerButtonHeight = multiplayerButtonImage.getLocalBounds().height;
	multiplayerButtonImage.setTexture(multiplayerButton);

	//Exit button
	sf::Texture exitButton;
	sf::Sprite exitButtonImage;
	if (!exitButton.loadFromFile("images/exit.png"))
		std::cout << "Can't find the image" << std::endl;
	exitButtonImage.setPosition(189, 315);
	float exitButtonWidth = exitButtonImage.getLocalBounds().width;
	float exitButtonHeight = exitButtonImage.getLocalBounds().height;
	exitButtonImage.setTexture(exitButton);

	while (menuwindow.isOpen())
	{
		sf::Event Event;
		while (menuwindow.pollEvent(Event)) {
			switch (Event.type) {

			case sf::Event::Closed:
				menuwindow.close();
				break;

			case sf::Event::MouseMoved: {
				sf::Vector2i mousePos = sf::Mouse::getPosition(menuwindow);
				sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
				if (exitButtonImage.getGlobalBounds().contains(mousePosF)) {
					exitButtonImage.setColor(sf::Color(200, 200, 200));
				}
				else if (oneplayerButtonImage.getGlobalBounds().contains(mousePosF)) {
					oneplayerButtonImage.setColor(sf::Color(80, 120, 120));
				}
				else if (multiplayerButtonImage.getGlobalBounds().contains(mousePosF)) {
					multiplayerButtonImage.setColor(sf::Color(80, 120, 120));
				}
				else {
					exitButtonImage.setColor(sf::Color(255, 255, 255));
					oneplayerButtonImage.setColor(sf::Color(255, 255, 255));
					multiplayerButtonImage.setColor(sf::Color(255, 255, 255));
				}
			}
			break;

			case sf::Event::MouseButtonPressed: {
				sf::Vector2i mousePos = sf::Mouse::getPosition(menuwindow);
				sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
				if (exitButtonImage.getGlobalBounds().contains(mousePosF))
					menuwindow.close();
				if (oneplayerButtonImage.getGlobalBounds().contains(mousePosF)) {
					menuwindow.close();
					return nam = 1;
				}
				if (multiplayerButtonImage.getGlobalBounds().contains(mousePosF)) {
					menuwindow.close();
					return nam = 2;
				}
			}
			break;
			}
		}

		menuwindow.clear();
		menuwindow.draw(MenubackgroundImage);
		menuwindow.draw(exitButtonImage);
		menuwindow.draw(oneplayerButtonImage);
		menuwindow.draw(multiplayerButtonImage);
		menuwindow.display();
	}
}

int Graph::Name(char pr[]) {
	std::string y;
	string x;
	int win = 0;
	Font font2;
	if (!font2.loadFromFile("emmasophia.ttf")) cout << "can't load font" << endl;
	Text text("", font2);
	text.setCharacterSize(23);
	text.setFillColor(Color::White);
	text.setPosition(129, 415);

	//SetName Window 
	sf::RenderWindow window_SetName(sf::VideoMode(512, 512), "TETRIS1", sf::Style::Default);
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

	//InputBox
	Texture t1;
	t1.loadFromFile("images/boxName.png");

	Sprite boxname(t1);
	boxname.setPosition(164, 163);

	sf::RectangleShape box;
	sf::Text label;
	box.setSize(sf::Vector2f(123, 25));
	box.setPosition(189, 180);
	box.setFillColor(sf::Color::White);
	label.setString("Name");
	label.setFillColor(sf::Color::Black);
	label.setCharacterSize(13);
	label.setPosition(210, 180);
	label.setFont(font2);

	window_SetName.setVisible(true);

	while (window_SetName.isOpen()) {
		sf::Event Event;
		while (window_SetName.pollEvent(Event)) {
			switch (Event.type) {

			case sf::Event::TextEntered:
				{
					if (Event.text.unicode == '\b' && y.size() > 0)
						y.erase(y.size() - 1, 1);
					else if (Event.text.unicode < 128 && y.size() < 10)
						y += char(Event.text.unicode);
				}
				label.setString(y);
				strcpy_s(pr, 15, y.c_str());
				break;

			case (sf::Event::KeyPressed): {
				if (Keyboard::isKeyPressed(Keyboard::Return)) {
					window_SetName.close();
					return win = 1;
				}
			}
			break;

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

			case sf::Event::MouseButtonPressed: {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window_SetName);
				sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
				if (playButtonImage.getGlobalBounds().contains(mousePosF)) {
					window_SetName.close();
					return win = 1;
				}
			}
			break;
			}
		}
		window_SetName.clear();
		window_SetName.draw(SetNamebackgroundImage);
		window_SetName.draw(box);
		window_SetName.draw(boxname);
		window_SetName.draw(label);
		window_SetName.draw(playButtonImage);
		window_SetName.display();
	}
}


/*
int Graph::BoardGame(char na[])
{
	// Texto del puntaje
	Font font;
	//Load and check the availability of the font file
	if (!font.loadFromFile("arial.ttf"))
		cout << "can't load font" << endl;

	Font font1;
	if (!font1.loadFromFile("FrescoStamp.ttf"))
		cout << "can't load font" << endl;

	Font font2;
	if (!font2.loadFromFile("emmasophia.ttf"))
		cout << "can't load font" << endl;

	sf::Text label;
	label.setFillColor(sf::Color::White);
	label.setCharacterSize(25);
	label.setPosition(30, 410);
	label.setFont(font1);
	label.setString(na);

	sf::Text labelPlayer;
	labelPlayer.setString("Player");
	labelPlayer.setFillColor(sf::Color::White);
	labelPlayer.setCharacterSize(6);
	labelPlayer.setPosition(30, 410);
	labelPlayer.setFont(font2);

	sf::Text textScore;
	textScore.setString("Score");
	textScore.setFillColor(sf::Color::White);
	textScore.setCharacterSize(6);
	textScore.setPosition(120, 410);
	textScore.setFont(font2);


	sf::Text gameoverlabel;

	sf::String y;
	stringstream ss;
	string x;

	//Declare a Text object
	Text text("", font1);
	//Set character size
	text.setCharacterSize(23);
	//Set fill color
	text.setFillColor(Color::White);
	text.setPosition(129, 415);

	srand(time(0));

	RenderWindow window(VideoMode(320, 480), "TETRIS");		//crea la ventana de juego con un nombre en la ventana ancho x alto
	window.setVisible(true);

	Texture t1, t2, t3, t4, t5, t6, t7, t8;
	t1.loadFromFile("images/tiles1.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame3.png");
	t4.loadFromFile("images/Nextbox.png");
	t5.loadFromFile("images/Specialbox.png");
	t7.loadFromFile("images/holdbox.png");
	t8.loadFromFile("images/tilegameover.png");

	Sprite s(t1), background(t2), frame(t3), nextbox(t4), specialbox(t5), powerup, holdbox(t7), g(t8);

	nextbox.setPosition(233, 22);
	specialbox.setPosition(233, 342);
	holdbox.setPosition(233, 180);

}
*/