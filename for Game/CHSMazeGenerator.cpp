#include "CHSMazeGenerator.hpp"

CHSMazeGenerator::CHSMazeGenerator () {

	cRandom.SetSeedFromLocalTimeEx ();

	WallNumber = 0;
	AisleNumber = 1;
}

CHSMazeGenerator::~CHSMazeGenerator () {
}

void CHSMazeGenerator::SetWallNumber ( int NewWallNumber ) {
	WallNumber = NewWallNumber;
}
int CHSMazeGenerator::GetWallNumber ( void ) {
	return WallNumber;
}

void CHSMazeGenerator::SetAisleNumber ( int NewAisleNumber ) {
	AisleNumber = NewAisleNumber;
}
int CHSMazeGenerator::GetAisleNumber ( void ) {
	return AisleNumber;
}

bool CHSMazeGenerator::InnerGenerate ( CHSMazeMap *pMap , unsigned int x , unsigned int y ) {
	if ( pMap == nullptr )return false;
	if ( pMap->IsInitialized () == false ) return false;

	int Directions [ 4 ] = { 0 , 1 , 2 , 3 };

	cRandom.Shuffle ( Directions , 4 );
	
	for ( int i = 0; i < 4; i++ ) {

		switch ( Directions[i] ) {

			//ã
			case 0:
				if ( y > 2 ) {
					if ( pMap->GetValue ( x , y - 2 ) == WallNumber ) {
						pMap->SetValue ( x , y - 1 , AisleNumber );
						pMap->SetValue ( x , y - 2 , AisleNumber );
						InnerGenerate ( pMap , x , y - 2 );
					}
				}
				break;

			//‰º
			case 1:
				if ( pMap->GetValue ( x , y + 2 ) == WallNumber ) {
					pMap->SetValue ( x , y + 1 , AisleNumber );
					pMap->SetValue ( x , y + 2 , AisleNumber );
					InnerGenerate ( pMap , x , y + 2 );
				}
				break;

			//¶
			case 2:
				if ( x > 2 ) {
					if ( pMap->GetValue ( x - 2 , y ) == WallNumber ) {
						pMap->SetValue ( x - 1 , y , AisleNumber );
						pMap->SetValue ( x - 2 , y , AisleNumber );
						InnerGenerate ( pMap , x - 2 , y );
					}
				}
				break;

			//‰E
			case 3:
				if ( pMap->GetValue ( x + 2 , y ) == WallNumber ) {
					pMap->SetValue ( x + 1 , y , AisleNumber );
					pMap->SetValue ( x + 2 , y , AisleNumber );
					InnerGenerate ( pMap , x + 2 , y );
				}
				break;
			default:
				break;
		}

	}

	return true;
}

bool CHSMazeGenerator::Generate ( CHSMazeMap *pMap ) {
	if ( pMap == nullptr )return false;
	if ( pMap->IsInitialized () == false ) return false;
	if ( WallNumber == AisleNumber ) return false;
	unsigned int spone_x; 
	
	do {
		spone_x = cRandom.RangeNext ( 1 , pMap->GetWidth () - 2 );
	} while ( ( spone_x % 2 ) == 0 );


	unsigned int spone_y;
	do {
		spone_y = cRandom.RangeNext ( 1 , pMap->GetHeight () - 2 );
	} while ( ( spone_y % 2 ) == 0 );


	pMap->FillValue ( WallNumber );
	int ercode = pMap->GetErrorReturnValueInt ();

	while ( ( ercode == WallNumber ) || ( ercode == AisleNumber ) ) {
		ercode = cRandom.Next ();
		pMap->SetErrorReturnValueInt ( ercode );
	}

	return this->InnerGenerate ( pMap , spone_x , spone_y );
}
