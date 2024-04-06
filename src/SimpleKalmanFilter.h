/*
 * SimpleKalmanFilter.h
 *
 *  Created on: Mar 8, 2024
 *      Author: pramita
 */

#ifndef SIMPLEKALMANFILTER_H_
#define SIMPLEKALMANFILTER_H_

class SimpleKalmanFilter {
private:
    float _err_measure;
    float _err_estimate;
    float _q;
    float _last_estimate;
    float _current_estimate;
    float _kalman_gain;

public:
    SimpleKalmanFilter(float mea_e, float est_e, float q);
    float updateEstimate(float mea);
    void setMeasurementError(float mea_e);
    void setEstimateError(float est_e);
    void setProcessNoise(float q);
    float getKalmanGain();
    float getEstimateError();
};

#endif /* SIMPLEKALMANFILTER_H_ */
