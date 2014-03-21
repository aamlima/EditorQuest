#ifndef _DEBUGJANELAS_H_
#define _DEBUGJANELAS_H_

#include "Tela.h"
#include "Sprite.h"
#include <sstream>

class DebugJanelas : public Tela{
private:
	TTF_Font *fonte[3];
	int x, y;
	double r;
	Sprite texto;
	Sprite menu[5];
	std::stringstream txt;

public:
	virtual void Inicializar(Janela* _janela);
	virtual void Atualizar();
	virtual void Renderizar();
	virtual void Finalizar();

	virtual Tela* ProximaTela();
	
	~DebugJanelas();

};
#endif