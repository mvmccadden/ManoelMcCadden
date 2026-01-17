/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   scene.cpp
 *
 *  \brief
 *    Implementation filter application ontop given wave files based on a scenes
 *    room and settings
 */

/*!
 *  Uses a given scene filter to digitally process the given .wav file
 *
 *  \param wave
 *    A user given .wav file that will be processed and have the scene's filter
 *    applied to it
 */
void Scene::apply_filter_to_wave(WaveFile &wave)
{
  // If the incoming sampling rate is different than the currently used sampling
  // rate for the scene or the filter is empty, then we must attempt to generate
  // a new filter for the given .wav file
  unsigned incomingSamplingRate = wave.get_sampling_rate();
  if((filters.size() == 0) || currentSamplingRate != incomingSamplingRate)
  {
    if(filters.size() > 0)
    {
      filters.clear();
    }
    currentSamplingRate = incomingSamplingRate;
    generate_scene_filter();
  }

  // Update the output (even if the room is empty) based on the scenes filter
  CArray<float> output;
  for(size_t i = 0; i < filters.size(); ++i)
  {
    CArray<float> input(wave.get_samples());
    filters[i].apply_filter(input);
    output += input;
  }
  wave.get_samples() = output;
}

/*!
 *  Generates a new filter based on the scene's room and it's settings
 */
void Scene::generate_scene_filter()
{
  // Remove all equalizers that may be present within the filter
  filters.clear();

  // Resize the filter to match the size of the number of AudioRays
  filters.resize(audioRayVec.size());

  // Use each audio ray to calculate a delay and set of absorbiton coefficents
  // for each given band to create equalizers based on each given rays path
  size_t i = 0;
  for(vector<AudioRay *> &audioRays : audioRayVec)
  {
    float distance = 0.f;
    float scalar = (relativeScalar.x > relativeScalar.y) 
      ? relativeScalar.x : relativeScalar.y;

    // NOTE: each pixel is a centimeter in this simulation
    for(AudioRay *audioRay : audioRays)
    { 
      distance += audioRay->get_distance() / scalar;
    }
    unsigned delay = distance / 34300.f * currentSamplingRate;

    const CArray<Vec2> &array = audioRays.back()->get_amp();
    filters[i] = Equalizer(array.size(), currentSamplingRate, delay);

    for(size_t j = 0; j < array.size(); ++j)
    {
      // Divide the coefficent by the number of rays to ensure it doesn't get
      // too loud and go outside the bounds of [-1, 1)
      filters[i].add_coefficent(array.at(j).x, array.at(j).y / audioRays.size()
          , static_cast<unsigned>(j));
    }

    ++i;
  }
}

/*! 
 *  Applies a filter based on the Sabine forumla and using Schroeder reverb to
 *  generate an estimation of reverberance within the given scene's room.
 *
 *  NOTE: This does not account for microphone properties or speaker settings
 */
void Scene::apply_t60_to_wave(WaveFile &wave)
{
  // Update the sampling rate as needed
  if(currentSamplingRate != wave.get_sampling_rate())
  {
    currentSamplingRate = wave.get_sampling_rate();
  }

  // Using an audio ray to track absorbtion as it has all the built in
  // functionality needed
  AudioRay absorbtionRay(nullptr, 0, Barrier::get_coefficent(Barrier::C_WALL)
      , {0.f, 0.f}, {0.f, 0.f});
  absorbtionRay.scale_amp(2.f * relativeSize.x 
      + 2.f * relativeSize.y);

  float area = relativeSize.x * relativeSize.y;

  for(Object *object : objects)
  {
    Vec2 size = object->get_size();
    absorbtionRay.scale_and_add_to_amp(object->get_absortion_coefficent()
        , 2.f * size.x + 2.f *size.y);
  }

  // NOTE: Logs the amplitude of the ray which contains all objects absorbtion
  // coefficents combined
  for(size_t i = 0; i < absorbtionRay.get_amp().size(); ++i)
  {
    Logger(Logger::L_MSG, "T60 AR" + to_string(i) + ": " 
        + to_string(absorbtionRay.get_amp().at(i).y));
  }

  // NOTE: Logs the average of all bands
  Logger(Logger::L_MSG, "T60 AMP AVERAGE: " 
      + to_string(absorbtionRay.get_amp_average()));

  CArray<Vec2> bands(absorbtionRay.get_amp());
  for(size_t i = 0; i < bands.size(); ++i)
  {
    /* 
     * e^(-6.908)/T60
     * -6.908 is the ln(1000) which represents the drop to -60dB which is
     * 1/1000th the amplitude of the original signal.
     */
    bands[i].y = exp((-6.908f) 
        / ((0.161f * area) / (bands[i].y)));
    Logger(Logger::L_MSG, "T60 BAND " + to_string(i) + ": " 
        + to_string(bands[i].y));
  }


  // NOTE: Each pixel is a centimeter and delay is in ms and uses the 
  // larger wall distance to calculate the ms delay
  uint16_t delayTime = (relativeSize.x > relativeSize.y)
    ? relativeSize.x / 34300.f * wave.get_sampling_rate()
    : relativeSize.y / 34300.f * wave.get_sampling_rate();

  CArray<float> output;
  const size_t delayCount = 10;
  // Generate the coprimes for a realistic sound
  CArray<uint16_t> delays = generate_nearest_coprimes(delayTime, delayCount);
  for(size_t i = 0; i < delayCount; ++i)
  {
    static_cast<void>(Logger(Logger::L_MSG, "T60 DelayTime " + to_string(i)
          + ": " + to_string(delays[i])));

    CArray<float> input(wave.get_samples());
    
    // Generate the bands and equalizer
    size_t bandSize = bands.size();
    Equalizer t60EQ(bandSize, currentSamplingRate, delays[i]);
    for(size_t j = 0; j < bandSize; ++j)
    {
      t60EQ.add_coefficent(bands[j].x, bands[j].y / delayCount, j);
    }

    t60EQ.apply_filter(input);
    output += input;
  }

  // Overwrite samples from output into wave object
  wave.get_samples() = output;
}

/* T60 Explanation Info
 * 
 *  Equation:
 *    A(t) = A_0 * e^((-6.908 * t)/T60)
 *
 *    We will use a mix of comb and lowpass filters to create artifical
 *    reverberation. To avoid robotic sounding delay we will use Schroeder
 *    Reverb with 10 (originally 4) parallel delay lines with coprime delay 
 *    lengths
 *
 *    T60 is taken from the Sabine formula for RT_60 = (0.161 * V) / A
 *    V = room volume in m^3
 *    A = total absorbtion area in Sabins (the sum of all surface areas
 *      multiplied by their respective absortion coefficients)
 *
 *    We then move this into 2D space by replace V with A2 and A with P
 *    A2 = room area in m^2
 *    P = perimeter of all surfaces multiplied by their respective abosorbtion
 *      coefficients
 */

