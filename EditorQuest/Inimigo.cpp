#include "Inimigo.h"
#include "Mapa.h"
#include "GerenteAtor.h"
#include "Jogador.h"
#include "DropItem.h"
#include "Item.h"
#include "Equipamento.h"
#include "Consumavel.h"

Inimigo::Inimigo(GerenteAtor& _gerente, Jogador* _jogador, Mapa* _mapa) : Ator(_gerente)
{
	jogador = _jogador;
	mapa = _mapa;
}

unsigned int Inimigo::PegaId(){
	return id;
}

void Inimigo::FoiAtingido(int dano, unsigned int tipo, SDL_Rect* colisao){
	double d = atan2(y-colisao->y, x-colisao->x);
	if(tipo == 0){
		atributos.hpatual -= (int)(floor((dano*atributos.hp)/(atributos.hp*(1.0+(atributos.defesa/100.0)))));
		x += cos(d)*24;
		y += sin(d)*24;
	}
	else if(tipo == 1){
		atributos.hpatual -= (int)(floor((dano*atributos.hp)/(atributos.hp*(1.0+(atributos.magia/100.0)))));
		x += cos(d)*12;
		y += sin(d)*12;
	}
}

Atributos& Inimigo::PegaAtributos(){
	return atributos;
}

SDL_Rect Inimigo::PegaBoundingBox(){
	SDL_Rect ret = sprite.PegaDimensao();
	ret.x = (int)x;
	ret.y = (int)y;
	return ret;
}

unsigned int Inimigo::PegaTipo(){
	return ATOR_INIMIGO;
}

double Inimigo::PegaDirecao(){
	return direcao;
}

bool Inimigo::EstaNoJogo(){
	return atributos.hpatual > 0;
}

void Inimigo::Colidiu(Ator* ator, SDL_Rect* colisao){

}

void Inimigo::ColidiuMapa(cMap* tile, SDL_Rect* colisao){
	double dx, dy;
	switch (tile->id)
	{
	case 1:
		if (colisao->w > colisao->h)
		{
			dy = (double)(y - tile->rect.y);
			if (dy > 0)
			{
				y += colisao->h;
			}
			else
			{
				y -= colisao->h;
			}
			direcao += M_PI/10.0;
		}
		else
		{
			dx = (double)(x - tile->rect.x);
			if (dx > 0)
			{
				x += colisao->w;
			}
			else
			{
				x -= colisao->w;
			}
			direcao += M_PI/10.0;
		}
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}

bool Inimigo::TemVisaoDoJogador(){
	double dx = jogador->PegaBoundingBox().x - x;
	double dy = jogador->PegaBoundingBox().y - y;
	double dd = sqrt(dx*dx+dy*dy);
	if (visao >= dd){
		cMap* tiles = mapa->PegaColisao();
		unsigned int qtd = mapa->PegaQtdColisao();
		for(unsigned int i = 0; i < qtd; i++){
			if(tiles[i].id == 1){
				int x1 = (int)x+16;
				int y1 = (int)y+16;
				int x2 = jogador->PegaBoundingBox().x+16;
				int y2 = jogador->PegaBoundingBox().y+16;
				if(SDL_IntersectRectAndLine(&tiles[i].rect, &x1, &y1, &x2, &y2) == SDL_TRUE){
					return false;
				}
			}
		}
		return true;
	}
	return false;
}

void Inimigo::Renderizar(SDL_Rect* camera){
	sprite.Renderizar(gerente.janela->renderer, x - (double)camera->x, y - (double)camera->y, 0, 0, direcao);
	SDL_Rect hpbar = { (int)x-camera->x+4, (int)y-8-camera->y, (int)((double)atributos.hpatual/(double)atributos.hp*24.0), 5};
	SDL_SetRenderDrawColor(gerente.janela->renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(gerente.janela->renderer, &hpbar);
	hpbar.w = 26;
	hpbar.x = (int)x-1-camera->x+4;
	SDL_SetRenderDrawColor(gerente.janela->renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(gerente.janela->renderer, &hpbar);
}

void Inimigo::Finalizar(){
	Item* item = 0;
	Atributos temp = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
	switch (rand()%11)
	{
	case 0:
		if(atributos.hp != 0)
			temp.hpatual = temp.hp = rand()%atributos.hp;
		if(atributos.hpregen != 0)
			temp.hpregen = rand()%atributos.hpregen;
		if(atributos.mp != 0)
			temp.mpatual = temp.mp = rand()%atributos.mp;
		if(atributos.mpregen != 0)
			temp.mpregen = rand()%atributos.mpregen;
		if(atributos.forca != 0)
			temp.forca = rand()%atributos.forca;
		if(atributos.defesa != 0)
			temp.defesa = rand()%atributos.defesa;
		if(atributos.magia != 0)
			temp.magia = rand()%atributos.magia;
		item = new Equipamento(gerente.janela->renderer, "Arma", "Uma arma", temp, EQUIP_ARMA, rand()%10);
		break;
	case 1:
		if(atributos.hp != 0)
			temp.hpatual = temp.hp = rand()%atributos.hp;
		if(atributos.hpregen != 0)
			temp.hpregen = rand()%atributos.hpregen;
		if(atributos.mp != 0)
			temp.mpatual = temp.mp = rand()%atributos.mp;
		if(atributos.mpregen != 0)
			temp.mpregen = rand()%atributos.mpregen;
		if(atributos.forca != 0)
			temp.forca = rand()%atributos.forca;
		if(atributos.defesa != 0)
			temp.defesa = rand()%atributos.defesa;
		if(atributos.magia != 0)
			temp.magia = rand()%atributos.magia;
		item = new Equipamento(gerente.janela->renderer, "Capacete", "Um capacete", temp, EQUIP_CABECA, rand()%10);
		break;
	case 2:
		if(atributos.hp != 0)
			temp.hpatual = temp.hp = rand()%atributos.hp;
		if(atributos.hpregen != 0)
			temp.hpregen = rand()%atributos.hpregen;
		if(atributos.mp != 0)
			temp.mpatual = temp.mp = rand()%atributos.mp;
		if(atributos.mpregen != 0)
			temp.mpregen = rand()%atributos.mpregen;
		if(atributos.forca != 0)
			temp.forca = rand()%atributos.forca;
		if(atributos.defesa != 0)
			temp.defesa = rand()%atributos.defesa;
		if(atributos.magia != 0)
			temp.magia = rand()%atributos.magia;
		item = new Equipamento(gerente.janela->renderer, "Peitoral", "Um peitoral", temp, EQUIP_TRONCO, rand()%10);
		break;
	case 3:
		if(atributos.hp != 0)
			temp.hpatual = temp.hp = rand()%atributos.hp;
		if(atributos.hpregen != 0)
			temp.hpregen = rand()%atributos.hpregen;
		if(atributos.mp != 0)
			temp.mpatual = temp.mp = rand()%atributos.mp;
		if(atributos.mpregen != 0)
			temp.mpregen = rand()%atributos.mpregen;
		if(atributos.forca != 0)
			temp.forca = rand()%atributos.forca;
		if(atributos.defesa != 0)
			temp.defesa = rand()%atributos.defesa;
		if(atributos.magia != 0)
			temp.magia = rand()%atributos.magia;
		item = new Equipamento(gerente.janela->renderer, "Calca", "Uma calca", temp, EQUIP_PERNAS, rand()%10);
		break;
	case 4:
		if(atributos.hp != 0)
			temp.hpatual = temp.hp = rand()%atributos.hp;
		if(atributos.hpregen != 0)
			temp.hpregen = rand()%atributos.hpregen;
		if(atributos.mp != 0)
			temp.mpatual = temp.mp = rand()%atributos.mp;
		if(atributos.mpregen != 0)
			temp.mpregen = rand()%atributos.mpregen;
		if(atributos.forca != 0)
			temp.forca = rand()%atributos.forca;
		if(atributos.defesa != 0)
			temp.defesa = rand()%atributos.defesa;
		if(atributos.magia != 0)
			temp.magia = rand()%atributos.magia;
		item = new Equipamento(gerente.janela->renderer, "Sapatos", "Um Par de sapatos", temp, EQUIP_PES, rand()%10);
		break;
	case 5:
		temp.hp = rand()%(atributos.hp+10);
		item = new Consumavel(gerente.janela->renderer, "Pocao de HP", "Uma pocao de vida", temp, POCAO_HP);
		break;
	case 6:
		temp.mp = rand()%(atributos.mp+10);
		item = new Consumavel(gerente.janela->renderer, "Pocao de MP", "Uma pocao de mana", temp, POCAO_MP);
		break;
	case 7:
		temp.hp = rand()%(atributos.hp+10);
		temp.mp = rand()%(atributos.mp+10);
		item = new Consumavel(gerente.janela->renderer, "Pocao Hibrida", "Uma pocao de vida e mana", temp, POCAO_HPMP);
		break;
	case 8:
		temp.hp = rand()%(atributos.hp+2);
		item = new Consumavel(gerente.janela->renderer, "Pocao de HP+", "Uma pocao % de vida", temp, POCAO_RHP);
		break;
	case 9:
		temp.mp = rand()%(atributos.mp+2);
		item = new Consumavel(gerente.janela->renderer, "Pocao de MP+", "Uma pocao % de mana", temp, POCAO_RMP);
		break;
	case 10:
		temp.hp = rand()%(atributos.hp+2);
		temp.mp = rand()%(atributos.mp+2);
		item = new Consumavel(gerente.janela->renderer, "Pocao Hibrida+", "Uma pocao % de vida e mana", temp, POCAO_RHPMP);
		break;
	}
	gerente.Adicionar(new DropItem(gerente, item, x, y));
}
