#ifndef OSC_HPP_
#define OSC_HPP_

#include <array>

class Osc {
  int portid; // clap port index
  int midi_channel;
  int midi_key;
  int node_id; // node_id is delivered by the host

  float sampleRate{0};
  float L{0.f}, R{0.f}; // Left Right channel on output
  enum ASRMode
    {
        OFF,
        ATTACK,
        HOLD,
        RELEASING
    } state{OFF};
 public:
  void start(int key);
  void step();
  void release();
  inline bool isPlaying() const { return state != OFF && state != NEWLY_OFF; }
 private:
  double baseFreq{440.0};
  float time{0};
};

#endif // OSC_HPP_
