#include "nrandom.h"

#include <glm/glm.hpp>

static thread_local uint32_t state_;

static uint32_t pcg_hash(uint32_t input) {
  uint32_t state = input * 747796405u + 2891336453u;
  uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
  return (word >> 22u) ^ word;
}

float RandomValue() {
  state_ = pcg_hash(state_);
  return (float)state_ / (float)std::numeric_limits<uint32_t>::max();
};

inline float fast_log2 (float val) {
   int * const  exp_ptr = reinterpret_cast <int *> (&val);
   int          x = *exp_ptr;
   const int    log_2 = ((x >> 23) & 255) - 128;
   x &= ~(255 << 23);
   x += 127 << 23;
   *exp_ptr = x;

   return (val + log_2);
}

inline float fast_log (const float &val) {
   return (fast_log2 (val) * 0.69314718f);
}

inline float fast_sine(float x) {
    constexpr float PI = 3.14159265358f;
    constexpr float B = 4.0f / PI;
    constexpr float C = -4.0f / (PI * PI);
    constexpr float P = 0.225f;

    float y = B * x + C * x * (x < 0 ? -x : x);
    return P * (y * (y < 0 ? -y : y) - y) + y;
}

// x range: [-PI, PI]
inline float fast_cosine(float x) {
    constexpr float PI = 3.14159265358f;
    constexpr float B = 4.0f / PI;
    constexpr float C = -4.0f / (PI * PI);
    constexpr float P = 0.225f;

    x = (x > 0) ? -x : x;
    x += PI/2;

    return fast_sine(x);
}

static float rnvalue() {
  float theta = 2.0f * 3.1415926f * RandomValue();
  float rho = std::sqrt(-2.0f * fast_log(RandomValue()));
  return rho * fast_cosine(theta);
}

glm::vec3 RandomDirection() {
  float a[3] = {rnvalue(), rnvalue(), rnvalue()};
  float ilen = 1 / std::sqrt( a[0]*a[0] + a[1]*a[1] + a[2]*a[2] );
  a[0] *= ilen;
  a[1] *= ilen;
  a[2] *= ilen;

    return glm::vec3{a[0],a[1], a[2]};
}

void RandomInit() {}
