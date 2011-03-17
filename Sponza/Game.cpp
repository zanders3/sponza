#include "stdafx.h"
#include "Game.h"

#include "Content/ContentIDs.h"
#include "Content/ContentLoader.h"
#include "..\ModelProcessor\ModelContent.h"

Game::Game() :
	m_content("..\\Content\\Out")
{
	content::Model* model = m_content.Get<content::Model>(ContentID::SPONZA_ATRIUM);
}

Game::~Game()
{
}

void Game::LoadContent( ID3D10Device* pd3dDevice, int width, int height )
{
}

void Game::Render( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime )
{
}

void Game::Update( double fTime, float fElapsedTime )
{
}