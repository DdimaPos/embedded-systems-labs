#pragma once

/*
 * PidController – reusable discrete-time PID algorithm.
 *
 * Design choices:
 *   - Derivative on measurement (not on error) to eliminate derivative kick
 *     when the setpoint changes abruptly.
 *   - Integral anti-windup: the integral accumulator is clamped so that its
 *     contribution (Ki * integral) stays within [outMin, outMax]. This prevents
 *     the integral from growing to infinity during sustained saturation.
 *   - dt is fixed at init time (task period), avoiding the need for a real-time
 *     clock read every iteration.
 */

typedef struct {
    float kp;              /* Proportional gain                              */
    float ki;              /* Integral gain                                  */
    float kd;              /* Derivative gain                                */
    float integral;        /* Accumulated integral                           */
    float prevMeasurement; /* Last measurement for derivative calculation    */
    float outMin;          /* Lower output clamp                             */
    float outMax;          /* Upper output clamp                             */
    float dt;              /* Sampling interval in seconds                   */
    int   firstCall;       /* 1 on first Compute call (skip derivative)      */
} PidController_t;

/*
 * Pid_Init – initialise all fields.
 *   outMin/outMax: output clamp range (e.g. 0.0f – 100.0f for percent)
 *   dt: seconds between Pid_Compute calls (= task period / 1000.0f)
 */
void  Pid_Init(PidController_t *pid,
               float kp, float ki, float kd,
               float outMin, float outMax,
               float dt);

/*
 * Pid_Compute – run one PID iteration.
 *   Returns clamped output in [outMin, outMax].
 */
float Pid_Compute(PidController_t *pid, float setpoint, float measurement);

/*
 * Pid_Reset – clear integral and derivative state (call when switching modes
 *             or after a long fault period to avoid windup on re-enable).
 */
void  Pid_Reset(PidController_t *pid);

/*
 * Pid_SetTunings – update Kp/Ki/Kd at runtime (e.g. via serial command).
 *                  Does NOT reset integral or derivative state.
 */
void  Pid_SetTunings(PidController_t *pid, float kp, float ki, float kd);
