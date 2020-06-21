#pragma once
class UserStatistics
{
public:
	UserStatistics();
	~UserStatistics();

	void SetUserScore(int score);
	void SetUserName(char name[255]);

	int GetUserScore();
	const char* GetUserName();

private:
	int* userScore = 0;
	char userName[255] = {};
};

