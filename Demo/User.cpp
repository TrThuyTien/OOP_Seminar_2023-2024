#include "User.h"
#include <iostream>
#include <algorithm>

using namespace std;

User::User()
{
	this->soluong = 1;
	this->Weight = 0;
	this->Height = 0;
	this->luot = -1;
}

void User::truWeight()
{
	int newWeight = (rand() % 60 + 40);
	if (Weight - newWeight >= 100)
	{
		this->Weight -= newWeight;
	}
	else
	{
		this->Weight = 75;
	}
}

int User::getWeight() {
	return Weight;
}

int User::getWeightrandom()
{
	int preWeight = this->Weight;
	this->Weight += (rand() % 70 + 50) * soluong;
	return preWeight;
}


void User::setWeight(int weight)
{
	if (weight >= 0)
		this->Weight = weight;

}


int User::getUser()
{
	return this->soluong;
}

void User::setUser()
{
	this->soluong = 0;
}

void User::subUser()
{
	int currentUser = this->soluong / 2;
	if (currentUser > 0)
		this->soluong -= (rand() % currentUser) + 1;
}

void User::addUser()
{
	this->soluong += 1 + rand() % 10;
}