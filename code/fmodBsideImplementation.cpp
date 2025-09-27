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
//		This sample shows how B-Side toggling is implemented to allow
//		for unique player/enemy tracks during gameplay.
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
void Music::Update(float dt) 
{
	
	// ...

	// If the music has a bside and the bside is toggle
	if (musicData->bSide && musicData->toggleBSide)
	{
		// Check music toggle direciton
		//	true = up
		//	false = down
		if (musicData->toggleDirection)
		{
			// Adds up to 1 over the toggle time
			musicData->bSideVol += 1.f / musicData->toggleTime * dt;
			if (musicData->bSideVol >= 1.f)
			{
				musicData->toggleBSide = false;
				musicData->bSideVol = 1.f;
			}
		}
		else
		{
			// Adds down to 0 over the toggle time
			musicData->bSideVol -= 1.f / musicData->toggleTime * dt;
			if (musicData->bSideVol <= 0.f)
			{
				musicData->toggleBSide = false;
				musicData->bSideVol = 0.f;
			}
		}
		// Set music volume
		musicData->channel->setVolume(1.f - musicData->bSideVol);
		musicData->bSideChannel->setVolume(musicData->bSideVol);
	}
}

/*!
	Toggles the state of the music from regular to Bside.
 */
void Music::ToggleBSide()
{
	if (!musicData->bSide)
	{
		return;
	}
	musicData->toggleBSide = true;
	musicData->toggleDirection = !musicData->toggleDirection;
}

/*!
	Resets the music to its regular state it it's not already
 */
void Music::ResetBSide()
{
	if (!music->bSide)
	{
		return;
	}
	musicData->toggleBSide = true;
	musicData->toggleDirection = false;
}

// ...
