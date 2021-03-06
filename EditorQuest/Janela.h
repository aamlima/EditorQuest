#ifndef _JANELA_H_
#define _JANELA_H_

#include "globaldef.h"
#include "Entrada.h"

class GerenciadorTelas;
class GerenteJanela;
class Tela;

class Janela{
private:
	SDL_Window* window;
	GerenciadorTelas* gerente;
	SDL_Color corfundo;

public:	
	SDL_Renderer* renderer;
	GerenteJanela* jGerente;
	Entrada entrada;

	Janela(Tela* telainicial, GerenteJanela* _jGerente);
	void Inicializar(Tela* telainicial);
	void ProcessarEventos(SDL_Event& evento);
	void ProcessarEventosW(SDL_Event& evento);
	void Atualizar(Uint32 deltaTime);
	void Renderizar();
	void Finalizar();
	~Janela();
	
	void Mostrar();
	void Esconder();
	void Remover();

	bool Existe();

	SDL_Color PegaCorFundo();
	Uint32 PegaID();
	const char* PegaTitulo();
	void PegaPosicao(int &x, int &y);
	void PegaTamanho(int &w, int &h);
	SDL_Rect PegaPosicaoeTamanho();

	void SetaCorFundo(SDL_Color cor);
	void SetaCorFundo(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void SetaTitulo(const char* titulo);
	void SetaIcone(SDL_Surface* icone);
	void SetaPosicao(int x, int y);
	void SetaTamanho(int w, int h);
	void ModoJanela();
	void ModoTelaCheia();
};

#endif