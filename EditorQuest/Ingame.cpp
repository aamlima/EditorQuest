#include "Ingame.h"
#include "Lobisomem.h"
#include "Crowley.h"
#include "Lucifer.h"
#include "janela.h"
#include "MenuInicial.h"
#include "Equipamento.h"
#include "Armadilha.h"
#include "DropItem.h"
#include "Consumavel.h"
#include <fstream>
#include <iostream>

using namespace std;

Ingame::Ingame(string nome) : perfil(nome){}

void Ingame::Inicializar(Janela* _janela){
	janela = _janela;
	janela->SetaTitulo("Walachia - Ingame");
	janela->SetaCorFundo(0, 0, 0);

	string nomes[6];
	string desc[6];

	nomes[0] = "Arma";
	nomes[1] = "Capacete";
	nomes[2] = "Peitoral";
	nomes[3] = "Calca";
	nomes[4] = "Botas";
	nomes[5] = "Pocao";

	desc[0] = "Uma arma";
	desc[1] = "Um capacete";
	desc[2] = "Um peitoral";
	desc[3] = "Uma calca";
	desc[4] = "Um par de botas";
	desc[5] = "Uma pocao";	

	extra = true;

	int w, h;
	janela->PegaTamanho(w, h);
	camera.x = 0;
	camera.y = 0;
	camera.w = w;
	camera.h = h;
	fonte = TTF_OpenFont("resources/fonts/pix.ttf", 32);
	SDL_Color cor = {0, 0, 0};

	botoes[BOTAO_STATUS].Inicializar(janela->renderer, "resources/botoes/Status.png", 50, h/10.0);
	botoes[BOTAO_INVENTARIO].Inicializar(janela->renderer, "resources/botoes/itens.png", 50, h/10.0*2.25);
	botoes[BOTAO_MENUINICIAL].Inicializar(janela->renderer, "resources/botoes/Menu Inicial.png", 50, h/10.0*5.50);
	botoes[BOTAO_SAIR].Inicializar(janela->renderer, "resources/botoes/Sair do jogo.png", 50, h/10.0*6.75);
	botoes[BOTAO_VOLTAR].Inicializar(janela->renderer, "resources/botoes/Voltar.png", 50, h/10.0*8.75);
	botoes[BOTAO_USAR].Inicializar(janela->renderer, "resources/botoes/Usar.png", w/10.0*8.25, h/10.0*8.75);
	botoes[BOTAO_USAR2].Inicializar(janela->renderer, "resources/botoes/Remover.png", w/10.0*7.0, h/10.0*8.75);
	botoes[BOTAO_DESTRUIR].Inicializar(janela->renderer, "resources/botoes/Destruir.png", w/10.0*5.0, h/10.0*8.75);
	botoes[BOTAO_RETRY].Inicializar(janela->renderer, "resources/botoes/Retry.png", 50, h/10.0*7.75);
	botoes[BOTAO_PROX].Inicializar(janela->renderer, "resources/botoes/Prox.png", 50, h/10.0*7.75);
	filtro.CriaTexturaDaImagem(janela->renderer, "resources/imgs/filtro.png");
	gameover.CriaTexturaDaImagem(janela->renderer, "resources/imgs/gameover.png");
	victory.CriaTexturaDaImagem(janela->renderer, "resources/imgs/vitoria.png");

	ifstream profile("resources/profiles/"+perfil+".player", ios_base::binary);
	if(profile.is_open()){
		unsigned int size;
		profile.read((char*)&size, sizeof(unsigned int));
		profile.read((char*)mapatual.c_str(), size);
		mapatual = mapatual.c_str();
		profile.close();
	}
	if(mapatual.size() == 0){
		mapatual = "num1";
	}
	mapa.Carregar(mapatual);
	mapa.Inicializar(janela->renderer);

	int altura, largura;
	largura = mapa.PegaDimensaoemTiles().w;
	altura = mapa.PegaDimensaoemTiles().h;
	
	gerenteAtor.Inicializar(janela);
	int initX, initY;

	ifstream headerfile("resources/maps/"+mapatual+".header", ios_base::binary);
	if(headerfile.is_open()){
		unsigned int size;
		headerfile.read((char*)&initX, sizeof(int));
		headerfile.read((char*)&initY, sizeof(int));
		headerfile.read((char*)&size, sizeof(unsigned int));
		headerfile.read((char*)proxMapa.c_str(), size);
		proxMapa = proxMapa.c_str();
		headerfile.close();
	}

	gerenteAtor.Adicionar(jogador = new Jogador(gerenteAtor, initX, initY));

	ifstream inv("resources/profiles/"+perfil+".inv", ios_base::binary);
	if(inv.is_open()){
		Item** inventario = jogador->PegaInventario();
		Equipamento** equip = jogador->PegaEquipamentos();
		Atributos atributos;
		unsigned int id, id2;
		int slot;
		while(true){
			inv.read((char*)&slot, sizeof(int));
			if(slot < 0){
				break;
			}
			inv.read((char*)&id, sizeof(unsigned int));
			inv.read((char*)&id2, sizeof(unsigned int));
			inv.read((char*)&atributos, sizeof(Atributos));
			if(slot < 10){
				if(id2 == 5){
					inventario[slot] = new Consumavel(janela->renderer, nomes[id2], desc[id2], atributos, id);
				} else {
					inventario[slot] = new Equipamento(janela->renderer, nomes[id2], desc[id2], atributos, id2, id);
				}
			} else {
				equip[slot-10] = new Equipamento(janela->renderer, nomes[id2], desc[id2], atributos, id2, id);
			}
		}
		inv.close();
	}

	ifstream mobfile("resources/maps/"+mapatual+".mob", ios_base::binary);
	if(mobfile.is_open()){
		unsigned int id, qtd;
		int posX, posY;
		Atributos atributos;
		mobfile.read((char*)&qtd, sizeof(unsigned int));
		for(unsigned int i = 0; i < qtd; i++){
			mobfile.read((char*)&id, sizeof(unsigned int));
			mobfile.read((char*)&posX, sizeof(int));
			mobfile.read((char*)&posY, sizeof(int));
			mobfile.read((char*)&atributos, sizeof(Atributos));
			Inimigo* a = 0;
			switch (id)
			{
			case 0:
				gerenteAtor.Adicionar(a = new Lobisomem(gerenteAtor, posX, posY, atributos, jogador, &mapa)); 
				break;
			case 1:
				gerenteAtor.Adicionar(a = new Crowley(gerenteAtor, posX, posY, atributos, jogador, &mapa)); 
				break;
			case 2:
				gerenteAtor.Adicionar(a = new Lucifer(gerenteAtor, posX, posY, atributos, jogador, &mapa)); 
				break;
			default:
				break;
			}			
			boss = a;
		}
		mobfile.close();
	}

	ifstream armfile("resources/maps/"+mapatual+".arm", ios_base::binary);
	if(armfile.is_open()){
		unsigned int id, qtd;
		int posX, posY, dano;
		double dir;
		armfile.read((char*)&qtd, sizeof(unsigned int));
		for(unsigned int i = 0; i < qtd; i++){
			armfile.read((char*)&id, sizeof(unsigned int));
			armfile.read((char*)&posX, sizeof(int));
			armfile.read((char*)&posY, sizeof(int));
			armfile.read((char*)&dano, sizeof(int));
			armfile.read((char*)&dir, sizeof(double));
			gerenteAtor.Adicionar(new Armadilha(gerenteAtor, posX, posY, dir, id, dano));
		}
		armfile.close();
	}
	
	ifstream itemfile("resources/maps/"+mapatual+".item", ios_base::binary);
	if(itemfile.is_open()){
		unsigned int id, id2, qtd;
		int posX, posY;
		Atributos atributos;
		itemfile.read((char*)&qtd, sizeof(unsigned int));
		for(unsigned int i = 0; i < qtd; i++){
			itemfile.read((char*)&id, sizeof(unsigned int));
			itemfile.read((char*)&id2, sizeof(unsigned int));
			itemfile.read((char*)&posX, sizeof(int));
			itemfile.read((char*)&posY, sizeof(int));
			itemfile.read((char*)&atributos, sizeof(Atributos));
			Item* b = 0;
			if(id2 == 5){
				b = new Consumavel(janela->renderer, nomes[id2], desc[id2], atributos, id);
			} else {
				b = new Equipamento(janela->renderer, nomes[id2], desc[id2], atributos, id2, id);
			}
			if(b){
				gerenteAtor.Adicionar(new DropItem(gerenteAtor, b, posX, posY, true));
			}
		}
		itemfile.close();
	}

	Atributos a = jogador->PegaAtributos();
	status.str("");
	status << "HP/HPMax = " << a.hpatual << "/" << a.hp
		<< "\nMP/MPMax = " << a.mpatual << "/" << a.mp
		<< "\nHPRegen/MPRegen = " << a.hpregen << "/" << a.mpregen 
		<< "\nForca = " << a.forca
		<< "\nDefesa = " << a.defesa
		<< "\nMagia = " << a.magia;
	txtstatus.CriaTexturaDoTextoC(janela->renderer, status.str().c_str(), fonte, cor, w);
	skills[0].CriaTexturaDaImagem(janela->renderer, "resources/imgs/1.png");
	skills[1].CriaTexturaDaImagem(janela->renderer, "resources/imgs/2.png");
	skills[2].CriaTexturaDaImagem(janela->renderer, "resources/imgs/3.png");
	estado = ESTADO_INGAME;
	invselecionado = 0;
	janela->entrada.SetaTamanhoTexto(10);
	janela->entrada.ativaInputTexto();
}

void Ingame::Atualizar(Uint32 deltaTime){
	KB_Botao* Teclas = PegaTecla();
	M_Mouse* Mouse = PegaMouse();
	Atributos a = jogador->PegaAtributos();
	stringstream newstatus;
	SDL_Color cor = {0, 0, 0};
	Item** inventario;
	int altura, largura;
	if(deltaTime != 16 && deltaTime != 17){
		printf("%d\t", deltaTime);
	}
	largura = mapa.PegaDimensaoAbsoluta().w;
	altura = mapa.PegaDimensaoAbsoluta().h;
	ofstream profile;
	switch(estado)
	{
	case ESTADO_INGAME:
		camera.x = jogador->PegaBoundingBox().x - camera.w/2;
		camera.y = jogador->PegaBoundingBox().y - camera.h/2;

		if(camera.w < largura){
				if(camera.x < 0)
				camera.x = 0;
			else if(camera.x > largura-camera.w)
				camera.x = largura-camera.w;
		} else {
			camera.x = 0;
		}
		if(altura > camera.h){
			if(camera.y < 0)
				camera.y = 0;
			else if(camera.y > altura-camera.h)
				camera.y = altura-camera.h;
		} else {
			camera.y = 0;
		}

		gerenteAtor.Atualizar(deltaTime, &mapa, &camera);
		
		if(janela->entrada.pegaTexto() == "reload"){
			mapa.Carregar(mapatual);
			mapa.Inicializar(janela->renderer);
			janela->entrada.toggleInputTexto();
			janela->entrada.toggleInputTexto();
		} else if(janela->entrada.pegaTexto() == "godmode"){
			jogador->Godmode();
			janela->entrada.toggleInputTexto();
			janela->entrada.toggleInputTexto();
		} else if(janela->entrada.pegaTexto() == "ghost"){
			jogador->Ghost();
			janela->entrada.toggleInputTexto();
			janela->entrada.toggleInputTexto();
		} else if(janela->entrada.pegaTexto() == "aspirina"){
			jogador->PegaAtributos().hpatual = jogador->PegaAtributos().hp;
			janela->entrada.toggleInputTexto();
			janela->entrada.toggleInputTexto();
		}

		if(Teclas[KB_ESC].pressionado){
			estado = ESTADO_PAUSADO;			
			janela->entrada.toggleInputTexto();
			janela->entrada.toggleInputTexto();
		}
		if(!jogador->EstaNoJogo())
			estado = ESTADO_LOSE;
		if(!boss->EstaNoJogo())
			estado = ESTADO_WIN;
		break;
	case ESTADO_PAUSADO:
		for(int i = 0; i < BOTAO_QTD-2; i++)
			botoes[i].Atualizar(Mouse);				
		newstatus.str("");
		newstatus << "HP/HPMax = " << a.hpatual << "/" << a.hp
				 << "\nMP/MPMax = " << a.mpatual << "/" << a.mp
				 << "\nHPRegen/MPRegen = " << a.hpregen << "/" << a.mpregen 
				 << "\nForca = " << a.forca
				 << "\nDefesa = " << a.defesa
				 << "\nMagia = " << a.magia;
		if(newstatus.str() != status.str()){
			status.str(newstatus.str());
			txtstatus.CriaTexturaDoTextoC(janela->renderer, status.str().c_str(), fonte, cor, janela->PegaPosicaoeTamanho().w);
		}
		if(Teclas[KB_ESC].pressionado || botoes[BOTAO_VOLTAR].Pressionado())
			estado = ESTADO_INGAME;
		if(botoes[BOTAO_INVENTARIO].Pressionado()){
			estado = ESTADO_INVENTARIO;
			invselecionado = 0;
		}
		if(botoes[BOTAO_STATUS].Pressionado()){
			estado = ESTADO_STATUS;
			invselecionado = 0;
		}
	break;
	case ESTADO_STATUS:
		botoes[BOTAO_VOLTAR].Atualizar(Mouse);		
		botoes[BOTAO_USAR2].Atualizar(Mouse);
		newstatus.str("");
		newstatus << "HP/HPMax = " << a.hpatual << "/" << a.hp
				 << "\nMP/MPMax = " << a.mpatual << "/" << a.mp
				 << "\nHPRegen/MPRegen = " << a.hpregen << "/" << a.mpregen 
				 << "\nForca = " << a.forca
				 << "\nDefesa = " << a.defesa
				 << "\nMagia = " << a.magia;
		if(newstatus.str() != status.str()){
			status.str(newstatus.str());
			txtstatus.CriaTexturaDoTextoC(janela->renderer, status.str().c_str(), fonte, cor, janela->PegaPosicaoeTamanho().w);
		}
		inventario = (Item**)jogador->PegaEquipamentos();
		if(inventario[EQUIP_CABECA] && Mouse->x > 600 && Mouse->x < 632 && Mouse->y > 100 && Mouse->y < 132 && Mouse->botoes[M_ESQUERDO].ativo)
			invselecionado = EQUIP_CABECA;
		if(inventario[EQUIP_ARMA] && Mouse->x > 550 && Mouse->x < 582 && Mouse->y > 150 && Mouse->y < 182 && Mouse->botoes[M_ESQUERDO].ativo)
			invselecionado = EQUIP_ARMA;
		if(inventario[EQUIP_TRONCO] && Mouse->x > 600 && Mouse->x < 632 && Mouse->y > 150 && Mouse->y < 182 && Mouse->botoes[M_ESQUERDO].ativo)
			invselecionado = EQUIP_TRONCO;
		if(inventario[EQUIP_PERNAS] && Mouse->x > 650 && Mouse->x < 682 && Mouse->y > 150 && Mouse->y < 182 && Mouse->botoes[M_ESQUERDO].ativo)
			invselecionado = EQUIP_PERNAS;
		if(inventario[EQUIP_PES] && Mouse->x > 600 && Mouse->x < 632 && Mouse->y > 200 && Mouse->y < 232 && Mouse->botoes[M_ESQUERDO].ativo)
			invselecionado = EQUIP_PES;
		if(botoes[BOTAO_USAR2].Pressionado() && inventario[invselecionado])
			inventario[invselecionado]->Usar(jogador);
		if(botoes[BOTAO_VOLTAR].Pressionado())
			estado = ESTADO_PAUSADO;
		break;
	case ESTADO_INVENTARIO:
		inventario = jogador->PegaInventario();
		botoes[BOTAO_USAR].Atualizar(Mouse);		
		botoes[BOTAO_VOLTAR].Atualizar(Mouse);		
		botoes[BOTAO_DESTRUIR].Atualizar(Mouse);		
		newstatus.str("");
		newstatus << "HP/HPMax = " << a.hpatual << "/" << a.hp
				 << "\nMP/MPMax = " << a.mpatual << "/" << a.mp
				 << "\nHPRegen/MPRegen = " << a.hpregen << "/" << a.mpregen 
				 << "\nForca = " << a.forca
				 << "\nDefesa = " << a.defesa
				 << "\nMagia = " << a.magia;
		if(newstatus.str() != status.str()){
			status.str(newstatus.str());
			txtstatus.CriaTexturaDoTextoC(janela->renderer, status.str().c_str(), fonte, cor, janela->PegaPosicaoeTamanho().w);
		}
		for(int i = 0; i < 10; i++){
			if(Mouse->x > 100 && Mouse->x < 700 && Mouse->y > 50.0+i*45.0 && Mouse->y < 90.0+i*45.0 && Mouse->botoes[M_ESQUERDO].ativo)
				invselecionado = i;
		}
		if(botoes[BOTAO_USAR].Pressionado() && inventario[invselecionado])
			inventario[invselecionado]->Usar(jogador);
		if(botoes[BOTAO_DESTRUIR].Pressionado() && inventario[invselecionado]){
			delete inventario[invselecionado];
			inventario[invselecionado] = 0;
		}
		if(botoes[BOTAO_VOLTAR].Pressionado())
			estado = ESTADO_PAUSADO;
		break;
	case ESTADO_WIN:
		botoes[BOTAO_MENUINICIAL].Atualizar(Mouse);
		if(extra){
			extra = false;
			gerenteAtor.Atualizar(deltaTime, &mapa, &camera);
			profile.open("resources/profiles/"+perfil+".player", ios_base::binary);
			if(profile.is_open()){
				unsigned int size = proxMapa.size()+1;
				profile.write((char*)&size, sizeof(unsigned int));
				profile.write((char*)proxMapa.c_str(), size);
				profile.close();
			}
			profile.open("resources/profiles/"+perfil+".inv", ios_base::binary);
			if(profile.is_open()){
				Item** inventario = jogador->PegaInventario();
				Equipamento** equip = jogador->PegaEquipamentos();
				unsigned int id, id2;
				int slot;
				for(int i = 0; i < 10; i++){
					if(inventario[i]){
						slot = i;
						inventario[i]->PegaXY(id, id2);						
						profile.write((char*)&slot, sizeof(int));					
						profile.write((char*)&id, sizeof(unsigned int));
						profile.write((char*)&id2, sizeof(unsigned int));
						profile.write((char*)&inventario[i]->PegaAtributos(), sizeof(Atributos));
					}
				}
				for(int i = 10; i < 15; i++){
					if(equip[i-10]){
						slot = i;
						equip[i-10]->PegaXY(id, id2);						
						profile.write((char*)&slot, sizeof(int));					
						profile.write((char*)&id, sizeof(unsigned int));
						profile.write((char*)&id2, sizeof(unsigned int));
						profile.write((char*)&equip[i-10]->PegaAtributos(), sizeof(Atributos));
					}
				}
				slot = -1;
				profile.write((char*)&slot, sizeof(int));					
				profile.close();
			}
		}
		if(!(proxMapa.size() == 0)){
			botoes[BOTAO_PROX].Atualizar(Mouse);
		}
		break;
	case ESTADO_LOSE:
		botoes[BOTAO_MENUINICIAL].Atualizar(Mouse);
		botoes[BOTAO_RETRY].Atualizar(Mouse);
		if(extra){
			extra = false;
			gerenteAtor.Atualizar(deltaTime, &mapa, &camera);
			profile.open("resources/profiles/"+perfil+".player", ios_base::binary);
			if(profile.is_open()){
				unsigned int size = mapatual.size()+1;
				profile.write((char*)&size, sizeof(unsigned int));
				profile.write((char*)mapatual.c_str(), size);
				profile.close();
			}
			profile.open("resources/profiles/"+perfil+".inv", ios_base::binary);
			if(profile.is_open()){
				Item** inventario = jogador->PegaInventario();
				Equipamento** equip = jogador->PegaEquipamentos();
				unsigned int id, id2;
				int slot;
				for(int i = 0; i < 10; i++){
					if(inventario[i]){
						slot = i;
						inventario[i]->PegaXY(id, id2);						
						profile.write((char*)&slot, sizeof(int));					
						profile.write((char*)&id, sizeof(unsigned int));
						profile.write((char*)&id2, sizeof(unsigned int));
						profile.write((char*)&inventario[i]->PegaAtributos(), sizeof(Atributos));
					}
				}
				for(int i = 10; i < 15; i++){
					if(equip[i-10]){
						slot = i;
						equip[i-10]->PegaXY(id, id2);						
						profile.write((char*)&slot, sizeof(int));					
						profile.write((char*)&id, sizeof(unsigned int));
						profile.write((char*)&id2, sizeof(unsigned int));
						profile.write((char*)&equip[i-10]->PegaAtributos(), sizeof(Atributos));
					}
				}
				slot = -1;
				profile.write((char*)&slot, sizeof(int));					
				profile.close();
			}
		}
		break;
	}
}

void Ingame::Renderizar(){
	M_Mouse* Mouse = PegaMouse();
	Atributos a = jogador->PegaAtributos();
	SDL_Rect hpbar = { 16, 16, (int)((double)a.hpatual/(double)a.hp*100.0), 16};
	SDL_Rect mpbar = { 16, 36, (int)((double)a.mpatual/(double)a.mp*100.0), 16};
	SDL_Rect selectrect;
	Item** inventario;
	mapa.Renderizar(janela->renderer, &camera);
	gerenteAtor.Renderizar(&camera);
	
	if(estado != ESTADO_INGAME)
	{
		filtro.Renderizar(janela->renderer, 0.0, 0.0);
	}
	
	switch(estado)
	{
	case ESTADO_INGAME:	
		SDL_SetRenderDrawColor(janela->renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(janela->renderer, &hpbar);
		SDL_SetRenderDrawColor(janela->renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(janela->renderer, &mpbar);
		hpbar.w = 102;
		hpbar.x = 15;
		mpbar.w = 102;
		mpbar.x = 15;
		SDL_SetRenderDrawColor(janela->renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(janela->renderer, &hpbar);
		SDL_RenderDrawRect(janela->renderer, &mpbar);
		skills[jogador->PegaSkillSelecionada()].Renderizar(janela->renderer, camera.w/2.0, camera.h-50.0, 0, 0, -M_PI/2.0);
		hpbar.x = (int)(camera.w/2.0)-1;
		hpbar.y = camera.h-51;
		hpbar.w = 34;
		hpbar.h = 34;
		SDL_RenderDrawRect(janela->renderer, &hpbar);
		break;
	case ESTADO_PAUSADO:		
		for(int i = 0; i < 5; i++)
			botoes[i].Renderizar(janela->renderer);
		break;
	case ESTADO_INVENTARIO:
		inventario = jogador->PegaInventario();
		for(int i = 0; i < 10; i++){
			if(inventario[i] != 0){
				inventario[i]->PegaIcone().Renderizar(janela->renderer, 100.0, 50.0+i*45.0);
				inventario[i]->PegaTxtNome().Renderizar(janela->renderer, 150.0, 50.0+i*45.0);
				if(i == invselecionado)
					inventario[i]->PegaTxtDesc().Renderizar(janela->renderer, (double)Mouse->x+16.0, (double)Mouse->y);
			}				
		}
		if(inventario[invselecionado]){
			botoes[BOTAO_USAR].Renderizar(janela->renderer);
			botoes[BOTAO_DESTRUIR].Renderizar(janela->renderer);
			SDL_SetRenderDrawColor(janela->renderer, 0, 0, 0, 255);
			selectrect.x = 99;
			selectrect.y = 49+invselecionado*45;
			selectrect.w = 602;
			selectrect.h = 42;
			SDL_RenderDrawRect(janela->renderer, &selectrect);
		}
		botoes[BOTAO_VOLTAR].Renderizar(janela->renderer);
		break;
	case ESTADO_STATUS:
		txtstatus.Renderizar(janela->renderer, 50.0, 50.0);
		inventario = (Item**)jogador->PegaEquipamentos();
		if(inventario[EQUIP_CABECA])
			inventario[EQUIP_CABECA]->PegaIcone().Renderizar(janela->renderer, 600.0, 100.0);
		if(inventario[EQUIP_ARMA])
			inventario[EQUIP_ARMA]->PegaIcone().Renderizar(janela->renderer, 550.0, 150.0);
		if(inventario[EQUIP_TRONCO])
			inventario[EQUIP_TRONCO]->PegaIcone().Renderizar(janela->renderer, 600.0, 150.0);
		if(inventario[EQUIP_PERNAS])
			inventario[EQUIP_PERNAS]->PegaIcone().Renderizar(janela->renderer, 650.0, 150.0);
		if(inventario[EQUIP_PES])
			inventario[EQUIP_PES]->PegaIcone().Renderizar(janela->renderer, 600.0, 200.0);
		if(inventario[invselecionado]){
			inventario[invselecionado]->PegaTxtDesc().Renderizar(janela->renderer, (double)Mouse->x+16.0, (double)Mouse->y);
			botoes[BOTAO_USAR2].Renderizar(janela->renderer);
			SDL_SetRenderDrawColor(janela->renderer, 0, 0, 0, 255);
			switch (invselecionado)
			{
			case EQUIP_CABECA:
				selectrect.x = 599;
				selectrect.y = 99;
				break;
			case EQUIP_ARMA:
				selectrect.x = 549;
				selectrect.y = 149;
				break;
			case EQUIP_TRONCO:
				selectrect.x = 599;
				selectrect.y = 149;
				break;
			case EQUIP_PERNAS:
				selectrect.x = 649;
				selectrect.y = 149;
				break;
			case EQUIP_PES:
				selectrect.x = 599;
				selectrect.y = 199;
				break;
			}
			selectrect.w = 34;
			selectrect.h = 34;
			SDL_RenderDrawRect(janela->renderer, &selectrect);
		}
		botoes[BOTAO_VOLTAR].Renderizar(janela->renderer);
		break;
	case ESTADO_LOSE:
		gameover.Renderizar(janela->renderer, (800.0-(double)gameover.PegaDimensao().w)/2.0, 50.0);
		botoes[BOTAO_MENUINICIAL].Renderizar(janela->renderer);
		botoes[BOTAO_RETRY].Renderizar(janela->renderer);
		break;
	case ESTADO_WIN:
		victory.Renderizar(janela->renderer, (800.0-(double)gameover.PegaDimensao().w)/2.0, 50.0);
		botoes[BOTAO_MENUINICIAL].Renderizar(janela->renderer);		
		if(!(proxMapa.size() == 0)){
			botoes[BOTAO_PROX].Renderizar(janela->renderer);
		}
		break;
	}
}

void Ingame::Finalizar(){	
	TTF_CloseFont(fonte);
}

Tela* Ingame::ProximaTela(){
	if(PegaTecla()[KB_ENCERRA].pressionado || botoes[BOTAO_SAIR].Pressionado())
		return 0;
	if(botoes[BOTAO_PROX].Pressionado() || botoes[BOTAO_RETRY].Pressionado())
		return new Ingame(perfil);
	if(botoes[BOTAO_MENUINICIAL].Pressionado())
		return new MenuInicial();
	return this;
}