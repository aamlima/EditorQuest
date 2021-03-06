#include "MenuInicial.h"
#include "Janela.h"
#include "Ingame.h"
#include "Editor.h"

void MenuInicial::Inicializar(Janela* _janela){
	janela = _janela;
	janela->SetaTitulo("Walachia - Menu Inicial");
	int w, h;
	janela->PegaTamanho(w, h);
	janela->SetaCorFundo(255, 255, 255);
	janela->Mostrar();

	unsigned int altura = (unsigned int)ceil((double)h/32.0);
	unsigned int largura = (unsigned int)ceil((double)w/32.0);

	mapa.Novo(largura, altura);
	for(unsigned int i = 0; i < altura; i++){
		for(unsigned int j = 0; j < largura; j++){
			if( i == 0 || j == 0 || j == largura-1 || i == altura-1)
				mapa.Alterar(j, i, 1);
			else
				mapa.Alterar(j, i, 2);
		}
	}
	mapa.Inicializar(janela->renderer);

	fonte = TTF_OpenFont("resources/fonts/pix.ttf", 32);
	cor.r = cor.g = cor.b = 0;
	jogar.Inicializar(janela->renderer, "resources/botoes/Jogar.png", w/2.0-71.0, h/10.0*4.0);
	sair.Inicializar(janela->renderer, "resources/botoes/Sair do jogo.png", w/2.0-131.0, h/10.0*8.0);
	altnome.Inicializar(janela->renderer, "resources/botoes/alt.png", 0, 540, fonte, cor);
	titulo.CriaTexturaDaImagem(janela->renderer, "resources/imgs/Walachia.png");
	portal.CriaTexturaDaImagem(janela->renderer, "resources/imgs/portal.png", 122);
	indice = 0;
	nome = "Default";
	nomespr.CriaTexturaDoTexto(janela->renderer, nome.c_str(), fonte, cor);
	time = 0;
	resolucao = false;
	editor = false;
	for(int i = 0; i < 6; i++)
		code[i] = false;
	janela->entrada.SetaTamanhoTexto(10);
	input = false;
}

void MenuInicial::Atualizar(Uint32 deltaTime){
	KB_Botao* Teclas = PegaTecla();
	M_Mouse* Mouse = PegaMouse();
	SDL_Color cor2 = {255, 0, 0};
	time += deltaTime;
	if(time >= 125){
		time -= 125;
		indice = (indice+1)%4;
	}
	altnome.Atualizar(Mouse);
	jogar.Atualizar(Mouse);
	sair.Atualizar(Mouse);
	if(altnome.Pressionado()){
			input = janela->entrada.toggleInputTexto();
			if(!input)
				nomespr.CriaTexturaDoTexto(janela->renderer, nome.c_str(), fonte, cor);
		}
	if (Teclas[KB_1].pressionado)
	{
		janela->ModoJanela();
		janela->SetaTamanho(800, 600);
		resolucao = true;
	}
	if (Teclas[KB_2].pressionado)
	{
		janela->ModoJanela();
		janela->SetaTamanho(1024, 768);
		resolucao = true;
	}
	if (Teclas[KB_3].pressionado)
	{
		janela->ModoTelaCheia();
		resolucao = true;
	}

	if(Teclas[KB_E].pressionado)
		code[0] = true;
	if(Teclas[KB_D].pressionado)
		code[1] = true;
	if(Teclas[KB_I].pressionado)
		code[2] = true;
	if(Teclas[KB_T].pressionado)
		code[3] = true;
	if(Teclas[KB_O].pressionado)
		code[4] = true;
	if(Teclas[KB_R].pressionado)
		code[5] = true;
	int c = 0;
	for(int i = 0; i < 6; i++){
		if(code[i])
			c++;
	}

	if(c == 6 && !editor)
	{
		editor = true;
		new Janela(new Editor("num1"), janela->jGerente);
		janela->ModoJanela();
		janela->SetaTamanho(800, 600);
		resolucao = true;
	} 
	if(janela->entrada.textoUpdate()){
		nome = janela->entrada.pegaTexto();
		nomespr.CriaTexturaDoTexto(janela->renderer, nome.c_str(), fonte, cor2);
	}
}

void MenuInicial::Renderizar(){
	int w, h;
	janela->PegaTamanho(w, h);
	mapa.Renderizar(janela->renderer, 0);
	titulo.Renderizar(janela->renderer, ((w-(double)titulo.PegaDimensao().w)/2.0), 25.0);
	portal.Renderizar(janela->renderer, ((w-(double)portal.PegaDimensao().w)/2.0), h/1.9, indice);
	jogar.Renderizar(janela->renderer);
	sair.Renderizar(janela->renderer);
	altnome.Renderizar(janela->renderer);
	nomespr.Renderizar(janela->renderer, (altnome.PegaDimensao().w-nomespr.PegaDimensao().w)/2, 515);
}

void MenuInicial::Finalizar(){
	TTF_CloseFont(fonte);
}

Tela* MenuInicial::ProximaTela(){
	if(PegaTecla()[KB_ENCERRA].pressionado || sair.Pressionado())
		return 0;
	if(jogar.Pressionado())
		return new Ingame(nome);
	if (resolucao)
		return new MenuInicial();	
	return this;
}
	
MenuInicial::~MenuInicial(){
}