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
using namespace std;
int posicionMazo = 0;
int posicionJugador = 0;
int posicionCrupier = 0;
const int TAMANOMANO = 15;
const int PALOS = 4;
const int CARTASPALO = 13;

const float ALTOCARTA = 95;//95 +1
const float ANCHOCARTA = 70; // 70+1

const int POSICION_INICIAL_JUGADORX = 445;
const int POSICION_INICIAL_JUGADORY = 352;

const int POSICION_INICIAL_CRUPIERX = 445;
const int POSICION_INICIAL_CRUPIERY = 92;

const float POSICION_MAZO_X = 444.0f;//X del 0.0 del Mazo
const float POSICION_MAZO_Y = 222.0f;

// las proporciones de las cartas son 72 de ancho 95 de alto
void calculo( int& puntuacion, int (&mano)[TAMANOMANO][2], int valores[CARTASPALO]){
	puntuacion = 0;
	int ases = 0;
	for (int i = 0; i < TAMANOMANO; i++) {
		puntuacion += valores[mano[i][1]];
		if (mano[i][1] == 1) {// calculo los ases de la mano
			ases++;
		}
	}
	while (ases > 0 && puntuacion <= 11) {
		for (int i = 0; i < ases; i++) {
			puntuacion += 10;
		}
	}
	
}
void robo(int &punt, int(&mano)[TAMANOMANO][2], int (&mazo)[52][2], int valores[], int &posicionMano) {
	for (int j = 0; j < 2; j++) {//la carta robada
		mano[posicionMano][j] = mazo[posicionMazo][j];
	}
	calculo(punt, mano, valores);
	posicionMazo++;// cada vez que se utiliza el mazo se le suma uno a carta.
	posicionMano++;
}
void victoria() {
	cout << "ganaste";
}
void derrota(){
	cout << "perdiste";
}

void fase3(int(&mazo)[52][2], int(&manoJugador)[TAMANOMANO][2], int& puntJugador, int(&manoCrupier)[TAMANOMANO][2], int& puntCrupier, int valores[CARTASPALO]) {// modo crupier, roba hasta intentar superar al jugador
	while (puntCrupier < puntJugador) {
		robo(puntCrupier, manoCrupier, mazo, valores, posicionCrupier);
	}
	if (puntCrupier == puntJugador || puntCrupier < 21) {
		derrota();
	}
	else {
		victoria();
	}
}
void fase2(int(&mazo)[52][2], int(&manoJugador)[TAMANOMANO][2], int& puntJugador, int(&manoCrupier)[TAMANOMANO][2], int& puntCrupier, int valores[CARTASPALO]) {// Bucle en que se pregunta al jugador si quiere robar una carta hasta que se pasa o pasa al crupier
	while (puntJugador <21) {
		cout << "quieres robar una carta?";
		string opcion;
		cin >> opcion;
		if (opcion == "s") {
			robo(puntJugador, manoJugador, mazo, valores, posicionJugador);
		}
		else if (opcion == "n") {
			fase3(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
		}
	}
	if (puntJugador == 21) {
		fase3(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
	}
	else if (puntJugador > 21) {
		derrota();
	}
}
void fase1( int(&mazo)[52][2], int(&manoJugador)[TAMANOMANO][2],int &puntJugador, int(&manoCrupier)[TAMANOMANO][2], int &puntCrupier, int valores[CARTASPALO]) {
	// se reparten 2 cartas al jugador y 1 al crupier

	//jugador
	for (int i = 0; i < 2; i++) {//jugador roba 2 cartas
		robo(puntJugador, manoJugador, mazo, valores,posicionJugador);
	}

	//crupier
	for (int i = 0; i < 1; i++) {
		robo(puntCrupier, manoCrupier, mazo, valores, posicionCrupier);
	}							
	//si se reparten 21 al jugador pasamos a modo crupier
	if (puntJugador == 21) {
		fase3(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
	}
	else {//de lo contrario pasamos a fase2
		fase2(mazo, manoJugador, puntJugador, manoCrupier, puntCrupier, valores);
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
int window() {
	sf::Color colors[4] = { sf::Color::Green,sf::Color::Red,sf::Color::Blue,sf::Color::Yellow };
	// create the window
	sf::RenderWindow window(sf::VideoMode(1366, 768), "MunchJack"/*, sf::Style::Fullscreen*/);
	sf::CircleShape shape(100.f);
	int colorNum = 0;
	
	

	// run the program as long as the window is open
	while (window.isOpen())
	{
		shape.setFillColor(colors[colorNum]);
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		window.draw(shape);

		// end the current frame
		window.display();
		if (colorNum+1==(sizeof(colors)/sizeof(sf::Color))) {
			colorNum = 0;
		}
		else {
			colorNum++;
		}
	}

	return 0;
}
int pruebasSfml() {
	sf::Texture txTablero;
	sf::Texture txReverso;
	//Defino el tablero
	txTablero.loadFromFile("./img/tablero.png");
	// Crear un sprite para la textura
	sf::Sprite spTablero(txTablero);  
	//tablero definido
	
	//Defino la carta Reverso
	txReverso.loadFromFile("./img/reverso.png");
	sf::Sprite spReverso(txReverso);
	spReverso.setPosition(POSICION_MAZO_X, POSICION_MAZO_Y);//por ahora una carta
	// Obtener el tamaño de la textura (ancho y alto)
	sf::Vector2u textureSize = txTablero.getSize();  
	sf::RenderWindow window(sf::VideoMode(textureSize.x, textureSize.y), "MunchJack", sf::Style::Titlebar | sf::Style::Close);//tamaño de las dimensiones de "tablero" e "inmutable"

	while (window.isOpen()) { 
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}		
		window.clear(sf::Color::Black); 
		window.draw(spTablero);
		window.draw(spReverso);

		window.display(); 
	}
	return 0;


}
int main()
{
	srand(time(NULL));

	//declaracion de arrays que se van a utilizar
	//estas 2 tendran la informacion de las cartas, las cartas tendran referencias a esta información
	string palos[PALOS] = { "Corazones","Picas","Treboles","Diamantes" };
	int rowPalos[PALOS] = { 197,99,1,295 };

	string nombreCartas[CARTASPALO] = { "As","2","3","4","5","6","7","8","9","10","J","Q","K"};
	int valorCartas[CARTASPALO] = {1,2,3,4,5,6,7,8,9,10,10,10,10};
	int columnCarta[CARTASPALO] = { 1,74,147,220,293,366,439,512,585,658,731,804,877 };

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
	pruebasSfml();

}


