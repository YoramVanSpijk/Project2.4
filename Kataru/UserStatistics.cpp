#include "UserStatistics.h"
#include <stdio.h>
#include <corecrt_malloc.h>
#include <string.h>

UserStatistics::UserStatistics()
{
	this->userScore = (int*)malloc(sizeof(int));

	SetUserScore(0);
	SetUserName((char*)"Player 1");
}

UserStatistics::~UserStatistics()
{
	//delete this->userName;
	//free(this->userName);
}

void UserStatistics::SetUserScore(int score)
{
	*(this->userScore) = score;
}

void UserStatistics::SetUserName(char* name)
{
	strcpy(this->userName, name);
}

int UserStatistics::GetUserScore()
{
	return *(this->userScore);
}

const char* UserStatistics::GetUserName()
{
	return this->userName;
}