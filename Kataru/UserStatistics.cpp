#include "UserStatistics.h"
#include <stdio.h>
#include <corecrt_malloc.h>

UserStatistics::UserStatistics()
{
	this->userName = (const char**)malloc(sizeof(char[255]));
	this->userScore = (int*)malloc(sizeof(int));

	SetUserScore(0);
	SetUserName("Player 1");
}

UserStatistics::~UserStatistics()
{
}

void UserStatistics::SetUserScore(int score)
{
	*(this->userScore) = score;
}

void UserStatistics::SetUserName(const char* name)
{
	*(this->userName) = name;
}

int UserStatistics::GetUserScore()
{
	return *(this->userScore);
}

const char* UserStatistics::GetUserName()
{
	return *(this->userName);
}
