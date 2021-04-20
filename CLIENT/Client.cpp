#include "pch.h"
#include "play.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <list>

using namespace std;
using namespace sf;

const int M = 20;
const int N = 10;

int fieldclient[M][N] = { 0 };
int fieldP1[M][N] = { 0 };
int fieldP2[M][N] = { 0 };
int fieldP3[M][N] = { 0 };
int fieldP4[M][N] = { 0 };

const unsigned short PORT = 5000;
//sf::IpAddress IPADDRESS = sf::IpAddress::getLocalAddress();
sf::IpAddress IPADDRESS = "192.168.43.190";

//CLIENTS
// Create a list to store the future clients
sf::TcpSocket socket;
sf::Mutex globalMutex;
string pseudo;

// Create a selector
	// - Multiplexer that allows to read from multiple sockets.
	// - Socket selectors provide a way to wait until some data is
	// available on a set of sockets, instead of just one.
	// - This is convenient when you have multiple sockets that may
	// possibly receive data, but you don't know which one will be
	// ready first. In particular, it avoids to use a thread for each
	// socket; with selectors, a single thread can handle all the sockets.

SocketSelector selector;

bool comunica = false;
bool quit = false;
bool done = false;
int k = 0;
int pru = -1;
string var;
game g;

//Message
int msgSend;
int msgReceived;
int poder, power = 100, yo = 0;
bool perde;
bool varfin = false;
bool con = 0;

void GetInput(void)
{
	int punto = g.getscore();
	int pow = g.getpower();
	bool los = g.getlos();
	bool stt = g.getstr();

	if (punto > pru || pow != 100) {
		globalMutex.lock();
		msgSend = punto;
		poder = pow;
		perde = los;
		globalMutex.unlock();
		comunica = true;
		poder = g.getpower();
		pru = punto;
	}
	
	if (los == true && con == false) {
		globalMutex.lock();
		msgSend = punto;
		poder = pow;
		perde = los;
		globalMutex.unlock();
		//comunica = true;
		con = true;
	}

	if (stt == true)
		comunica = true;
}

void Client_Game(void)
{
	char pr[20];
	strcpy_s(pr, 20, pseudo.c_str());
	g.juego(pr, fieldclient);
}

bool CountDown(void) {
	bool ban1 = false, ijj = 0;
	while (!ban1) {
		string lala;
		sf::Packet receivePacket;
		globalMutex.lock();
		socket.receive(receivePacket);
		receivePacket >> lala >> yo;
		sf::sleep(milliseconds(500));
		if (receivePacket.getData() != 0) {
			ban1 = true;
			globalMutex.unlock();
			return ban1;
		}
		globalMutex.unlock();
	}
}

void Client_DoStuff(void)
{
	int flag = 0;
	while (!done)
	{
		char pruu[300];
		string username, cadelle;
		int message, desventaja, jota;
		bool per;
		sf::Packet receivePacket;
		sf::Packet sendPacket; //Paquete de datos de envio

		globalMutex.lock();
		socket.receive(receivePacket);
		receivePacket >> username >> message >> desventaja >> per >> cadelle >> jota >> varfin;
		if (receivePacket.getData() != 0) {
			if (jota == 1) { 
				int mul = 0;
				int nP1;
				for (int i = 0; i < M; i++) 
					for (int j = 0; j < N; j++)
					{
						nP1 = (int)cadelle.at(mul);
						fieldP1[i][j] = nP1 - 48;
						mul++;
					}
			}
			if (jota == 2) { 
				int mul = 0;
				int nP1;
				for (int i = 0; i < M; i++)
					for (int j = 0; j < N; j++)
					{
						nP1 = (int)cadelle.at(mul);
						fieldP2[i][j] = nP1 - 48;
						mul++;
					}
			}
			if (jota == 3) { 
				int mul = 0;
				int nP1;
				for (int i = 0; i < M; i++)
					for (int j = 0; j < N; j++)
					{
						nP1 = (int)cadelle.at(mul);
						fieldP3[i][j] = nP1 - 48;
						mul++;
					}
			}
			if (jota == 4) { 
				int mul = 0;
				int nP1;
				for (int i = 0; i < M; i++)
					for (int j = 0; j < N; j++)
					{
						nP1 = (int)cadelle.at(mul);
						fieldP4[i][j] = nP1 - 48;
						mul++;
					}
			}
			if (username != pseudo) {
				//cout << "		[" << username << "]: score " << message << " y la desventaja " << desventaja << " status (1 loser) " << per << " la varfin es " << varfin << endl;
				//cambiar 
				power = desventaja;
			}
		}
		globalMutex.unlock();

		if (comunica == true)
		{
			sf::sleep(milliseconds(100));
			std::string cadena = "";
			for (int temp = 0; temp < 20; temp++) {
				for (int tt = 0; tt < 10; tt++)
					cadena += std::to_string(fieldclient[temp][tt]);
			}
			globalMutex.lock();
			sendPacket << pseudo << msgSend << poder << perde << cadena ;
			globalMutex.unlock();
			socket.send(sendPacket);
			comunica = false;
			poder = 100;
		}
	}
}

bool getvarfin() { return varfin; }

int getpowerenemy() { return power; }

void setpower() { power = 100;}

int whoami() { return yo; }

void getfields(int P1[][N], int P2[][N], int P3[][N], int P4[][N]) {
	for (int i2 = 0; i2 < M; i2++)
		for (int i3 = 0; i3 < N; i3++) {
			P1[i2][i3] = fieldP1[i2][i3]; 
			P2[i2][i3] = fieldP2[i2][i3]; 
			P3[i2][i3] = fieldP3[i2][i3]; 
			P4[i2][i3] = fieldP4[i2][i3];
		}
}

int client(char ps[])
{
	pseudo = ps;

	if (socket.connect(IPADDRESS, PORT) == Socket::Done)
	{
		cout << "Connecte au serveur!\nText:" << IPADDRESS << endl;
		socket.setBlocking(false);
		Packet sendPacket;
		sendPacket << pseudo;
		socket.send(sendPacket);
		return 0;
	}
	else {
		cout << "Serveur DOWN!" << endl;
		return 1;
	}
		//cerrar la ventana o mandar mensaje de error 
	//Client_DoStuff();
}
