/* 
 * SimpleKalmanFilter - a Kalman Filter implementation for single variable models.
 * Created by Denys Sene, January, 1, 2017.
 * Released under MIT License - see LICENSE file for details.
 */

#ifndef SimpleKalmanFilter_h
#define SimpleKalmanFilter_h


#define PH_KM		    		0
#define EC_KM           1
#define YAW             2

void SimpleKalmanFilter(float mea_e, float est_e, float q);

void setMeasurementError(float mea_e);
void setEstimateError(float est_e);
void setProcessNoise(float q);

float updateEstimate(float mea, int kind);
float getKalmanGain_roll(int kind);



  


#endif
