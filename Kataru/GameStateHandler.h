#pragma once

class GameStateHandler
{
public:
	GameStateHandler();
	~GameStateHandler();

	enum class GameState 
	{
		Startup,
		Menu,
		Game,
		Quit
	};

	void GetGamestate(GameState* gameState);
	void SetGamestate(GameState gameState);

private:
	GameState gameState;
};