#pragma once

#include <array>

namespace Blib {

enum class EaseFunc { Lerp, InSine, OutSine, InOutSine, InQuad, OutQuad, InOutQuad, InCubic, OutCubic, InOutCubic, InQuart, OutQuart, InOutQuart, InQuint, OutQuint, InOutQuint, InExpo, OutExpo, InOutExpo, InCirc, OutCirc, InOutCirc, InBack, OutBack, InOutBack, InElastic, OutElastic, InOutElastic, InBounce, OutBounce, InOutBounce };
const std::array<EaseFunc, 31> EasingFunctions { EaseFunc::Lerp, EaseFunc::InSine, EaseFunc::OutSine, EaseFunc::InOutSine, EaseFunc::InQuad, EaseFunc::OutQuad, EaseFunc::InOutQuad, EaseFunc::InCubic, EaseFunc::OutCubic, EaseFunc::InOutCubic, EaseFunc::InQuart, EaseFunc::OutQuart, EaseFunc::InOutQuart, EaseFunc::InQuint, EaseFunc::OutQuint, EaseFunc::InOutQuint, EaseFunc::InExpo, EaseFunc::OutExpo, EaseFunc::InOutExpo, EaseFunc::InCirc, EaseFunc::OutCirc, EaseFunc::InOutCirc, EaseFunc::InBack, EaseFunc::OutBack, EaseFunc::InOutBack, EaseFunc::InElastic, EaseFunc::OutElastic, EaseFunc::InOutElastic, EaseFunc::InBounce, EaseFunc::OutBounce, EaseFunc::InOutBounce };
const std::array<const char *, 31> easeFuncNames { "Lerp", "EaseInSine", "EaseOutSine", "EaseInOutSine", "EaseInQuad", "EaseOutQuad", "EaseInOutQuad", "EaseInCubic", "EaseOutCubic", "EaseInOutCubic", "EaseInQuart", "EaseOutQuart", "EaseInOutQuart", "EaseInQuint", "EaseOutQuint", "EaseInOutQuint", "EaseInExpo", "EaseOutExpo", "EaseInOutExpo", "EaseInCirc", "EaseOutCirc", "EaseInOutCirc", "EaseInBack", "EaseOutBack", "EaseInOutBack", "EaseInElastic", "EaseOutElastic", "EaseInOutElastic", "EaseInBounce", "EaseOutBounce", "EaseInOutBounce" };

template<typename T>
T lerp(T from, T to, float progress) {
    return (1.0f - progress) * from + progress * to;
}

float ease(EaseFunc e, float progress);

float easefunc_easeInSine(float x);
float easefunc_easeOutSine(float x);
float easefunc_easeInOutSine(float x);
float easefunc_easeInQuad(float x);
float easefunc_easeOutQuad(float x);
float easefunc_easeInOutQuad(float x);
float easefunc_easeInCubic(float x);
float easefunc_easeOutCubic(float x);
float easefunc_easeInOutCubic(float x);
float easefunc_easeInQuart(float x);
float easefunc_easeOutQuart(float x);
float easefunc_easeInOutQuart(float x);
float easefunc_easeInQuint(float x);
float easefunc_easeOutQuint(float x);
float easefunc_easeInOutQuint(float x);
float easefunc_easeInExpo(float x);
float easefunc_easeOutExpo(float x);
float easefunc_easeInOutExpo(float x);
float easefunc_easeInCirc(float x);
float easefunc_easeOutCirc(float x);
float easefunc_easeInOutCirc(float x);
float easefunc_easeInBack(float x);
float easefunc_easeOutBack(float x);
float easefunc_easeInOutBack(float x);
float easefunc_easeInElastic(float x);
float easefunc_easeOutElastic(float x);
float easefunc_easeInOutElastic(float x);
float easefunc_easeOutBounce(float x);
float easefunc_easeInBounce(float x);
float easefunc_easeOutBounce(float x);
float easefunc_easeInOutBounce(float x);

}
