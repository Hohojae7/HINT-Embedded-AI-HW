/*
 * Course Support License -- for instructional use for courses.  Not for
 * government, research, commercial, or other organizational use.
 *
 * File: untitled.c
 *
 * Code generated for Simulink model 'untitled'.
 *
 * Model version                  : 1.0
 * Simulink Coder version         : 26.1 (R2026a) 20-Nov-2025
 * C/C++ source code generated on : Thu Sep  3 17:14:41 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Atmel->AVR
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "untitled.h"
#include "MW_AnalogIn.h"
#include "MW_ServoWriteRead.h"
#include "rtwtypes.h"
#include "untitled_private.h"
#include <math.h>
#define untitled_period                (0.01)

/* Block states (default storage) */
DW_untitled_T untitled_DW;

/* Real-time model */
static RT_MODEL_untitled_T untitled_M_;
RT_MODEL_untitled_T *const untitled_M = &untitled_M_;
real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/* Model step function */
void untitled_step(void)
{
  real_T rateLimiterRate;
  real_T rtb_Gain;
  real_T tmp_0;
  uint16_T b_varargout_1;
  uint8_T tmp;

  /* MATLABSystem: '<Root>/Analog Input' */
  untitled_DW.obj.AnalogInDriverObj.MW_ANALOGIN_HANDLE = MW_AnalogIn_GetHandle
    (14UL);
  MW_AnalogInSingle_ReadResult
    (untitled_DW.obj.AnalogInDriverObj.MW_ANALOGIN_HANDLE, &b_varargout_1,
     MW_ANALOGIN_UINT16);

  /* Gain: '<Root>/Gain' incorporates:
   *  MATLABSystem: '<Root>/Analog Input'
   * */
  rtb_Gain = (real_T)untitled_P.Gain_Gain * 3.814697265625E-6 * (real_T)
    b_varargout_1;

  /* RateLimiter: '<Root>/Rate Limiter' */
  rateLimiterRate = rtb_Gain - untitled_DW.PrevY;
  tmp_0 = untitled_P.RateLimiter_RisingLim * untitled_period;
  if (rateLimiterRate > tmp_0) {
    rtb_Gain = tmp_0 + untitled_DW.PrevY;
  } else if (rateLimiterRate < untitled_P.RateLimiter_FallingLim *
             untitled_period) {
    rtb_Gain = untitled_P.RateLimiter_FallingLim * untitled_period +
      untitled_DW.PrevY;
  }

  untitled_DW.PrevY = rtb_Gain;

  /* End of RateLimiter: '<Root>/Rate Limiter' */

  /* MATLABSystem: '<Root>/Standard Servo Write' */
  if (rtb_Gain < 0.0) {
    tmp = 0U;
  } else if (rtb_Gain > 180.0) {
    tmp = 180U;
  } else {
    tmp_0 = rt_roundd_snf(rtb_Gain);
    if (tmp_0 < 256.0) {
      tmp = (uint8_T)tmp_0;
    } else {
      tmp = MAX_uint8_T;
    }
  }

  MW_servoWrite(0, tmp);

  /* End of MATLABSystem: '<Root>/Standard Servo Write' */

  /* MATLABSystem: '<Root>/Standard Servo Write1' */
  if (rtb_Gain < 0.0) {
    tmp = 0U;
  } else if (rtb_Gain > 180.0) {
    tmp = 180U;
  } else {
    tmp_0 = rt_roundd_snf(rtb_Gain);
    if (tmp_0 < 256.0) {
      tmp = (uint8_T)tmp_0;
    } else {
      tmp = MAX_uint8_T;
    }
  }

  MW_servoWrite(1, tmp);

  /* End of MATLABSystem: '<Root>/Standard Servo Write1' */
}

/* Model initialize function */
void untitled_initialize(void)
{
  /* InitializeConditions for RateLimiter: '<Root>/Rate Limiter' */
  untitled_DW.PrevY = untitled_P.RateLimiter_IC;

  /* Start for MATLABSystem: '<Root>/Analog Input' */
  untitled_DW.obj.matlabCodegenIsDeleted = false;
  untitled_DW.obj.isInitialized = 1L;
  untitled_DW.obj.AnalogInDriverObj.MW_ANALOGIN_HANDLE = MW_AnalogInSingle_Open
    (14UL);
  untitled_DW.obj.isSetupComplete = true;

  /* Start for MATLABSystem: '<Root>/Standard Servo Write' */
  untitled_DW.obj_j.matlabCodegenIsDeleted = false;
  untitled_DW.obj_j.isInitialized = 1L;
  MW_servoAttach(0, 9);
  untitled_DW.obj_j.isSetupComplete = true;

  /* Start for MATLABSystem: '<Root>/Standard Servo Write1' */
  untitled_DW.obj_n.matlabCodegenIsDeleted = false;
  untitled_DW.obj_n.isInitialized = 1L;
  MW_servoAttach(1, 10);
  untitled_DW.obj_n.isSetupComplete = true;
}

/* Model terminate function */
void untitled_terminate(void)
{
  /* Terminate for MATLABSystem: '<Root>/Analog Input' */
  if (!untitled_DW.obj.matlabCodegenIsDeleted) {
    untitled_DW.obj.matlabCodegenIsDeleted = true;
    if ((untitled_DW.obj.isInitialized == 1L) && untitled_DW.obj.isSetupComplete)
    {
      untitled_DW.obj.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
        MW_AnalogIn_GetHandle(14UL);
      MW_AnalogIn_Close(untitled_DW.obj.AnalogInDriverObj.MW_ANALOGIN_HANDLE);
    }
  }

  /* End of Terminate for MATLABSystem: '<Root>/Analog Input' */

  /* Terminate for MATLABSystem: '<Root>/Standard Servo Write' */
  if (!untitled_DW.obj_j.matlabCodegenIsDeleted) {
    untitled_DW.obj_j.matlabCodegenIsDeleted = true;
  }

  /* End of Terminate for MATLABSystem: '<Root>/Standard Servo Write' */

  /* Terminate for MATLABSystem: '<Root>/Standard Servo Write1' */
  if (!untitled_DW.obj_n.matlabCodegenIsDeleted) {
    untitled_DW.obj_n.matlabCodegenIsDeleted = true;
  }

  /* End of Terminate for MATLABSystem: '<Root>/Standard Servo Write1' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
