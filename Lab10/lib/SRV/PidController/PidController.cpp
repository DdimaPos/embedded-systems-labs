#include "PidController.h"

void Pid_Init(PidController_t *pid,
              float kp, float ki, float kd,
              float outMin, float outMax,
              float dt)
{
    pid->kp    = kp;
    pid->ki    = ki;
    pid->kd    = kd;
    pid->outMin = outMin;
    pid->outMax = outMax;
    pid->dt     = dt;
    Pid_Reset(pid);
}

void Pid_Reset(PidController_t *pid)
{
    pid->integral        = 0.0f;
    pid->prevMeasurement = 0.0f;
    pid->firstCall       = 1;
}

void Pid_SetTunings(PidController_t *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

float Pid_Compute(PidController_t *pid, float setpoint, float measurement)
{
    float error = setpoint - measurement;

    /* ── Proportional ─────────────────────────────────────────────────── */
    float pTerm = pid->kp * error;

    /* ── Integral with anti-windup clamp ──────────────────────────────── */
    pid->integral += error * pid->dt;

    /* Clamp integral so that Ki*integral stays within output limits.
     * This prevents runaway accumulation while the actuator is saturated. */
    if (pid->ki != 0.0f) {
        float iMax = pid->outMax / pid->ki;
        float iMin = pid->outMin / pid->ki;
        if (pid->integral > iMax) pid->integral = iMax;
        if (pid->integral < iMin) pid->integral = iMin;
    }
    float iTerm = pid->ki * pid->integral;

    /* ── Derivative on measurement (not error) ────────────────────────── */
    /* Using measurement prevents a sudden derivative spike ("kick") when
     * the setpoint is changed. The sign is negated because:
     *   d(error)/dt = d(SP - meas)/dt = -d(meas)/dt  */
    float dTerm = 0.0f;
    if (!pid->firstCall) {
        float dMeas = measurement - pid->prevMeasurement;
        dTerm = -pid->kd * dMeas / pid->dt;
    }
    pid->firstCall       = 0;
    pid->prevMeasurement = measurement;

    /* ── Sum and clamp output ─────────────────────────────────────────── */
    float output = pTerm + iTerm + dTerm;
    if (output > pid->outMax) output = pid->outMax;
    if (output < pid->outMin) output = pid->outMin;

    return output;
}
