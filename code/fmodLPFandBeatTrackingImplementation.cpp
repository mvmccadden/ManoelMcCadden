/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Game Name: Cretaceous Clash
// Game Team: Dino Nuggies
// Author: Manoel McCadden, Javier Dantin (27)
// Copyright: Copyright © 2025 DigiPen (USA) Corporation
// 
// Brief: 
//		Handles audio input/output, playing, freeing, and more basic 
//		functionality.
//		
//		!!! THIS IS A CODE SAMPLE AND NOT THE WHOLE FILE !!!
//
//		This sample shows how beats are tracked using FMOD for reactive music
//		elements in game and a LPF is applied to music to enhance pausing.
// 
// Sources:
//	- https://www.fmod.com/docs/2.00/api
//  - GAM-150 FMOD WALKTHROUGH by Professor Ellinger
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

// ...

/*!
	\returns an unsigned value dictating what beat we are on in the music
 */
unsigned Music::GetBeatTrigger()
{
	// retrived the current clock info
	MusicData::musicGroup->getDSPClock(&MusicData::dspClock, 0);

	// Gets the current beat and returns it
	return MusicData::dspClock /
		static_cast<unsigned>(musicData->beatInterval
			* MusicData::sampleRate);
}

/*!
	Toggles the state of the low pass filter on the music channel.
 */
void Music::ToggleMusicLowPass(const bool& toggle)
{
	if (toggle == true)
	{
		MusicData::musicGroup->addDSP(0, System_Audio::GetLowPassFilter());
	}
	else
	{
		MusicData::musicGroup->removeDSP(System_Audio::GetLowPassFilter());
	}
}

// ...
