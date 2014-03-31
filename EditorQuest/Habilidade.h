#ifndef _HABILIDADE_H_
#define _HABILIDADE_H_

#include "Ator.h"
#include "Sprite.h"

class Habilidade : public Ator
{
private:
	Sprite sprite;
	double x, y, direcao;
	unsigned int indice, tempodevida;
	bool vivo;

public:
	Habilidade(GerenteAtor& _gerente, double _x, double _y, double _direcao);
	
	virtual SDL_Rect PegaBoundingBox();
	
	virtual bool EstaNoJogo();
	virtual unsigned int PegaTipo();

	virtual void Colidiu(Ator* ator);
	virtual void ColidiuMapa(cMap* tile, SDL_Rect* colisao);

	virtual void Inicializar();		
	virtual void Atualizar(Uint32 deltaTime, SDL_Rect* camera);	
	virtual void Renderizar(SDL_Rect* camera);
	virtual void Finalizar();
};

#endif
