#ifndef _TELA_H_
#define _TELA_H_

class Janela;
#include "globaldef.h"

#ifndef _INPUT_
#define _INPUT_
#define PegaTecla janela->entrada.pegaTecla
#define PegaMouse janela->entrada.pegaMouse
#endif

class Tela
{
protected:
	Janela* janela;

public:
	virtual void Inicializar(Janela* _janela) = 0;
	virtual void Atualizar(Uint32 deltaTime) = 0;
	virtual void Renderizar() = 0;
	virtual void Finalizar() = 0;

	virtual Tela* ProximaTela() = 0;

	virtual ~Tela() {}
};
#endif
