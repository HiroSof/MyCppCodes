#pragma once
#include "CHSGameRandom.hpp"
#include "CHSMazeMap.hpp"

class CHSMazeGenerator {
private:
	CHSGameRandom cRandom;
	int WallNumber;	//�ǂ������l
	int AisleNumber; //�ʘH�������l

	bool InnerGenerate ( CHSMazeMap *pMap , unsigned int x , unsigned int y );

public:
	CHSMazeGenerator ();
	~CHSMazeGenerator ();

	void SetWallNumber ( int NewWallNumber );
	int GetWallNumber ( void );

	void SetAisleNumber ( int NewAisleNumber );
	int GetAisleNumber ( void );
	bool Generate ( CHSMazeMap *pMap );
};


