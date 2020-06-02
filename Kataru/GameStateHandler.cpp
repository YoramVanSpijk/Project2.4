#include "GameStateHandler.h"

GameStateHandler::GameStateHandler()
{
	this->gameState = GameState::Startup;
}

GameStateHandler::~GameStateHandler()
{

}

void GameStateHandler::GetGamestate(GameState* gameState)
{
	*gameState = this->gameState;
}

void GameStateHandler::SetGamestate(GameState gameState)
{
	this->gameState = gameState;
}
