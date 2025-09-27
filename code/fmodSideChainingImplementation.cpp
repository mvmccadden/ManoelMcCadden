/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Game Name: Cretaceous Clash
// Game Team: Dino Nuggies
// Author: Manoel McCadden, Javier Dantin (27)
// Copyright: Copyright © 2025 DigiPen (USA) Corporation
// 
// Brief: 
//		Handles audio input/output, playing, freeing, and more basic functionality.
//		
//		!!! THIS IS A CODE SAMPLE AND NOT THE WHOLE FILE !!!
//
//		This sample shows how side chaining is implemented to allow for sfx to
//		have more sonic space and cut through the music more prominently.
// 
// Sources:
//	- https://www.fmod.com/docs/2.00/api
//  - GAM-150 FMOD WALKTHROUGH by Professor Ellinger
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

// ...

/*!
	Updates the music object and performs actions such as 
	sidechaining for sound effects and swapping to the b-side on request.

	\param dt
		The time in seconds between the last frame and this frame
 */
void Music::Update(float dt) {
	
	// ...
	
	// This allows for sidechaing when requested
	if (MusicData::sideChainBegan && MusicData::sideChainLenght > 0.0f) {
		MusicData::sideChainLenght -= dt;
	}
	if (MusicData::sideChainBegan && MusicData::sideChainLenght <= 0.0f) {
		float currentVol;
		MusicData::musicGroup->getVolume(&currentVol);
		if (currentVol < MusicData::musicGroupVolume)
		{
			MusicData::musicGroup->setVolume(currentVol + 0.01f);
		}
		else
		{
			MusicData::musicGroup->setVolume(MusicData::musicGroupVolume);
			MusicData::sideChainLenght = 0.0f;
			MusicData::sideChainBegan = false;
		}
	}

	// ...
	
}

/*!
	Sets the amount of time the music will be side-chained.
	This is done to give some sonic space (i.e. for a sfx)
 */
void Music::SideChainMusic(float seconds) {
	MusicData::musicGroup->setVolume(
		MusicData::musicGroupVolume - (MusicData::musicGroupVolume / 2.0f));
	MusicData::sideChainLenght = seconds;
	MusicData::sideChainBegan = true;
}

// ...
