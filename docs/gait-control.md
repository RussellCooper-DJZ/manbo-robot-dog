# Quadruped Gait Control Algorithm

## Overview

This document describes the gait control algorithms implemented for the Manbo Robot Dog, including trot, walk, and bound gaits.

## Coordinate System

```
        +Z (up)
        |
        |___+X (forward)
       /
      +Y (left)

Body frame origin: center of mass
```

## Inverse Kinematics

For each leg, given a target foot position (x, y, z) in body frame:

```c
typedef struct {
    float hip_angle;    // radians
    float thigh_angle;  // radians
    float calf_angle;   // radians
} LegAngles;

LegAngles ik_solve(float x, float y, float z,
                   float l1, float l2, float l3) {
    // l1=hip offset, l2=thigh length, l3=calf length
    LegAngles angles;

    // Hip angle (rotation about Z axis)
    angles.hip_angle = atan2f(y, x);

    // Project to sagittal plane
    float r = sqrtf(x*x + y*y) - l1;
    float d = sqrtf(r*r + z*z);

    // Calf angle (law of cosines)
    float cos_calf = (l2*l2 + l3*l3 - d*d) / (2*l2*l3);
    angles.calf_angle = acosf(fmaxf(-1.0f, fminf(1.0f, cos_calf))) - M_PI;

    // Thigh angle
    float alpha = atan2f(-z, r);
    float beta = acosf((l2*l2 + d*d - l3*l3) / (2*l2*d));
    angles.thigh_angle = alpha + beta;

    return angles;
}
```

## Trot Gait

In trot gait, diagonal leg pairs move simultaneously:

```
Phase 0°:   FL, RR swing  |  FR, RL stance
Phase 180°: FR, RL swing  |  FL, RR stance
```

```c
#define GAIT_TROT_PERIOD_MS  400
#define SWING_RATIO          0.4f   // 40% swing, 60% stance

float get_leg_phase(LegID leg, uint32_t t_ms) {
    static const float phase_offset[4] = {
        0.0f,   // FL
        0.5f,   // FR (180° offset from FL)
        0.5f,   // RL (same as FR)
        0.0f    // RR (same as FL)
    };
    float t_norm = fmodf((float)t_ms / GAIT_TROT_PERIOD_MS
                         + phase_offset[leg], 1.0f);
    return t_norm;
}
```

## Foot Trajectory (Bezier Curve)

```c
// Cubic Bezier swing trajectory
Vec3 swing_trajectory(float t, Vec3 p0, Vec3 p3, float step_height) {
    Vec3 p1 = {p0.x + (p3.x-p0.x)*0.25f, p0.y, p0.z + step_height};
    Vec3 p2 = {p0.x + (p3.x-p0.x)*0.75f, p0.y, p3.z + step_height};

    float u = 1.0f - t;
    return (Vec3){
        u*u*u*p0.x + 3*u*u*t*p1.x + 3*u*t*t*p2.x + t*t*t*p3.x,
        u*u*u*p0.y + 3*u*u*t*p1.y + 3*u*t*t*p2.y + t*t*t*p3.y,
        u*u*u*p0.z + 3*u*u*t*p1.z + 3*u*t*t*p2.z + t*t*t*p3.z
    };
}
```

## Balance Controller (PD)

```c
typedef struct {
    float kp_roll, kd_roll;
    float kp_pitch, kd_pitch;
} BalanceGains;

void balance_control(IMUData *imu, BalanceGains *gains,
                     float body_height, Vec3 *foot_targets) {
    float roll_correction  = gains->kp_roll  * imu->roll
                           + gains->kd_roll  * imu->roll_rate;
    float pitch_correction = gains->kp_pitch * imu->pitch
                           + gains->kd_pitch * imu->pitch_rate;

    // Adjust foot targets to compensate for body tilt
    for (int i = 0; i < 4; i++) {
        foot_targets[i].z -= roll_correction  * LEG_Y_OFFSET[i];
        foot_targets[i].z -= pitch_correction * LEG_X_OFFSET[i];
    }
}
```
