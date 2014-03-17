#ifndef _INGAME_H_
#define _INGAME_H_

#include "Tela.h"
#include "Sprite.h"

class Ingame : public Tela
{
private:
	Sprite mapa;
	SDL_Rect camera;
public:
	virtual void Inicializar(Janela* _janela);
	virtual void Atualizar() ;
	virtual void Renderizar();
	virtual void Finalizar();

	virtual Tela* ProximaTela();
};

#endif
