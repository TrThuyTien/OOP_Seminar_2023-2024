#include <cstdlib> 
#include <ctime> 
#include <iostream>

using namespace std;

#pragma once
class User
{
private:

	int Weight;
	int Height;
public:
	int soluong;
	int luot;
	User();
	int getWeight();
	int getWeightrandom();
	void truWeight();
	void setWeight(int weight);
	void subUser();
	void addUser();
	int getUser();
	void setUser();
};