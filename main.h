#ifndef __MAIN_H
#define __MAIN_H
#define pragma once

#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>
#include <fstream>
#include <time.h>
#include <iomanip> 
#include <unistd.h>

clock_t start,stop;
std::vector<int> pTable;	//Prediction Table

void simBimodal(int, char*,int,int);
void simGshare(int, int, char*,int,int);
void simHybrid(int ,int ,int ,int ,char*,int,int);

void printTheOutput(int, int,const char *);

#endif