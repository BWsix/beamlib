#include "easings.h"
using Blib::EaseFunc;

#include <cmath>

static const float PI = 3.1415926f;

float Blib::easefunc_easeInSine(const float x) {
  return 1 - std::cos((x * PI) / 2);
}
float Blib::easefunc_easeOutSine(const float x) {
  return std::sin((x * PI) / 2);
}
float Blib::easefunc_easeInOutSine(const float x) {
  return -(std::cos(PI * x) - 1) / 2;
}
float Blib::easefunc_easeInQuad(const float x) { return x * x; }
float Blib::easefunc_easeOutQuad(const float x) {
  return 1 - (1 - x) * (1 - x);
}
float Blib::easefunc_easeInOutQuad(const float x) {
  return x < 0.5 ? 2 * x * x : 1 - std::pow(-2 * x + 2, 2) / 2;
}
float Blib::easefunc_easeInCubic(const float x) { return x * x * x; }
float Blib::easefunc_easeOutCubic(const float x) {
  return 1 - std::pow(1 - x, 3);
}
float Blib::easefunc_easeInOutCubic(const float x) {
  return x < 0.5 ? 4 * x * x * x : 1 - std::pow(-2 * x + 2, 3) / 2;
}
float Blib::easefunc_easeInQuart(const float x) { return x * x * x * x; }
float Blib::easefunc_easeOutQuart(const float x) {
  return 1 - std::pow(1 - x, 4);
}
float Blib::easefunc_easeInOutQuart(const float x) {
  return x < 0.5 ? 8 * x * x * x * x : 1 - std::pow(-2 * x + 2, 4) / 2;
}
float Blib::easefunc_easeInQuint(const float x) { return x * x * x * x * x; }
float Blib::easefunc_easeOutQuint(const float x) {
  return 1 - std::pow(1 - x, 5);
}
float Blib::easefunc_easeInOutQuint(const float x) {
  return x < 0.5 ? 16 * x * x * x * x * x : 1 - std::pow(-2 * x + 2, 5) / 2;
}
float Blib::easefunc_easeInExpo(const float x) {
  return x == 0 ? 0 : std::pow(2, 10 * x - 10);
}
float Blib::easefunc_easeOutExpo(const float x) {
  return x == 1 ? 1 : 1 - std::pow(2, -10 * x);
}
float Blib::easefunc_easeInOutExpo(const float x) {
  return x == 0    ? 0
         : x == 1  ? 1
         : x < 0.5 ? std::pow(2, 20 * x - 10) / 2
                   : (2 - std::pow(2, -20 * x + 10)) / 2;
}
float Blib::easefunc_easeInCirc(const float x) {
  return 1 - std::sqrt(1 - std::pow(x, 2));
}
float Blib::easefunc_easeOutCirc(const float x) {
  return std::sqrt(1 - std::pow(x - 1, 2));
}
float Blib::easefunc_easeInOutCirc(const float x) {
  return x < 0.5 ? (1 - std::sqrt(1 - std::pow(2 * x, 2))) / 2
                 : (std::sqrt(1 - std::pow(-2 * x + 2, 2)) + 1) / 2;
}
float Blib::easefunc_easeInBack(const float x) {
  const float c1 = 1.70158;
  const float c3 = c1 + 1;

  return c3 * x * x * x - c1 * x * x;
}
float Blib::easefunc_easeOutBack(const float x) {
  const float c1 = 1.70158;
  const float c3 = c1 + 1;

  return 1 + c3 * std::pow(x - 1, 3) + c1 * std::pow(x - 1, 2);
}
float Blib::easefunc_easeInOutBack(const float x) {
  const float c1 = 1.70158;
  const float c2 = c1 * 1.525;

  return x < 0.5
             ? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
             : (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}
float Blib::easefunc_easeInElastic(const float x) {
  const float c4 = (2 * PI) / 3;

  return x == 0   ? 0
         : x == 1 ? 1
                  : -std::pow(2, 10 * x - 10) * std::sin((x * 10 - 10.75) * c4);
}
float Blib::easefunc_easeOutElastic(const float x) {
  const float c4 = (2 * PI) / 3;

  return x == 0   ? 0
         : x == 1 ? 1
                  : std::pow(2, -10 * x) * std::sin((x * 10 - 0.75) * c4) + 1;
}
float Blib::easefunc_easeInOutElastic(const float x) {
  const float c5 = (2 * PI) / 4.5;

  return x == 0   ? 0
         : x == 1 ? 1
         : x < 0.5
             ? -(std::pow(2, 20 * x - 10) * std::sin((20 * x - 11.125) * c5)) /
                   2
             : (std::pow(2, -20 * x + 10) * std::sin((20 * x - 11.125) * c5)) /
                       2 +
                   1;
}
float Blib::easefunc_easeOutBounce(float x);
float Blib::easefunc_easeInBounce(const float x) {
  return 1 - easefunc_easeOutBounce(1 - x);
}
float Blib::easefunc_easeOutBounce(float x) {
  const float n1 = 7.5625;
  const float d1 = 2.75;

  if (x < 1 / d1) {
    return n1 * x * x;
  } else if (x < 2 / d1) {
    x -= 1.5 / d1;
    return n1 * x * x + 0.75;
  } else if (x < 2.5 / d1) {
    x -= 2.25 / d1;
    return n1 * x * x + 0.9375;
  } else {
    x -= 2.625 / d1;
    return n1 * x * x + 0.984375;
  }
}
float Blib::easefunc_easeInOutBounce(const float x) {
  return x < 0.5 ? (1 - easefunc_easeOutBounce(1 - 2 * x)) / 2
                 : (1 + easefunc_easeOutBounce(2 * x - 1)) / 2;
}

float Blib::ease(EaseFunc e, float p) {
    switch (e) {
        case EaseFunc::Lerp: return p;
        case EaseFunc::InSine: return easefunc_easeInSine(p);
        case EaseFunc::OutSine: return easefunc_easeOutSine(p);
        case EaseFunc::InOutSine: return easefunc_easeInOutSine(p);
        case EaseFunc::InQuad: return easefunc_easeInQuad(p);
        case EaseFunc::OutQuad: return easefunc_easeOutQuad(p);
        case EaseFunc::InOutQuad: return easefunc_easeInOutQuad(p);
        case EaseFunc::InCubic: return easefunc_easeInCubic(p);
        case EaseFunc::OutCubic: return easefunc_easeOutCubic(p);
        case EaseFunc::InOutCubic: return easefunc_easeInOutCubic(p);
        case EaseFunc::InQuart: return easefunc_easeInQuart(p);
        case EaseFunc::OutQuart: return easefunc_easeOutQuart(p);
        case EaseFunc::InOutQuart: return easefunc_easeInOutQuart(p);
        case EaseFunc::InQuint: return easefunc_easeInQuint(p);
        case EaseFunc::OutQuint: return easefunc_easeOutQuint(p);
        case EaseFunc::InOutQuint: return easefunc_easeInOutQuint(p);
        case EaseFunc::InExpo: return easefunc_easeInExpo(p);
        case EaseFunc::OutExpo: return easefunc_easeOutExpo(p);
        case EaseFunc::InOutExpo: return easefunc_easeInOutExpo(p);
        case EaseFunc::InCirc: return easefunc_easeInCirc(p);
        case EaseFunc::OutCirc: return easefunc_easeOutCirc(p);
        case EaseFunc::InOutCirc: return easefunc_easeInOutCirc(p);
        case EaseFunc::InBack: return easefunc_easeInBack(p);
        case EaseFunc::OutBack: return easefunc_easeOutBack(p);
        case EaseFunc::InOutBack: return easefunc_easeInOutBack(p);
        case EaseFunc::InElastic: return easefunc_easeInElastic(p);
        case EaseFunc::OutElastic: return easefunc_easeOutElastic(p);
        case EaseFunc::InOutElastic: return easefunc_easeInOutElastic(p);
        case EaseFunc::InBounce: return easefunc_easeInBounce(p);
        case EaseFunc::OutBounce: return easefunc_easeOutBounce(p);
        case EaseFunc::InOutBounce: return easefunc_easeInOutBounce(p);
    }
}
