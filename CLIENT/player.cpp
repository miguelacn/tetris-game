#include "pch.h"
#include "play.h"

//FUNCIONES DE LA CLASE JUGADOR
int jugador::point() {
	sc++;
	return sc;
}
int jugador::getscore() {
	return sc;
}
void jugador::tocero() {
	sc = 0;
}
void jugador::floser() {
	loser = true;
}
bool jugador::getlos() {
	return loser;
}