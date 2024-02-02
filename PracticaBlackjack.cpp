// PracticaBlackjack.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
// Utilitzar diccionaris si els arrays no em funcionen

//compruebo el repositorio
#include <iostream>

#include <algorithm>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>

#include <thread>
#include <future>
#include <chrono>

//funcion asyncrona con sleep ayudado con chatgpt

using namespace std;
int posicionMazo = 0;
int posicionJugador = 0;
int posicionCrupier = 0;
const int TAMANOMANO = 9;
const int PALOS = 4;
const int CARTASPALO = 13;

const float ALTOCARTA = 95;//95 +1
const float ANCHOCARTA = 70; // 70+1

const int POSICION_INICIAL_JUGADORX = 365;
const int POSICION_INICIAL_JUGADORY = 353;

const int POSICION_INICIAL_CRUPIERX = 365;
const int POSICION_INICIAL_CRUPIERY = 92;

const float POSICION_MAZO_X = 445.0f;//X del 0.0 del Mazo
const float POSICION_MAZO_Y = 222.0f;

const int COLUMN[CARTASPALO] = { 1,74,147,220,293,366,439,512,585,658,731,804,877 };
const int ROW[PALOS] = { 197,99,1,295 };

bool bFase1 = true;
bool bFase2 = false;
bool bFase3 = false;

char keyRobar = 'k';
char keyPlantar = 'e';

const int DELAY = 500;

struct CARTA {
	int p00[2];
	int p01[2];
	int p10[2];
	int p11[2];
	bool player;

};
vector <CARTA> cartasTablero;


// las proporciones de las cartas son 72 de ancho 95 de alto
void calculo( int& puntuacion, int (&mano)[TAMANOMANO][2], int valores[CARTASPALO],bool player,int posicionMano){
	puntuacion = 0;
	int ases = 0;
	for (int i = 0; i <= posicionMano; i++) {
		puntuacion += valores[mano[i][1]];
		if (mano[i][1] == 0) {// calculo los ases de la mano
			ases++;
		}
	}
	for (int i = 0; i < ases; i++) {
		if (puntuacion <= 11) {
			puntuacion += 10;
		}
			
	}
	if (player) {
		cout << "Puntuacion jugador: " << puntuacion <<"\n";
	}
	else{
		cout << "Puntuacion crupier: " << puntuacion << "\n";
	}
}
string nombrar(int palo, int mano) {
	return "" + palo + mano;
}
void robo(int &punt, int(&mano)[TAMANOMANO][2], int (&mazo)[52][2], int valores[], int& posicionMano, bool player) {
	for (int j = 0; j < 2; j++) {//la carta robada
		mano[posicionMano][j] = mazo[posicionMazo][j];//asigno a la mano correspondiente una carta con valor de la carta del mazo
	}
	CARTA miCarta;//obtiene los puntos del sprite de la carta
	miCarta.p00[0] = ROW[mano[posicionMano][0]];//palo->Y del sprite
	miCarta.p00[1] = COLUMN[mano[posicionMano][1]];//valor->X del sprite

	miCarta.p01[0] = ROW[mano[posicionMano][0]];
	miCarta.p01[1] = COLUMN[mano[posicionMano][1]]+ANCHOCARTA;

	miCarta.p10[0] = ROW[mano[posicionMano][0]]+ALTOCARTA;
	miCarta.p10[1] = COLUMN[mano[posicionMano][1]];

	miCarta.p11[0] = ROW[mano[posicionMano][0]]+ALTOCARTA;
	miCarta.p11[1] = COLUMN[mano[posicionMano][1]]+ANCHOCARTA;

	miCarta.player = player;
	
	cartasTablero.push_back(miCarta);
	calculo(punt,mano,valores,player,posicionMano);
	posicionMazo++;// cada vez que se utiliza el mazo se le suma uno a carta.
	posicionMano++;
	std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
	
}
void victoria() {
	cout << "ganaste\n";
}
void derrota(){
	cout << "perdiste\n";
}


void fase3(int(&mazo)[52][2], int(&manoJugador)[TAMANOMANO][2], int& puntJugador, int(&manoCrupier)[TAMANOMANO][2], int& puntCrupier, int valores[CARTASPALO]) {// modo crupier, roba hasta intentar superar al jugador
	
	while (puntCrupier < puntJugador) {
		robo(puntCrupier, manoCrupier, mazo, valores, posicionCrupier, false);
	}
	bFase3 = false;
	if (puntCrupier == puntJugador || puntCrupier <= 21) {
		derrota();
	}
	else {
		victoria();
	}
}
void fase2(int(&mazo)[52][2], int(&manoJugador)[TAMANOMANO][2], int& puntJugador, int(&manoCrupier)[TAMANOMANO][2], int& puntCrupier, int valores[CARTASPALO]) {// Bucle en que se pregunta al jugador si quiere robar una carta hasta que se pasa o pasa al crupier
	robo(puntJugador, manoJugador, mazo, valores, posicionJugador, true);
	
	if (puntJugador == 21) {
		bFase1 = false;
		bFase2 = false;
		bFase3 = true;
		fase3(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
	}
	else if (puntJugador > 21) {
		bFase2 = false;
		derrota();
	}
}
void fase1( int(&mazo)[52][2], int(&manoJugador)[TAMANOMANO][2],int &puntJugador, int(&manoCrupier)[TAMANOMANO][2], int &puntCrupier, int valores[CARTASPALO]) {
	// se reparten 2 cartas al jugador y 1 al crupier
	//jugadorx
	//std::thread roboThread;
	for (int i = 0; i < 2; i++) {//jugador roba 2 cartas
		robo(puntJugador, manoJugador, mazo, valores, posicionJugador, true);
		
	}

	//crupier
	for (int i = 0; i < 1; i++) {
		//roboThread = std::thread(robo, std::ref(puntCrupier), std::ref(manoCrupier), std::ref(mazo), std::ref(valores), std::ref(posicionCrupier), false);
		robo(puntCrupier, manoCrupier, mazo, valores, posicionCrupier, false);
		
	}							
	//si se reparten 21 al jugador pasamos a modo crupier
	if (puntJugador == 21) {
		bFase1 = false;
		bFase2 = false;
		bFase3 = true;
		//fase3(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
	}
	else {//de lo contrario pasamos a fase2
		bFase1 = false;
		bFase2 = true;
		bFase3 = false;
		//fase2(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
	}
}

void impresionMazo(int PALOS, int CARTASPALO, int(&mazo)[52][2]) {
	//impresion del mazo
	/*for (int i = 0; i < PALOS * CARTASPALO; i++) {//POR CADA posicion del mazo
		cout << mazo[i][0] << ", " << mazo[i][1] << "\n";
	}*/
}
void preparacion(int PALOS, int (&mazo)[52][2], int valores[CARTASPALO]) {//con (&mazo)[tamaño][tamaño] puedo pasar el array como referencia
	int miNum = 0;
	//relleno del mazo
	for (int j = 0; j < PALOS; j++) {
		for (int k = 0; k < CARTASPALO; k++) {
			for (int i = 0; i < 2; i++) {
				if (i == 0) {
					mazo[miNum][i] = j;
				}
				else {
					mazo[miNum][i] = k;
				}
			}

			miNum++;
		}
	}
	//barajado del mazo
	random_shuffle(begin(mazo), end(mazo));
}

int pruebasSfml(int(&mazo)[52][2], int(&manoJugador)[TAMANOMANO][2], int& puntJugador, int(&manoCrupier)[TAMANOMANO][2], int& puntCrupier, int valores[CARTASPALO]) {
	sf::Texture txTablero;
	sf::Texture txReverso;
	sf::Texture txCartas;
	//Defino el tablero
	txTablero.loadFromFile("./img/tablero.png");
	// Crear un sprite para la textura
	sf::Sprite spTablero(txTablero);  
	//tablero definido
	
	//Defino la carta Reverso
	txReverso.loadFromFile("./img/reverso.png");
	
	// Obtener el tamaño de la textura (ancho y alto)

	// defino la textura de las cartas
	txCartas.loadFromFile("./img/cards.png");
	

	sf::Vector2u textureSize = txTablero.getSize();  
	sf::RenderWindow window(sf::VideoMode(textureSize.x, textureSize.y), "MunchJack", sf::Style::Titlebar | sf::Style::Close);//tamaño de las dimensiones de "tablero" e "inmutable"

	std::thread faseThread;


	while (window.isOpen()) { 
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			/*else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)//evento de clicar
				{
					//if(event.mouseButton.x  event.mouseButton.y)
					//std::cout << "the right button was pressed" << std::endl;
					
				}
			}p*/
			if (event.type == sf::Event::TextEntered)
			{
				
				if (event.text.unicode < 128) {
					if (bFase1) {
						if (faseThread.joinable()) {
							faseThread.join();
						}
						if (static_cast<char>(event.text.unicode) == keyRobar) {
							//std::thread faseThread = std::thread(fase1,mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
							faseThread = std::thread(fase1, std::ref(mazo), std::ref(manoJugador), std::ref(puntJugador), std::ref(manoCrupier), std::ref(puntCrupier), valores);
							//fase1(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
						}
					}
					else if (bFase2) {
						
						if (static_cast<char>(event.text.unicode) == keyRobar) {
							if (faseThread.joinable()) {
								faseThread.join();
							}
							faseThread = std::thread(fase2, std::ref(mazo), std::ref(manoJugador), std::ref(puntJugador), std::ref(manoCrupier), std::ref(puntCrupier), valores);
							//fase2(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
						}
						if (static_cast<char>(event.text.unicode) == keyPlantar) {
							if (faseThread.joinable()) {
								faseThread.join();
							}
							faseThread = std::thread(fase3, std::ref(mazo), std::ref(manoJugador), std::ref(puntJugador), std::ref(manoCrupier), std::ref(puntCrupier), valores);
							//fase3(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
						}
					}
					 
				}
					std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
			}
			if (bFase3) {
				if (faseThread.joinable()) {
					faseThread.join();
				}
				faseThread = std::thread(fase3, std::ref(mazo), std::ref(manoJugador), std::ref(puntJugador), std::ref(manoCrupier), std::ref(puntCrupier), valores);
				//fase3(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
			}
		}
		

		window.clear(sf::Color::Black); 
		window.draw(spTablero);
		float desplazamientoY = 0;
		// dibujado del mazo en el tablero
		for (int i = 0; i < 52 - posicionMazo; i++) {
			sf::Sprite spReverso(txReverso);
			spReverso.setPosition(POSICION_MAZO_X, POSICION_MAZO_Y + desplazamientoY);
			desplazamientoY -= 0.5;
			window.draw(spReverso);
		}
		
		//dibujado de de las manos en el tablero
		int desplazamientoJugador = 0;
		int desplazamientoCrupier = 0; 
		for (CARTA card : cartasTablero) {
			
			sf::IntRect r1(card.p00[1], card.p00[0], ANCHOCARTA, ALTOCARTA);
			sf::Sprite spCarta(txCartas, r1);
			if (card.player) {
				spCarta.setPosition(POSICION_INICIAL_JUGADORX + desplazamientoJugador, POSICION_INICIAL_JUGADORY);
				desplazamientoJugador += 20;
			}
			else {
				spCarta.setPosition(POSICION_INICIAL_CRUPIERX + desplazamientoCrupier, POSICION_INICIAL_CRUPIERY);
				desplazamientoCrupier += 20;
			}
			window.draw(spCarta);
		}

		window.display(); 
		
	}
	if (faseThread.joinable()) {
		faseThread.join();
	}
	return 0;


}

int main()
{
	srand(time(NULL));

	//declaracion de arrays que se van a utilizar
	//estas 2 tendran la informacion de las cartas, las cartas tendran referencias a esta información
	string palos[PALOS] = { "Corazones","Picas","Treboles","Diamantes" };
	string nombreCartas[CARTASPALO] = { "As","2","3","4","5","6","7","8","9","10","J","Q","K"};

	int valorCartas[CARTASPALO] = {1,2,3,4,5,6,7,8,9,10,10,10,10};

	//aqui creo el mazo, son 52 arrays que contienen arrays de 2
	int mazo[52][2];

	//la mano del jugador y crupier
	int manoJugador[TAMANOMANO][2];
	int puntJugador = 0;
	int manoCrupier[TAMANOMANO][2];
	int puntCrupier = 0;

	preparacion(PALOS, mazo, valorCartas);//prepara el mazo

	//impresionMazo(PALOS, CARTASPALO, mazo);
	//window();
	
	
	pruebasSfml(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valorCartas);

}


