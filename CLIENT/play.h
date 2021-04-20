#pragma once
#ifndef PLAYHPP
#define PLAYHPP
#include "Graph.h"

class jugador {
	int sc; //puntaje de cada jugador 
	bool loser;
public:
	jugador(int s=0) : sc(s) {
		loser = false;
	}
	int point();
	int getscore();
	void tocero();
	void floser();
	bool getlos();
};

class game : public jugador, public Graph{
	int power, n, n2, n3; //poder que se env�a
	bool strrr;
public:
	game() {
		power = 100;
		strrr = false;

	}
	int getpower();
	bool getstr();
	bool check();
	void mov(int);
	void rot();
	void tick(float *, int *, int *,bool *);
	void checklines(int *, int, int *);
	int single(char[]);
	int juego(char[], int[][10]);
};

#endif