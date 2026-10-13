/*
 * untitled.c
 *
 * Course Support License -- for instructional use for courses.  Not for
 * government, research, commercial, or other organizational use.
 *
 * Code generation for model "untitled".
 *
 * Model version              : 1.0
 * Simulink Coder version : 26.1 (R2026a) 20-Nov-2025
 * C source code generated on : Mon Aug 31 17:18:10 2026
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "untitled.h"
#include "rtwtypes.h"
#include "untitled_private.h"
#include <string.h>

/* Block signals (default storage) */
B_untitled_T untitled_B;

/* Continuous states */
X_untitled_T untitled_X;

/* Disabled State Vector */
XDis_untitled_T untitled_XDis;

/* Block states (default storage) */
DW_untitled_T untitled_DW;

/* Real-time model */
static RT_MODEL_untitled_T untitled_M_;
RT_MODEL_untitled_T *const untitled_M = &untitled_M_;

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 2;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  untitled_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  untitled_step();
  untitled_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  untitled_step();
  untitled_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void untitled_step(void)
{
  real_T rtb_Add_g;
  if (rtmIsMajorTimeStep(untitled_M)) {
    /* set solver stop time */
    if (!(untitled_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&untitled_M->solverInfo,
                            ((untitled_M->Timing.clockTickH0 + 1) *
        untitled_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&untitled_M->solverInfo,
                            ((untitled_M->Timing.clockTick0 + 1) *
        untitled_M->Timing.stepSize0 + untitled_M->Timing.clockTickH0 *
        untitled_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(untitled_M)) {
    untitled_M->Timing.t[0] = rtsiGetT(&untitled_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(untitled_M) &&
      untitled_M->Timing.TaskCounters.TID[1] == 0) {
    /* Outputs for Atomic SubSystem: '<S2>/Subsystem' */
    /* Sum: '<S3>/Add' incorporates:
     *  Delay: '<S3>/Delay'
     */
    rtb_Add_g = 0.0 - untitled_DW.Delay_DSTATE_f;

    /* Gain: '<S3>/Gain' */
    untitled_B.Gain = untitled_P.Gain_Gain * rtb_Add_g;

    /* End of Outputs for SubSystem: '<S2>/Subsystem' */
  }

  /* Outputs for Atomic SubSystem: '<S2>/Subsystem' */
  /* Sum: '<S3>/Add1' incorporates:
   *  Integrator: '<S3>/Integrator'
   */
  untitled_B.Add1_b = untitled_B.Gain + untitled_X.Integrator_CSTATE_i;
  if (rtmIsMajorTimeStep(untitled_M) &&
      untitled_M->Timing.TaskCounters.TID[1] == 0) {
    /* Gain: '<S3>/Gain1' */
    untitled_B.Gain1_g = untitled_P.Gain1_Gain * rtb_Add_g;

    /* Outputs for Atomic SubSystem: '<S2>/Subsystem1' */
    /* Delay: '<S4>/Delay' */
    untitled_B.Delay = untitled_DW.Delay_DSTATE;

    /* End of Outputs for SubSystem: '<S2>/Subsystem1' */
  }

  /* End of Outputs for SubSystem: '<S2>/Subsystem' */

  /* Outputs for Atomic SubSystem: '<S2>/Subsystem1' */
  /* Sum: '<S4>/Add' */
  rtb_Add_g = untitled_B.Add1_b - untitled_B.Delay;

  /* Sum: '<S4>/Add1' incorporates:
   *  Gain: '<S4>/Gain'
   *  Integrator: '<S4>/Integrator'
   */
  untitled_B.Add1 = untitled_P.Gain_Gain_o * rtb_Add_g +
    untitled_X.Integrator_CSTATE;

  /* Gain: '<S4>/Gain1' */
  untitled_B.Gain1 = untitled_P.Gain1_Gain_a * rtb_Add_g;

  /* End of Outputs for SubSystem: '<S2>/Subsystem1' */
  if (rtmIsMajorTimeStep(untitled_M)) {
    /* Matfile logging */
    rt_UpdateTXYLogVars(untitled_M->rtwLogInfo, (untitled_M->Timing.t));
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(untitled_M)) {
    /* Update for Atomic SubSystem: '<S2>/Subsystem' */
    if (rtmIsMajorTimeStep(untitled_M) &&
        untitled_M->Timing.TaskCounters.TID[1] == 0) {
      /* Update for Delay: '<S3>/Delay' */
      untitled_DW.Delay_DSTATE_f = untitled_B.Add1_b;

      /* Update for Atomic SubSystem: '<S2>/Subsystem1' */
      /* Update for Delay: '<S4>/Delay' */
      untitled_DW.Delay_DSTATE = untitled_B.Add1;

      /* End of Update for SubSystem: '<S2>/Subsystem1' */
    }

    /* End of Update for SubSystem: '<S2>/Subsystem' */
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(untitled_M)) {
    /* signal main to stop simulation */
    {                                  /* Sample time: [0.0s, 0.0s] */
      if ((rtmGetTFinal(untitled_M)!=-1) &&
          !((rtmGetTFinal(untitled_M)-(((untitled_M->Timing.clockTick1+
               untitled_M->Timing.clockTickH1* 4294967296.0)) * 0.001)) >
            (((untitled_M->Timing.clockTick1+untitled_M->Timing.clockTickH1*
               4294967296.0)) * 0.001) * (DBL_EPSILON))) {
        rtmSetErrorStatus(untitled_M, "Simulation finished");
      }
    }

    rt_ertODEUpdateContinuousStates(&untitled_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++untitled_M->Timing.clockTick0)) {
      ++untitled_M->Timing.clockTickH0;
    }

    untitled_M->Timing.t[0] = rtsiGetSolverStopTime(&untitled_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.001s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.001, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      untitled_M->Timing.clockTick1++;
      if (!untitled_M->Timing.clockTick1) {
        untitled_M->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void untitled_derivatives(void)
{
  XDot_untitled_T *_rtXdot;
  _rtXdot = ((XDot_untitled_T *) untitled_M->derivs);

  /* Derivatives for Atomic SubSystem: '<S2>/Subsystem' */
  /* Derivatives for Integrator: '<S3>/Integrator' */
  _rtXdot->Integrator_CSTATE_i = untitled_B.Gain1_g;

  /* End of Derivatives for SubSystem: '<S2>/Subsystem' */

  /* Derivatives for Atomic SubSystem: '<S2>/Subsystem1' */
  /* Derivatives for Integrator: '<S4>/Integrator' */
  _rtXdot->Integrator_CSTATE = untitled_B.Gain1;

  /* End of Derivatives for SubSystem: '<S2>/Subsystem1' */
}

/* Model initialize function */
void untitled_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)untitled_M, 0,
                sizeof(RT_MODEL_untitled_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&untitled_M->solverInfo,
                          &untitled_M->Timing.simTimeStep);
    rtsiSetTPtr(&untitled_M->solverInfo, &rtmGetTPtr(untitled_M));
    rtsiSetStepSizePtr(&untitled_M->solverInfo, &untitled_M->Timing.stepSize0);
    rtsiSetdXPtr(&untitled_M->solverInfo, &untitled_M->derivs);
    rtsiSetContStatesPtr(&untitled_M->solverInfo, (real_T **)
                         &untitled_M->contStates);
    rtsiSetNumContStatesPtr(&untitled_M->solverInfo,
      &untitled_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&untitled_M->solverInfo,
      &untitled_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&untitled_M->solverInfo,
      &untitled_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&untitled_M->solverInfo,
      &untitled_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&untitled_M->solverInfo, (boolean_T**)
      &untitled_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&untitled_M->solverInfo, (&rtmGetErrorStatus
      (untitled_M)));
    rtsiSetRTModelPtr(&untitled_M->solverInfo, untitled_M);
  }

  rtsiSetSimTimeStep(&untitled_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&untitled_M->solverInfo, false);
  rtsiSetIsContModeFrozen(&untitled_M->solverInfo, false);
  untitled_M->intgData.y = untitled_M->odeY;
  untitled_M->intgData.f[0] = untitled_M->odeF[0];
  untitled_M->intgData.f[1] = untitled_M->odeF[1];
  untitled_M->intgData.f[2] = untitled_M->odeF[2];
  untitled_M->contStates = ((X_untitled_T *) &untitled_X);
  untitled_M->contStateDisabled = ((XDis_untitled_T *) &untitled_XDis);
  untitled_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&untitled_M->solverInfo, (void *)&untitled_M->intgData);
  rtsiSetSolverName(&untitled_M->solverInfo,"ode3");
  rtmSetTPtr(untitled_M, &untitled_M->Timing.tArray[0]);
  rtmSetTFinal(untitled_M, 10.0);
  untitled_M->Timing.stepSize0 = 0.001;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    untitled_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(untitled_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(untitled_M->rtwLogInfo, (NULL));
    rtliSetLogT(untitled_M->rtwLogInfo, "tout");
    rtliSetLogX(untitled_M->rtwLogInfo, "");
    rtliSetLogXFinal(untitled_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(untitled_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(untitled_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(untitled_M->rtwLogInfo, 0);
    rtliSetLogDecimation(untitled_M->rtwLogInfo, 1);
    rtliSetLogY(untitled_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(untitled_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(untitled_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &untitled_B), 0,
                sizeof(B_untitled_T));

  /* states (continuous) */
  {
    (void) memset((void *)&untitled_X, 0,
                  sizeof(X_untitled_T));
  }

  /* disabled states */
  {
    (void) memset((void *)&untitled_XDis, 0,
                  sizeof(XDis_untitled_T));
  }

  /* states (dwork) */
  (void) memset((void *)&untitled_DW, 0,
                sizeof(DW_untitled_T));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(untitled_M->rtwLogInfo, 0.0, rtmGetTFinal
    (untitled_M), untitled_M->Timing.stepSize0, (&rtmGetErrorStatus(untitled_M)));

  /* SystemInitialize for Atomic SubSystem: '<S2>/Subsystem' */
  /* InitializeConditions for Delay: '<S3>/Delay' */
  untitled_DW.Delay_DSTATE_f = untitled_P.Delay_InitialCondition;

  /* InitializeConditions for Integrator: '<S3>/Integrator' */
  untitled_X.Integrator_CSTATE_i = untitled_P.Integrator_IC;

  /* End of SystemInitialize for SubSystem: '<S2>/Subsystem' */

  /* SystemInitialize for Atomic SubSystem: '<S2>/Subsystem1' */
  /* InitializeConditions for Delay: '<S4>/Delay' */
  untitled_DW.Delay_DSTATE = untitled_P.Delay_InitialCondition_m;

  /* InitializeConditions for Integrator: '<S4>/Integrator' */
  untitled_X.Integrator_CSTATE = untitled_P.Integrator_IC_a;

  /* End of SystemInitialize for SubSystem: '<S2>/Subsystem1' */
}

/* Model terminate function */
void untitled_terminate(void)
{
  /* (no terminate code required) */
}
