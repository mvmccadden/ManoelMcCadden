/*!
 *  \author Manoel McCadden
 *  \date   26-20-03
 *  \file   Reverb.cpp
 *
 *  \brief
 *    A snippet of my reverb file for my JUCE Moore Reverb application.
 *
 *    Includes Moore Reverb process and All Pass Filtering.
 */

// ....

/*!
 *  Apply an allpass filter to the given audioBuffer that contains samples
 *
 *  \param audioBuffer 
 *    The sample data that is getting an AllPass filter applied to it
 *
 *  \param a
 *    The allpass coefficent passed for the allpass filter
 *
 *  \returns
 *    The modified sample data with an applied allpass filter
 */
juce::AudioBuffer<float> &Apply_Allpass(juce::AudioBuffer<float> &audioBuffer
    , float a, int delay)
{
  float prevInput = 0.f;
  float prevOutput = 0.f;

  juce::AudioBuffer<float> inputBuffer(audioBuffer);

  int numOfChannels = audioBuffer.getNumChannels();
  int numOfSamples = audioBuffer.getNumSamples();

  for(int i = 0; i < numOfSamples * numOfChannels; ++i)
  {
    int channel = i % numOfChannels;
    int sample = i / numOfChannels;
    int prevSample = i - delay;

    float input = audioBuffer.getSample(channel, sample);

    if(prevSample >= 0)
    {
      prevInput = inputBuffer.getSample(channel, prevSample);
      prevOutput = audioBuffer.getSample(channel, prevSample);
    }

    float output = a * (input - prevOutput) + prevInput;

    audioBuffer.setSample(channel, sample, output);
  }

  return audioBuffer;
}

// ....

/*!
 *  Applies MooreReverb to a given object of WaveData
 *
 *  \param waveData
 *    The WaveData object which will have MooreReverb applied, this object will
 *    be updated and changed
 */
WaveData &Apply_MooreReverb_Filter(WaveData &waveData)
{

  // Setup impulse response data
  juce::AudioBuffer<float> outputBuffer = waveData.get_audio_buffer_copy();
  juce::AudioBuffer<float> dryBuffer = waveData.get_audio_buffer_copy();

  // Sum the K and allpass buffers
  float K = 9.f * publicParams.dryWet;
  dryBuffer = Scale_Audio_Buffer(dryBuffer, K);

  // Inital delay calculation values
  const int delayCount = 6;
  double sampleRate = waveData.get_sampling_rate();
  // For interpolation between 25kHz and 50kHz
  double gRatio = (sampleRate / 1000.0 - 25.0) / 25.0;

  // Loop through and create 6 lowpass comb filters that will be summed
  for(int i = 0; i < delayCount; ++i)
  {
    juce::AudioBuffer<float> tempBuffer = waveData.get_audio_buffer_copy();

    int L = (int)((publicParams.initalDelay 
      + publicParams.delayInc[i] * publicParams.delaySpread) 
      * 0.001f * sampleRate);
    
    // Lerp between g50 and g25 to get value relative to current sampling rate
    if(sampleRate > 50000.0 || sampleRate < 25000.0)
    {
      std::cout 
        << 
        "Sampling rate is outside of valid rates, may cause errors" 
        << 
        std::endl;
    }
    
    double g = (publicParams.initalg25 
      + publicParams.g25Inc[i] * publicParams.gSpread)
      + ((publicParams.initalg25 + publicParams.gDiff 
      + publicParams.g50Inc[i] * publicParams.gSpread)
      - (publicParams.initalg25 
      + publicParams.g25Inc[i] * publicParams.gSpread))
      * gRatio;
    float R = (float)((1.0 - g) * 0.83);

    // Just some logging :)
    std::cout 
      << "Gain: " << g 
      << ", R: " << R 
      << ", Delay: " << L 
      << std::endl;

    tempBuffer = Apply_LowpassComb(tempBuffer, L, g, R);
    tempBuffer = Scale_Audio_Buffer(tempBuffer, 1.0 - publicParams.dryWet);
    outputBuffer = Sum_Audio_Buffers(outputBuffer, tempBuffer);
  }

  // Apply the allpass to the summed lowpass-comb filters
  float a = publicParams.allpassCoefficent;
  float m = publicParams.allpassDelay;
  outputBuffer = Apply_Allpass(outputBuffer, a, m);

  // Sum dry and wet buffer
  outputBuffer = Sum_Audio_Buffers(outputBuffer, dryBuffer);
  
  // Normalize the output to -1.5dB
  float dB = -1.5;
  outputBuffer = Normalize_Audio_Buffer(outputBuffer, dB);

  // Apply new buffer to waveData
  waveData.get_audio_buffer() = outputBuffer;
  std::cout << "Applied Reverb" << std::endl;

  return waveData;
};

