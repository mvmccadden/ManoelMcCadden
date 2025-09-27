/*
 *  author: Manoel McCadden
 *  date:   06-23-25
 *  file:   waveshape.js
 *  details:
 *    The main file that handles combining all the wave components together
 *    and converting position data to wave data.
 *
 *    NOTE: This is a sample of the file that has been simplified for 
 *    readers convience.
 *  
 *    This Sample demonstrates the WaveShape class, how it is extended to make
 *    specific shapes such as SineWaves, and how the position of each WaveShape
 *    is used to determine it's frequency and amplitude.
 */

// ...

// Used to set the min and max freq and min and max amp avaliable
const MAX_FREQ = 11000;
const MIN_FREQ = 15;
const MAX_LOG_FREQ = Math.log(MAX_FREQ);
const MIN_LOG_FREQ = Math.log(MIN_FREQ);
const MAX_AMP = 1;
const MIN_AMP = 0;

// ...

/*!
 *  Constructs a new base wave shape that can be drawn, played, 
 *  and collided with.
 */
class WaveShape {
  static waveID = 0;

  static IncrementWaveID() {
    return WaveShape.waveID++;
  }

  constructor([x,y], size, waveType) {
    this.x = x;
    this.y = y;
    this.size = size;

    this.shape = new Shape([0,0], 0, [0,0,0,0]);
    this.collider = new Collider(this.shape);

    this.osc = new Osc(waveType);
  
    // A toggle flag used to turn on and off sustain
    this.sustain = false;

    // A selection variable used to connect objects to the mouse movement
    // in order to move them around the plane
    this.selected = false;

    this.latestNote = MIDI_NOTES[0];
    this.octave = Math.round(x / (windowSize.x / TOTAL_OCTAVES) - MAX_OCTAVE);

    this.id = WaveShape.IncrementWaveID();
  }

  // ...

  Select() {
    // Set this to the selected wave and unselected wave if one is selected
    if(selectedWave != 'undefined') {
      selectedWave.Unselect();
    }
    selectedWave = this;
    this.selected = true;
    // Add the last playing note to the playing list to ensure it stays 
    // playing if sustain is active
    if(this.sustain == true) {
      playingNotes.push(this.latestNote);
    }
  }

  Unselect() {
    selectedWave = 'undefined';
    this.selected = false;
    if(this.sustain == false) {
      this.Stop();
    }
  }

  // ...

  Update() {
    // Update shape position and size as needed
    this.shape.SetPos([this.x, this.y]);
    this.shape.SetSize(this.size);

    // Get the positions shape and convert to a freq and amp for the osc
    const [x,y] = this.shape.Pos
    // We use the log freqeucnies to move it to the correct location on 
    // screen relative to a logrithmic scale and then exponent it
    this.osc.SetFreq(Math.exp(constrain(map(x, 0, windowSize.x, MIN_LOG_FREQ
      , MAX_LOG_FREQ), MIN_LOG_FREQ, MAX_LOG_FREQ), 0.1));
    this.osc.SetAmp(constrain(map(y, windowSize.y, 0, MIN_AMP, MAX_AMP)
      , MIN_AMP, MAX_AMP), 0.1);
  }
}

/*
 *  An extension of the WaveShape class that does the grunt work of setting up a
 *  SineWave for you.
 *
 *  NOTE: All wave types were constructed through extensions but ther rest were 
 *  left out as they are similar to the SineWave in construction.
 *
 */
class SineWave extends WaveShape {
  constructor([x,y], size) {
    // Call the original ctor
    super([x,y], size, 'sine');

    this.shape = new Circle([x,y], size
      , [SINE_COLOR.r, SINE_COLOR.g, SINE_COLOR.b, SINE_COLOR.a]);
    this.collider = new CircleCollider(this.shape);
  }
}

// ...

