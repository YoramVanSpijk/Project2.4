#pragma once
class UserStatistics
{
public:
	UserStatistics();
	~UserStatistics();

	void SetUserScore(int score);
	void SetUserName(const char* name);

	int GetUserScore();
	const char* GetUserName();

private:
	int* userScore = 0;
	const char** userName;
};

