#pragma once
#include "CHSRandom.hpp"
#include "CHSMazeMap.hpp"



class CHSMazeGenerator {
private:
	CHSRandom cRandom;
	int WallNumber;	//�ǂ������l
	int AisleNumber; //�ʘH�������l

	bool InnerGenerate ( CHSMazeMap *pMap , unsigned int x , unsigned int y );

public:
	CHSMazeGenerator ();
	~CHSMazeGenerator ();

	bool SetWallNumber ( int NewWallNumber );
	int GetWallNumber ( void );

	bool SetAisleNumber ( int NewAisleNumber );
	int GetAisleNumber ( void );
	bool Generate ( CHSMazeMap *pMap );
};


