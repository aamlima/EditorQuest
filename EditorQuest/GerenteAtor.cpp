#include "GerenteAtor.h"
#include "Ator.h"
#include "Mapa.h"

using namespace std;

void GerenteAtor::Inicializar(Janela* _janela){
	janela = _janela;
}

void GerenteAtor::Adicionar(Ator* ator)
{
	adicionados.push_back(ator);
	ator->Inicializar();
}

void GerenteAtor::Atualizar(Uint32 deltaTime, Mapa* mapa, SDL_Rect* camera)
{
	//1. Excluir os atores na lista excluido
	//para libera-la. 
	for (Ator* ator : excluidos)
	{
		delete ator;
	}
	excluidos.clear();

	//2. Mover os mortos para a lista de exclu�dos
	//para liberar a lista de mortos.
	for (Ator* ator : mortos)
	{
		excluidos.push_back(ator);
	}
	mortos.clear();

	//Adiciona todos os atores que foram adicionados no loop anterior
	//e coloca-los na lista de atores.
	for (Ator* ator : adicionados)
	{
		atores.push_back(ator);
	}
	adicionados.clear();

	//3. Atualiza a l�gica do ator e, caso queira sair do jogo
	//finaliza e move para a lista de mortos. Caso contr�rio,
	//mant�m o ator numa lista de vivos.
	if(atores.empty())
		return;
	std::vector<Ator*> vivos;

	for (Ator* ator : atores) 
	{
		ator->Atualizar(deltaTime, camera);
		if (ator->EstaNoJogo())
		{
			vivos.push_back(ator);
		}
		else
		{
			ator->Finalizar();
			mortos.push_back(ator);
		}
	}

	//Substitui a lista de atores pela de vivos.
	swap(atores, vivos);

	//Checa colis�o com o mapa
	if(atores.size() < 1)
		return;
	SDL_Rect rcolisao;
	if(mapa != 0){
		cMap* tiles = mapa->PegaColisao();
		for(Ator* ator : atores)
		{
			if(ator->PegaTipo() == ATOR_ARMADILHA)
			{
				continue;
			}
			for(unsigned int i = 0; i < mapa->PegaQtdColisao(); i++)
			{
				if(SDL_IntersectRect(&ator->PegaBoundingBox(), &tiles[i].rect, &rcolisao) == SDL_TRUE)
				{
					ator->ColidiuMapa(&tiles[i], &rcolisao);
				}
			}
		}
	}

	//Checa colis�o entre atores
	if(atores.size() < 2)
		return;
	for(unsigned int i = 0; i < atores.size() - 1; i++)
	{
		if(atores[i]->PegaTipo() == ATOR_ARMADILHA)
		{
			continue;
		}
		for(unsigned int j = i + 1; j < atores.size(); j++)
		{
			if(atores[j]->PegaTipo() == ATOR_ARMADILHA)
			{
				continue;
			}
			if(SDL_IntersectRect(&atores[i]->PegaBoundingBox(), &atores[j]->PegaBoundingBox(), &rcolisao) == SDL_TRUE){
				atores[i]->Colidiu(atores[j], &rcolisao);
				atores[j]->Colidiu(atores[i], &rcolisao);
			}
		}
	}
}


void GerenteAtor::Renderizar(SDL_Rect* camera)
{
	if(!atores.empty())
		for (Ator* ator : atores) 
		{
			ator->Renderizar(camera);
		}
}

Ator* GerenteAtor::PegaAtormaisProximo(double x, double y){
	Ator* ret = 0;
	double dx, dy, d;
	d = 9999.0;
	if(!atores.empty())
		for (Ator* ator : atores) 
		{
			dx = x - ator->PegaBoundingBox().x;
			dy = y - ator->PegaBoundingBox().y;
			double dr = sqrt(dx*dx+dy*dy);
			if(d > dr)
			{
				ret = ator;
				d = dr;
			}
		}
	return ret;
}

Ator* GerenteAtor::PegaAtormaisProximo(double x, double y, unsigned int tipo){
	Ator* ret = 0;
	double dx, dy, d;
	d = 9999.0;
	if(!atores.empty())
		for (Ator* ator : atores) 
		{
			if(ator->PegaTipo() == tipo)
			{
				dx = x - ator->PegaBoundingBox().x;
				dy = y - ator->PegaBoundingBox().y;
				double dr = sqrt(dx*dx+dy*dy);
				if(d > dr)
				{
					ret = ator;
					d = dr;
				}
			}
		}
	return ret;
}

/*
Ator* GerenteAtor::PegaPrimeiroAtornaLista(unsigned int tipo){
	if(!atores.empty())
		for (Ator* ator : atores) 
		{
			if(ator->PegaTipo() == tipo)
			{
				return ator;
			}
		}
	return 0;
}
*/

GerenteAtor::~GerenteAtor()
{
	for (Ator* ator : atores) 
	{
		delete ator;
	}
}
