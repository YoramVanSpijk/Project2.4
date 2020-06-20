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
		Calibration,
		Game,
		GameOver,
		Quit
	};

	void GetGamestate(GameState* gameState);
	void SetGamestate(GameState gameState);

private:
	GameState gameState;
};