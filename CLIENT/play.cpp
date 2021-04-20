
#include "pch.h"
#include "play.h"
#include "Client.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <sstream>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

struct Point
{int x, y;} a[4], b[4], c[4], h[4], aux[4];

int figures[7][4] =
{
	1,3,5,7, // I
	0,2,3,5, // Z
	1,3,2,4, // S
	1,3,2,5, // T
	0,1,3,5, // L
	1,3,5,4, // J
	0,1,2,3, // O
};

std::string powerups[] = {"speedup", "slowtime", "direction", "minus2" };

//FUNCIONES DE LA CLASE GAME (DEL JUEGO)

//funcion que se modifica cada vez que haya un poder con el número de poder 
int game::getpower() {
	int p;
	p = power;
	power = 100;
	return p;
}

bool game::getstr() {
	bool st;
	st = strrr;
	strrr = false;
	return st;
}


bool game::check()		// a es el estado actual 
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) {
			return 0;
		}
		else if (field[a[i].y][a[i].x]) {
			return 0;
		}
	return 1;
};
//Funcion Movimiento
void game::mov(int dx1)
{
	for (int i = 0; i < 4; i++)
	{
		b[i] = a[i];
		a[i].x += dx1;
	}
	if (!check())
		for (int i = 0; i < 4; i++)
			a[i] = b[i];
}
//Funcion Rotacion 
void game::rot()
{
	Point p = a[1];					//Centro de rotacion
	for (int i = 0; i < 4; i++)
	{
		int x = a[i].y - p.y;
		int y = a[i].x - p.x;
		a[i].x = p.x - x;
		a[i].y = p.y + y;
	}
	if (!check())
		for (int i = 0; i < 4; i++)
			a[i] = b[i];
}
//Funcion de mov automático de la pieza 
void game::tick(float *timer1, int *colorNum1, int *colorNum22, bool *hold1)
{
	for (int i = 0; i < 4; i++)
	{
		b[i] = a[i];
		a[i].y += 1;						//movimiento automático 
	}

	if (!check())  //!check, si es igual a cero; no se puede mover más
	{
		*hold1 = false;
		strrr = true;
		for (int i = 0; i < 4; i++)
			field[b[i].y][b[i].x] = *colorNum1;

		for (int i = 0; i < 4; i++)
		{
			*colorNum1 = *colorNum22;
			//Posición inicial de la nueva ficha
			a[i].x = c[i].x + 4;  //Residuo de la division entera
			a[i].y = c[i].y;  //Parte entera de la divisi�n
		}
		n = n2;
		//Ficha siguiente inicial
		*colorNum22 = 1 + rand() % 7;  //random entre 1 y 7 para el color
		n2 = rand() % 7;       // random para la ficha
		for (int i = 0; i < 4; i++)
		{
			//Posición inicial de la nueva ficha
			c[i].x = figures[n2][i] % 2;  //Residuo de la division entera
			c[i].y = figures[n2][i] / 2;  //Parte entera de la división
		}
	}
	*timer1 = 0;
}
//Funcion para chequear si alguna linea está completa 
void game::checklines(int *ban, int k, int *axx)
{
	for (int i = M - 1; i > 0; i--)
	{
		int count = 0;
		for (int j = 0; j < N; j++)
		{
			if (field[i][j])						//si la posición i,j está llena cuenta uno
				count++;
			field[k][j] = field[i][j];				//llena la posición k,j con lo que hay en i,j (elimina si k no disminuye)
			for (int j = 0; j < N; j++)					//cierra el juego cuando se llena la pantalla
				if (field[1][j])
					*ban = 1;
		}
		if (count < N)								//si la linea no está llena k disminuye sino no disminuye
			k--;									//y se elimina la línea ya que no cambiará la posición de k pero si la de i
		if (count == N) {
			*axx = *axx + 1;
			point();								//Sumamos el puntaje
		}
	}
}
//juego completo usando las clases que están arriba
int game::single(char na[])
{
	// Music 
	sf::Music music;
	if (!music.openFromFile("TETRIS(TrapRemix).wav"))
		return -1; // error

	music.play();

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
	gameoverlabel.setString("Game Over");
	gameoverlabel.setFillColor(sf::Color::Yellow);
	gameoverlabel.setCharacterSize(13);
	gameoverlabel.setPosition(60, 120);
	gameoverlabel.setFont(font2);

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
	t1.loadFromFile("images/tiles.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame3.png");
	t4.loadFromFile("images/Nextbox.png");
	t7.loadFromFile("images/holdbox.png");
	t8.loadFromFile("images/tilegameover.png");

	Sprite s(t1), background(t2), frame(t3), nextbox(t4), powerup, holdbox(t7), g(t8);

	nextbox.setPosition(233, 22);
	holdbox.setPosition(233, 180);

	int dx = 0;
	int ban1 = 0, flag = 0, aux22 = 0;
	int n4, dir = 1, nivel = 1;
	int colorNum = 1, colorNum2, colorNum3, colorAux;
	int score = getscore();
	bool hold = false, pwup = false, gameover = false;
	float timer = 0, delay = 0.6, timer1 = 0, time1, del1 = 0.6;

	Clock clock;
	Clock clock1;

	//Ficha actual inicial
	colorNum = 1 + rand() % 7;  //random entre 1 y 7 para el color
	n = rand() % 7;       // random para la ficha
	for (int i = 0; i < 4; i++) {
		//Posición inicial de la nueva ficha
		a[i].x = (figures[n][i] % 2) + 4;  //Residuo de la division entera
		a[i].y = (figures[n][i] / 2);  //Parte entera de la divisi�n
	}

	//Ficha siguiente inicial
	colorNum2 = 1 + rand() % 7;  //random entre 1 y 7 para el color
	n2 = rand() % 7;       // random para la ficha
	for (int i = 0; i < 4; i++) {
		c[i].x = figures[n2][i] % 2;  //Residuo de la division entera
		c[i].y = figures[n2][i] / 2;  //Parte entera de la divisi�n
	}

	//Ficha hold inicial
	colorNum3 = 1 + rand() % 7;  //random entre 1 y 7 para el color
	n3 = rand() % 7;       // random para la ficha
	for (int i = 0; i < 4; i++)
	{
		//Posicion inicial de la nueva ficha
		h[i].x = figures[n3][i] % 2;  //Residuo de la division entera
		h[i].y = figures[n3][i] / 2;  //Parte entera de la divisi�n
	}

	//corre el programa siempre que la ventana esté abierta 

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		//Chequea todos los eventos de la ventana que fueron puestos después de la última iteración del ciclo
		while (window.pollEvent(e))							//pollEvent pone un evento de primero 
		{
			if (e.type == Event::Closed)					//en este caso si esto sucede se cierra la ventana
				window.close();

			if (e.type == Event::KeyPressed)				//si se presiona una tecla
				if (e.key.code == Keyboard::Up)	rot();	//arriba = rota
				else if (e.key.code == Keyboard::Left) dx = -1; //izquierda
				else if (e.key.code == Keyboard::Right) dx = 1; //derecha 
				else if (e.key.code == Keyboard::Down) delay = 0.05; //max rápido
		}

		// Movimiento 
		mov(dx);

		//PIEZA HOLD
		if (Keyboard::isKeyPressed(Keyboard::H) && hold == false)
		{
			hold = true;
			for (int i = 0; i < 4; i++) {
				aux[i] = a[i];
				a[i].x = (figures[n3][i] % 2) + 4;  //Residuo de la division entera
				a[i].y = figures[n3][i] / 2;
				h[i].x = figures[n][i] % 2;  //Residuo de la division entera
				h[i].y = figures[n][i] / 2;
			}
			colorAux = colorNum3;
			colorNum3 = colorNum;
			colorNum = colorAux;
			int naux = n;
			n = n3;
			n3 = naux;
		}

		//Tick
		if (timer > delay)
			tick(&timer, &colorNum, &colorNum2, &hold);

		//check lines
		int k = M - 1;
		checklines(&ban1, k, &aux22);
		score = getscore();

		//Puntaje
		std::string lText = "";
		//Convertimos el numero level en una cadena de texto
		std::stringstream ss;
		ss << score;
		lText += ss.str();

		text.setString(lText);

		
		dx = 0;
		delay = del1;
		////////////////////////////////////////PONER NIVELES/////////////////////////////////////////////
		// CADA 3 PUNTOS SE PONE MAS RÁPIDO 
		cout << "aux es" << aux22 << endl;
		if (aux22 >= 3 && delay > 0.01) {
			del1 = delay - 0.05;
			aux22 = 0;
			nivel++;
			cout << "aux22 es" << aux22 << endl;
		}

		/////////draw//////////
		window.clear();
		window.draw(frame);
		window.draw(nextbox);
		window.draw(holdbox);
		window.draw(label);

		if (ban1 == 1) {
			t1.loadFromFile("images/tiles1.png");
			Sprite s(t1);

			for (int i = 0; i < M; i++)
				for (int j = 0; j < N; j++)
				{
					if (field[i][j] == 0) continue;
					field[i][j] = 8;
				}
			colorNum = 8;
			gameover = true;
			//window.draw(gameoverlabel);
		}


		//Recorta la imagen de los cubos de colores para darle un color diferente a cada pieza
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0)
					continue;
				s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				s.setPosition(j * 18, i * 18);
				s.move(27, 31); //offset
				window.draw(s);
			}
		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			s.setPosition(a[i].x * 18, a[i].y * 18);
			s.move(27, 31); //offset
			window.draw(s);
		}
		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum3 * 18, 0, 18, 18)); // creamos el bloque de la pieza creada segun el color (X_ini,Y_ini,X_fin,Y_fin)
			if (n3 == 0) {
				s.setPosition((h[i].x + 12.5) * 18 - 10, (h[i].y) * 18 + 9 + 157);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
			else if (n3 == 6) {
				s.setPosition((h[i].x + 12.5) * 18 + 1, (h[i].y) * 18 + 23 + 157);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
			else {
				s.setPosition((h[i].x + 12.5) * 18, (h[i].y) * 18 + 15 + 157);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
		}
		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum2 * 18, 0, 18, 18)); // creamos el bloque de la pieza creada segun el color (X_ini,Y_ini,X_fin,Y_fin)
			if (n2 == 0) {
				s.setPosition((c[i].x + 12.5) * 18 - 10, (c[i].y) * 18 + 9);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
			else if (n2 == 6) {
				s.setPosition((c[i].x + 12.5) * 18 + 1, (c[i].y) * 18 + 23);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
			else {
				s.setPosition((c[i].x + 12.5) * 18, (c[i].y) * 18 + 15);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
		}
		window.draw(labelPlayer);
		window.draw(textScore);
		window.draw(text);
		if (gameover == true) window.draw(gameoverlabel);
			window.display();

		while (gameover == true) {
			music.stop();
			while (window.pollEvent(e)) {							//pollEvent pone un evento de primero 
				if (e.type == Event::Closed)					//en este caso si esto sucede se cierra la ventana
					window.close();
				if (e.type == Event::KeyPressed) {
					window.close();
					return 1;
				}
			}
		}
	}
	return 0;
}

int game::juego(char na[], int field2[][N])
{
	// Music 
	sf::Music music;
	if (!music.openFromFile("TETRIS(TrapRemix).wav"))
		return -1; // error

	music.play();

	int P1[M][N] = { 0 };
	int P2[M][N] = { 0 }; 
	int P3[M][N] = { 0 }; 
	int P4[M][N] = { 0 };
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

	int yo = whoami();
	stringstream numyo;
	numyo << yo;

	sf::Text labelPlayer;
	labelPlayer.setString("Player "+numyo.str());
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

	RenderWindow window(VideoMode(600, 480), "TETRIS");		//crea la ventana de juego con un nombre en la ventana ancho x alto
	window.setVisible(true);

	Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14;
	t1.loadFromFile("images/tiles1.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame3.png");
	t4.loadFromFile("images/Nextbox.png");
	t5.loadFromFile("images/Specialbox.png");
	t7.loadFromFile("images/holdbox.png");
	t8.loadFromFile("images/tilegameover.png");
	t9.loadFromFile("images/barra0.png");
	t10.loadFromFile("images/framePlayer1.png");
	t11.loadFromFile("images/framePlayer2.png");
	t12.loadFromFile("images/framePlayer3.png");
	t13.loadFromFile("images/framePlayer4.png");
	t14.loadFromFile("images/tiles2.png");

	Sprite s(t1), background(t2), frame(t3), nextbox(t4), specialbox(t5), powerup, holdbox(t7), g(t8), barra(t9),
		frP1(t10), frP2(t11), frP3(t12), frP4(t13), s1(t14);


	nextbox.setPosition(233, 22);
	specialbox.setPosition(233, 342);
	holdbox.setPosition(233, 180);
	barra.setPosition(20, 450);
	frP1.setPosition(350, 20);
	frP2.setPosition(465, 20);
	frP3.setPosition(350, 245);
	frP4.setPosition(465, 245);

	int dx = 0;
	int ban1 = 0, auxnouso = 0;
	int n4 = 100, n41 = 100, dir = 1, aleato = 10;
	int colorNum = 1, colorNum2, colorNum3, colorAux;
	int score = getscore();
	bool fin = false, variablefinal, flagg = false, flagclock = false;
	bool hold = false, pwup = false, gameover = false, fala = false;
	float timer = 0, delay = 0.6, time1 = 0;

	Clock clock;
	Clock clock1;

	//Ficha actual inicial
	colorNum = 1 + rand() % 7;  //random entre 1 y 7 para el color
	n = rand() % 7;       // random para la ficha
	for (int i = 0; i < 4; i++) {
		//Posición inicial de la nueva ficha
		a[i].x = (figures[n][i] % 2) + 4;  //Residuo de la division entera
		a[i].y = (figures[n][i] / 2);  //Parte entera de la divisi�n
	}

	//Ficha siguiente inicial
	colorNum2 = 1 + rand() % 7;  //random entre 1 y 7 para el color
	n2 = rand() % 7;       // random para la ficha
	for (int i = 0; i < 4; i++) {
		c[i].x = figures[n2][i] % 2;  //Residuo de la division entera
		c[i].y = figures[n2][i] / 2;  //Parte entera de la divisi�n
	}

	//Ficha hold inicial
	colorNum3 = 1 + rand() % 7;  //random entre 1 y 7 para el color
	n3 = rand() % 7;       // random para la ficha
	for (int i = 0; i < 4; i++)
	{
		//Posicion inicial de la nueva ficha
		h[i].x = figures[n3][i] % 2;  //Residuo de la division entera
		h[i].y = figures[n3][i] / 2;  //Parte entera de la divisi�n
	}

	//corre el programa siempre que la ventana esté abierta 

	while (window.isOpen())
	{
		for (int i2 = 0; i2 < M; i2++)
			for(int i3 = 0; i3 < N; i3++)
				field2[i2][i3] = field[i2][i3];

		getfields(P1, P2, P3, P4);

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		variablefinal = getvarfin();
		Event e;
		//Chequea todos los eventos de la ventana que fueron puestos después de la última iteración del ciclo
		while (window.pollEvent(e))							//pollEvent pone un evento de primero 
		{
			if (e.type == Event::Closed)					//en este caso si esto sucede se cierra la ventana
				window.close();

			if (e.type == Event::KeyPressed)				//si se presiona una tecla
				if (e.key.code == Keyboard::Up)	rot();	//arriba = rota
				else if (e.key.code == Keyboard::Left) dx = -dir; //izquierda
				else if (e.key.code == Keyboard::Right) dx = dir; //derecha 
				else if (e.key.code == Keyboard::Down) delay = 0.05; //max rápido
				else if ((e.key.code == Keyboard::S) && aleato != 10) {
					if (aleato == 1 || aleato == 3)
						power = aleato;
					else
						n41 = aleato;
					aleato = 10;
					tocero();
					flagg = false;
					score = getscore();
				}
			//PIEZA HOLD
				else if ((e.key.code == Keyboard::H) && hold == false)
				{
					hold = true;
					for (int i = 0; i < 4; i++) {
						aux[i] = a[i];
						a[i].x = (figures[n3][i] % 2) + 4;  //Residuo de la division entera
						a[i].y = figures[n3][i] / 2;
						h[i].x = figures[n][i] % 2;  //Residuo de la division entera
						h[i].y = figures[n][i] / 2;
					}
					colorAux = colorNum3;
					colorNum3 = colorNum;
					colorNum = colorAux;
					int naux = n;
					n = n3;
					n3 = naux;
				}
		}

		// Movimiento 
		mov(dx);

		//Tick
		if (timer > delay)
			tick(&timer, &colorNum, &colorNum2, &hold);

		//check lines
		int k = M - 1;
		checklines(&ban1, k, &auxnouso);
		score = getscore();
		if (score <= 5) {
			std::stringstream ss;
			ss << score * 20;
			t9.loadFromFile("images/barra" + ss.str() + ".png");
		}

		n4 = getpowerenemy();
		dx = 0;
		delay = 0.6;

		////////////////////////////////////////POWEEEEERS/////////////////////////////////////////////
		if (time1 == 0) {
			delay = 0.6;
			dir = 1;
			power = 100;
			pwup = false;
			flagclock = false;
		}
		else if (time1 > 5) {
			time1 = 0;
			power = 100;
			n4 = 100; 
			n41 = 100;
			aleato = 10;
			pwup = false;
			flagclock = false;
			setpower();
		}
		else {
			time1 = clock1.getElapsedTime().asSeconds();
			flagg = false;
			if (n4 == 1) {
				delay = 0.1;
			}
			else if (n4 == 3) {
				dir = -1;
			}
			else if (n41 == 2) {
				delay = 1;
			}
			else if (n41 == 4) {
				int k = M - 1;
				int count1 = 0;
				for (int i = M - 1; i > 0; i--)
				{
					int count = 0;
					for (int j = 0; j < N; j++)	{
						field[k][j] = field[i][j];
					}
					if (count1 < 2) {
						count = N;
						count1++;
					}
					if (count < N) 
						k--;
				}
				time1 = 6;
			}
		}


		/////////draw//////////
		window.clear();
		window.draw(frame);
		window.draw(nextbox);
		window.draw(specialbox);
		window.draw(holdbox);
		window.draw(label);
		window.draw(frP1);
		window.draw(frP2);
		window.draw(frP3);
		window.draw(frP4);

		/////////////////////////////////////////////////////////////////MULTIJUGADOR/////////////////////////////////////////////////////////////
		//ESTO SOLO DEBE SER EN MODO MULTIJUGADOR

		if (score >= 5 && flagg == false && pwup == false) {
			aleato = (rand() % 4) + 1;
			t6.loadFromFile("images/" + powerups[aleato-1] + ".png");
			flagg = true;
			pwup = true;
		}		

		if (flagg == true) {
			Sprite powerup(t6);
			powerup.setPosition(246, 369);
			window.draw(powerup);
		}

		if ((n4 != 100 || n41 != 100) && flagclock == false) {
			clock1.restart();
			time1 = clock1.getElapsedTime().asSeconds();
			flagclock = true;
		}

		if (ban1 == 1) {
			floser();
			Sprite s(t1);
			for (int i = 0; i < M; i++)
				for (int j = 0; j < N; j++)
				{
					if (field[i][j] == 0) continue;
					field[i][j] = 8;
				}
			colorNum = 8;
			gameover = true;
		}


		//Recorta la imagen de los cubos de colores para darle un color diferente a cada pieza
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0)
					continue;
				s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				s.setPosition(j * 18, i * 18);
				s.move(27, 31); //offset
				window.draw(s);
			}
		//DIBUJAR LAS OTRAS
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (P1[i][j] == 0) continue;
				s1.setTextureRect(IntRect(P1[i][j] * 9, 0, 9, 9));
				s1.setPosition(j * 9, i * 9);
				s1.move(7 + 350, 6 + 20); //offset
				window.draw(s1);
			}
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (P2[i][j] == 0) continue;
				s1.setTextureRect(IntRect(P2[i][j] * 9, 0, 9, 9));
				s1.setPosition(j * 9, i * 9);
				s1.move(7 + 465, 6 + 20); //offset
				window.draw(s1);
			}
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (P3[i][j] == 0) continue;
				s1.setTextureRect(IntRect(P3[i][j] * 9, 0, 9, 9));
				s1.setPosition(j * 9, i * 9);
				s1.move(7 + 350, 6 + 245); //offset
				window.draw(s1);
			}
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (P4[i][j] == 0) continue;
				s1.setTextureRect(IntRect(P4[i][j] * 9, 0, 9, 9));
				s1.setPosition(j * 9, i * 9);
				s1.move(7 + 465, 6 + 245); //offset
				window.draw(s1);
			}
		
		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			s.setPosition(a[i].x * 18, a[i].y * 18);
			s.move(27, 31); //offset
			window.draw(s);
		}
		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum3 * 18, 0, 18, 18)); // creamos el bloque de la pieza creada segun el color (X_ini,Y_ini,X_fin,Y_fin)
			if (n3 == 0) {
				s.setPosition((h[i].x + 12.5) * 18 - 10, (h[i].y) * 18 + 9 + 157);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
			else if (n3 == 6) {
				s.setPosition((h[i].x + 12.5) * 18 + 1, (h[i].y) * 18 + 23 + 157);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
			else {
				s.setPosition((h[i].x + 12.5) * 18, (h[i].y) * 18 + 15 + 157);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
		}
		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum2 * 18, 0, 18, 18)); // creamos el bloque de la pieza creada segun el color (X_ini,Y_ini,X_fin,Y_fin)
			if (n2 == 0) {
				s.setPosition((c[i].x + 12.5) * 18 - 10, (c[i].y) * 18 + 9);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
			else if (n2 == 6) {
				s.setPosition((c[i].x + 12.5) * 18 + 1, (c[i].y) * 18 + 23);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
			else {
				s.setPosition((c[i].x + 12.5) * 18, (c[i].y) * 18 + 15);  //le damos la posicion segun a
				s.move(28, 31); //offset
				window.draw(s);
			}
		}
		window.draw(labelPlayer);
		window.draw(text);
		window.draw(barra);

		if (gameover == true) {
			strrr = true;
			gameoverlabel.setString("LOSER");
			gameoverlabel.setFillColor(sf::Color::Yellow);
			gameoverlabel.setCharacterSize(13);
			gameoverlabel.setPosition(65, 120);
			gameoverlabel.setFont(font2);
			window.draw(gameoverlabel);
		}
		if (getvarfin() == true && gameover == false) {
			gameoverlabel.setString("WINNER");
			gameoverlabel.setFillColor(sf::Color::Yellow);
			gameoverlabel.setCharacterSize(13);
			gameoverlabel.setPosition(60, 120);
			gameoverlabel.setFont(font2);
			window.draw(gameoverlabel);
		}

		window.display();

		while (gameover == true) {
			while (window.pollEvent(e)) {
				if (e.type == Event::Closed) {
					window.close();
					music.stop();
				}
			}
			variablefinal = getvarfin();
			while (variablefinal == true) {
				music.stop();
				while (window.pollEvent(e)) {							
					if (e.type == Event::Closed)					
						window.close();
					if (e.type == Event::KeyPressed) {
						if (e.key.code == Keyboard::Return) {
							window.close();
							return 1;
						}
					}
				}
			}
		}
		while (variablefinal == true && gameover == false) {
			music.stop();
			while (window.pollEvent(e)) {						
				if (e.type == Event::Closed)					
					window.close();
				if (e.type == Event::KeyPressed) {
					if (e.key.code == Keyboard::Return) {
						window.close();
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

