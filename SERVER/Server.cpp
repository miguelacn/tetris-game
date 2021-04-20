#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <list>
#include <omp.h>

using namespace std;
using namespace sf;
std::vector<std::string> name_user;

const unsigned short PORT = 5000;
//sf::IpAddress IPADDRESS = sf::IpAddress::getLocalAddress();
sf::IpAddress IPADDRESS = "192.168.43.190";

//CLIENTS
// Create a list to store the future clients
sf::TcpSocket socket;
sf::Mutex globalMutex;
string pseudo;

//SERVER
// Create a socket to listen to new connections
TcpListener listener;

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
bool uno = false;
int k = 0;
std::vector<TcpSocket*>Clients;

//Message
int msgSend;
int msgReceived;
int pod;
bool sta;

void GetInput(void)
{
	std::string s;
	getline(cin, s);

	if (s == "exit")
		quit = true;
}

void countdown() {
	
	if ((int)Clients.size() == 2) {
		for (int i = 15; i > 0; --i) {
			cout << i << endl ;
			sf::sleep(milliseconds(1000));
		}
		int pu = 30;
		auto s = std::to_string(pu);
		for (int j = 0; j < (int)Clients.size(); j++) {
			Packet sendPacket1;
			sendPacket1 << s << (j+1) ;
			socket.send(sendPacket1);
			Clients[j]->send(sendPacket1);
		}
		uno = true;
	}
}

void server_DoStuff(void)
{
	// Endless loop that waits for new connections
	while (!uno && (int)Clients.size() < 5)
	{
		Packet sendPacket;
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			// Test the listener
			if (selector.isReady(listener))
			{
				sf::TcpSocket tcpSocket;
				tcpSocket.setBlocking(false);

				sf::TcpListener listenerSocket;
				listenerSocket.setBlocking(false);

				sf::UdpSocket udpSocket;
				udpSocket.setBlocking(false);

				// The listener is ready: there is a pending connection
				sf::TcpSocket* socket = new sf::TcpSocket;

				if (listener.accept(*socket) == sf::Socket::Done)
				{
					// Add the new client to the clients list
					Clients.push_back(socket);

					// Add the new client to the selector so that we will
					// be notified when he sends something
					selector.add(*socket);

					Packet receivePacket;
					if (socket->receive(receivePacket) == Socket::Done)
					{
						name_user.push_back(pseudo);
						string pseudo;
						receivePacket >> pseudo;
						cout << pseudo << " Vient de se connceter" << endl;
					}
				}
				else {
					// Error, we won't get a new connection, delete the socket
					delete socket;
				}
			}
		}
	}
}

void server_DoStuff2(void)
{
	int co = 0;
	while (!done)
	{
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			if (uno == true)
			{
				for (int i = 0; i < (int)Clients.size(); i++)
				{
					Packet sendPacket;
					if (selector.isReady(*Clients[i]))
					{
						Packet receivePacket;
						if (Clients[i]->receive(receivePacket) == Socket::Done)
						{
							const int M = 20, N = 10;
							string pseudo, cadenarec;
							int y = 0;									
							receivePacket >> pseudo >> msgSend >> pod >> sta >> cadenarec >> y;
							//cout << "(S)Recibido: de C." << i << " (" << pseudo << ") ( " << y << " ):  " << msgSend << " el poder " << pod << " y status (1 loser) " << sta << endl;
							
							if (sta) {
								co++;
							}
							if (co == (int)Clients.size() - 1) {
								sendPacket << pseudo << msgSend << pod << sta << cadenarec << (i+1) << true;
								socket.send(sendPacket);
								#pragma omp parallel for
								for (int j = 0; j < (int)Clients.size(); j++)
								{
									//fin de la partida
									Clients[j]->send(sendPacket);
									//cout << "(S)Eviado para C." << j << " (" << pseudo << "): score " << msgSend << " el poder " << pod << " y status (1 loser) " << sta << " alo " << co << endl;
								}
								//break;
							}
							sendPacket << pseudo << msgSend << pod << sta << cadenarec << (i + 1);
							socket.send(sendPacket);
							#pragma omp parallel for
							for (int j = 0; j < (int)Clients.size(); j++)
							{
								Clients[j]->send(sendPacket);
								//cout << "(S)Eviado para C." << j << " (" << pseudo << "): score " << msgSend << " el poder " << pod << " y status (1 loser) " << sta << endl;
							}
							sta = false;
						}
					}
				}
			}
		}
	}
}

void server(void)
{
	bool done = false;

	if (listener.listen(PORT) == Socket::Done)
	{
		cout << "Serveur correctement lance " << endl;
	}
	else
		exit(0);

	// Add the listener to the selector
	selector.add(listener);
	
	sf::Thread* thread = 0;
	thread = new sf::Thread(&server_DoStuff);
	thread->launch();
	do {
		countdown();
	} while (uno == false);

	server_DoStuff2();
}

int main()
{
	server();

	while (!quit)
	{
		GetInput();
	}
	return 0;
}

