#include "light.h"

SpotLight::SpotLight(float _cutOff, float _outerCutOff, float _kc, float _kl, float _kq) :cutOff(_cutOff), outerCutOff(_outerCutOff), kc(_kc), kl(_kl), kq(_kq)
{
}

PointLight::PointLight(float _kc, float _kl, float _kq) : kc(_kc), kl(_kl), kq(_kq)
{
}
