#include "CHSMazeMap.hpp"

CHSMazeMap::CHSMazeMap () {
}

CHSMazeMap::~CHSMazeMap () {
}



bool CHSMazeMap::Initialize ( unsigned int Width , unsigned int Height ) {
	if ( ( Width % 2 ) == 0 )return false;
	if ( ( Height % 2 ) == 0 )return false;
	if ( Width < 5 ) return false;
	if ( Height < 5 ) return false;

	return this->InnerInitialize ( Width , Height );
}

bool CHSMazeMap::Resize ( unsigned int Width , unsigned int Height , bool bTakeOver ) {
	if ( ( Width % 2 ) == 0 )return false;
	if ( ( Height % 2 ) == 0 )return false;
	if ( Width < 5 ) return false;
	if ( Height < 5 ) return false;
	return this->InnerResize ( Width , Height , bTakeOver );
}

