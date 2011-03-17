#include "stdafx.h"
#include "Game.h"

#include "Content/ContentIDs.h"
#include "Content/ContentLoader.h"
#include "Graphics/Model.h"

Game::Game() :
	m_content("..\\Content\\Out")
{
}

Game::~Game()
{
}

void Game::LoadContent( ID3D10Device* pd3dDevice, int width, int height )
{
	m_content.SetDevice(pd3dDevice);

	Model* model = m_content.Get<Model>(ContentID::SPONZA_ATRIUM);
}

void Game::Render( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime )
{
}

void Game::Update( double fTime, float fElapsedTime )
{
}