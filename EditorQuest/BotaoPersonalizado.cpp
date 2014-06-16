#include "BotaoPersonalizado.h"

void BotaoPersonalizado::Inicializar(SDL_Renderer* renderer, char* _texto, double _x, double _y, TTF_Font* fonte, SDL_Color cor){
	texto = "";
	x = _x;
	y = _y;
	pressionado = hover = solto = false;
	sprite[0].CriaTexturaDaImagemC(renderer, _texto, 0, 0, 255, 255, 255);
}

void BotaoPersonalizado::Renderizar(SDL_Renderer* renderer){
	sprite[0].Renderizar(renderer, x, y);
}