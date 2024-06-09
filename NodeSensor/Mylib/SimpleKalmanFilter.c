/* 
 * SimpleKalmanFilter - a Kalman Filter implementation for single variable models.
 * Created by Denys Sene, January, 1, 2017.
 * Released under MIT License - see LICENSE file for details.
 */


#include <math.h>
#include "SimpleKalmanFilter.h"

#define USE_KALMAN_FILTER

float _err_measure;

float _q;

float _err_estimate[4];
float _current_estimate[4];
float _last_estimate[4];
float _kalman_gain[4];


void SimpleKalmanFilter(float mea_e, float est_e, float q)
{
  _err_measure=mea_e;
  _err_estimate[0]=_err_estimate[1]=_err_estimate[2]=_err_estimate[3]=est_e;
  _q = q;
}

float updateEstimate(float mea, int kind)
{
#ifdef USE_KALMAN_FILTER
  _kalman_gain[kind] = _err_estimate[kind]/(_err_estimate[kind] + _err_measure);
  _current_estimate[kind] = _last_estimate[kind] + _kalman_gain[kind] * (mea - _last_estimate[kind]);
  _err_estimate[kind] =  (1.0 - _kalman_gain[kind])*_err_estimate[kind] + fabs(_last_estimate[kind] - _current_estimate[kind])*_q;
  _last_estimate[kind] =_current_estimate[kind];
#else
  _current_estimate[kind]  = mea;
#endif
  return _current_estimate[kind];
}

void setMeasurementError(float mea_e)
{
  _err_measure=mea_e;
}

void setEstimateError(float est_e)
{
  _err_estimate[0]=_err_estimate[1]=_err_estimate[2]=est_e;
}

void setProcessNoise(float q)
{
  _q=q;
}

float getKalmanGain_roll(int kind) {
  return _kalman_gain[kind];
}

