/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Game Name: Cretaceous Clash
// Game Team: Dino Nuggies
// Author: Manoel McCadden, Javier Dantin (27)
// Copyright: Copyright © 2025 DigiPen (USA) Corporation
// 
// Brief: 
//		Handles audio input and output, playing, freeing, and more basic 
//		functionality
//		
//		!!! THIS IS A CODE SAMPLE AND NOT THE WHOLE FILE !!!
//
//		This sample shows how MusicData is created and read in for a 
//		Music class object.
// 
// Sources:
//	- https://www.fmod.com/docs/2.00/api
//  - GAM-150 FMOD WALKTHROUGH by Professor Ellinger
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "Entity_Music.h"

#include "System_Audio.h"
#include "Stream.h"

/*!
 *  \struct MusicData
 *
 *  \brief
 *    A container for holding all the music date to ensure that
 *    it is only accessible within the internal music implementation
 */
struct MusicData {
public:
	static FMOD::ChannelGroup* musicGroup;
	FMOD::Sound* music = nullptr;
	FMOD::Sound* bSide = nullptr;
	float bSideVol = 0.f;
	bool playing;
	bool looping;
	bool toggleBSide = false;
	bool toggleDirection = false;
	// The beat where the music will loop from
	float loopingPoint;
	// The bpm of the music
	float bpm;
	// The beats per seconds
	float beatInterval;
	// counts the passed beats
	unsigned long long beatCounter = 0u;
	// The sample rate of the music file
	static float musicGroupVolume;
	static float sideChainLenght;
	static bool sideChainBegan;
	static int sampleRate;
	static unsigned long long dspClock;
	static float toggleTime;
	FMOD::Channel* channel = nullptr;
	FMOD::Channel* bSideChannel = nullptr;
	std::string name = "";
	std::string path = "";

	MusicData() : music(nullptr), playing(false)
		, channel(nullptr)
	{
	}
};


FMOD::ChannelGroup* MusicData::musicGroup = nullptr;
bool MusicData::sideChainBegan = false;
float MusicData::sideChainLenght = 0.0f;
float MusicData::musicGroupVolume = 1.0f;
float MusicData::toggleTime = 1.0f;
int MusicData::sampleRate = 0;
unsigned long long MusicData::dspClock = 0u;


/*!
	Creates music data for a given instance of the Music class
	
	\param _name
		The name of the Music object
	\param path
		The file path to the music
	\param looping
		Tells us if the music loops or not
	\param loopingPoint
		Defines the point that music starts looping from in beats
	\param bpm
		The bpm of the music
	\param bSidePath
		The file path to the bSide of the track if it has one
 */
void Music::CreateMusicData(const std::string& _name
	, const std::string& path, const bool& looping
	, const float& loopingPoint, const float& bpm
	, const std::string &bSidePath) {
	musicData->name = _name;

	// Ensure the given path is not empty
	if (path == "") {
		std::cout << "CREATED NO MUSIC PATH";
		return;
	}

	musicData->looping = looping;
	// Convet looping point a second value based on bpm
	musicData->loopingPoint = loopingPoint / bpm * 60.f;
	musicData->path = path;
	musicData->bpm = bpm;
	musicData->beatInterval = 60.f / bpm;

	musicData->music = System_Audio::GetMusicByName(musicData->name);

	// If the music has already been created then ignore the
	//	rest of the function and return
	if (musicData->music)
	{
		if (!bSidePath.empty())
		{
			musicData->bSide = System_Audio::GetMusicByName(musicData->name + "_B");
		}
		return;
	}

	//	...
}


/*!
	Reads in data from a given json file and updates the music objects with
	new music data.
 */
void Music::ReadData(const json& js) {
	CreateMusicData(js["Name"], js["FilePath"], js["IsLooping"]
		, js["LoopingPoint"], js["Bpm"], js["BSIDE_FilePath"]);
}

// ...
