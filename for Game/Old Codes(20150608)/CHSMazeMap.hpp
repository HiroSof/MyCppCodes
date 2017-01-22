#include "CHSMap.hpp"

#pragma once


class CHSMazeMap : public CHSMap{
private:
public:

	CHSMazeMap ();
	~CHSMazeMap ();

	bool Initialize ( unsigned int Width , unsigned int Height );
	bool Resize ( unsigned int Width , unsigned int Height , bool bTakeOver );


};


