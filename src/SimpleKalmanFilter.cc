/*
 * SimpleKalmanFilter.cc
 *
 *  Created on: Mar 8, 2024
 *      Author: pramita
 */

#include "../src/SimpleKalmanFilter.h"

#include <cmath>

SimpleKalmanFilter::SimpleKalmanFilter(float mea_e, float est_e, float q) {
    _err_measure = mea_e;
    _err_estimate = est_e;
    _q = q;
}

float SimpleKalmanFilter::updateEstimate(float mea) {
    _kalman_gain = _err_estimate / (_err_estimate + _err_measure);
    _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
    _err_estimate = (1.0f - _kalman_gain) * _err_estimate + std::fabs(_last_estimate - _current_estimate) * _q;
    _last_estimate = _current_estimate;

    return _current_estimate;
}

void SimpleKalmanFilter::setMeasurementError(float mea_e) {
    _err_measure = mea_e;
}

void SimpleKalmanFilter::setEstimateError(float est_e) {
    _err_estimate = est_e;
}

void SimpleKalmanFilter::setProcessNoise(float q) {
    _q = q;
}

float SimpleKalmanFilter::getKalmanGain() {
    return _kalman_gain;
}

float SimpleKalmanFilter::getEstimateError() {
    return _err_estimate;
}



