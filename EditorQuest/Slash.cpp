#include "Slash.h"
#include "Mapa.h"
#include "GerenteAtor.h"
#include "SDL.h"
#include "Dummy.h"
#include "Jogador.h"

Slash::Slash(GerenteAtor& _gerente, Ator* _origem, int _dano) : Habilidades(_gerente, _origem, _dano)
{
}

void Slash::ColidiuMapa(cMap* tile, SDL_Rect* colisao){
	/*
	switch (tile->id)
	{
	case 1:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
	*/
}

void Slash::Inicializar(){
	sprite.CriaTexturaDaImagemC(gerente.janela->renderer, "resources/imgs/slashh.png", 0, 0, 255, 255, 255);
	tempodevida = 0;
	tipo = DANO_FISICO;
	vivo = true;
	direcao = origem->PegaDirecao();
	x = origem->PegaBoundingBox().x+16.0+cos(direcao)*16.0;
	y = origem->PegaBoundingBox().y+sin(direcao)*16.0;
	direcao2 = -M_PI/4.0;
}

void Slash::Atualizar(Uint32 deltaTime, SDL_Rect* camera){
	if(!origem->EstaNoJogo())
		vivo = false;
	tempodevida += deltaTime;
	direcao2 += deltaTime*((M_PI/2.0)/500.0);
	direcao = origem->PegaDirecao();
	x = origem->PegaBoundingBox().x+16.0+cos(direcao)*16.0;
	y = origem->PegaBoundingBox().y+sin(direcao)*16.0;
	//printf("%f %f %f %f %f %f\n", direcao, x, y, cos(direcao*M_PI/180.0), sin(direcao*M_PI/180.0));
	if(tempodevida >= 500)
		vivo = false;
}

void Slash::Renderizar(SDL_Rect* camera){
	SDL_Point centro = {0, 16};
	sprite.Renderizar(gerente.janela->renderer, x-(double)camera->x, y-(double)camera->y, 0, 0, direcao+direcao2, 1.0, 1.0, &centro);
}

void Slash::Finalizar(){

}