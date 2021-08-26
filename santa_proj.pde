import ddf.minim.*;
import ddf.minim.analysis.*;
import processing.serial.*;

// BeatListener class from the mentioned Minim example
class BeatListener implements AudioListener
{
  private BeatDetect beat;
  private AudioPlayer source;
  
  BeatListener(BeatDetect beat, AudioPlayer source)
  {
    this.source = source;
    this.source.addListener(this);
    this.beat = beat;
  }
  
  void samples(float[] samps)
  {
    beat.detect(source.mix);
  }
  
  void samples(float[] sampsL, float[] sampsR)
  {
    beat.detect(source.mix);
  }
}

// Set up Minim beat detection classes
Minim minim;
AudioPlayer song;
BeatDetect beat;
BeatListener bl;

// Port for communicating with Arduino
Serial port;

// Numbers to store the detected beat type
float kickSize, snareSize, hatSize;

void setup() {
  // Visual display setup
  size(512, 200, P3D);
  textFont(createFont("Arial", 16));
  textAlign(CENTER);

  // Minim setup
  minim = new Minim(this);
  song = minim.loadFile("eric_nam_meltmyheart.mp3", 2048);
  song.play();
  beat = new BeatDetect(song.bufferSize(), song.sampleRate());
  beat.setSensitivity(10);
  bl = new BeatListener(beat, song);
  
  /*
   * Configure serial port to use the closest one that
   * it recognises.
   */
  port = new Serial(this, Serial.list()[0], 9600);

  // Initialise all beat values at 16
  kickSize = snareSize = hatSize = 16;
}

void draw() {
  // Draw visual stuff
  background(0);
  fill(255);

  kickSize = constrain(kickSize * 0.05, 0, 10);
  snareSize = constrain(snareSize * 0.05, 0, 10);
  hatSize = constrain(hatSize * 0.05, 0, 10);

  if(beat.isKick()) kickSize = 50;
  if(beat.isSnare()) snareSize = 50;
  if(beat.isHat()) hatSize = 50;
  
  // Visual kick code - irrelevant to Arduino
  textSize(kickSize);
  text("KICK", width/4, height/2);
  // Arduino kick code - light LED A and LED B for a kick
  port.write(int(constrain(kickSize, 0, 255)));
  port.write('A');
  port.write(int(constrain(kickSize, 0, 255)));
  port.write('B');

  // Visual snare code - irrelevant to Arduino
  textSize(snareSize);
  text("SNARE", 2*width/4, height/2);
  // Arduino snare code
  port.write(int(constrain(snareSize, 0, 255)));
  port.write('C');
  port.write(int(constrain(snareSize, 0, 255)));
  port.write('D');

  // Visual hat code - irrelevant to Arduino
  textSize(hatSize);
  text("HAT", 3*width/4, height/2);
  // Arduino hat code
  port.write(int(constrain(hatSize, 0, 255)));
  port.write('E');
  port.write(int(constrain(hatSize, 0, 255)));
  port.write('F');
  
  kickSize = constrain(kickSize * 0.5, 16, 32);
  snareSize = constrain(snareSize * 0.5, 16, 32);
  hatSize = constrain(hatSize * 0.5, 16, 32);
}

void stop() {
  port.write('G');
  song.close();
  minim.stop();
  super.stop();
}
