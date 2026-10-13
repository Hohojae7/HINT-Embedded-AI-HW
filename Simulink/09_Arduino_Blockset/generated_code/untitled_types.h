/*
 * Course Support License -- for instructional use for courses.  Not for
 * government, research, commercial, or other organizational use.
 *
 * File: untitled_types.h
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

#ifndef untitled_types_h_
#define untitled_types_h_
#include "MW_SVD.h"
#include "rtwtypes.h"
#ifndef struct_tag_nAWbnnOgZuxKWuTGXwrCG
#define struct_tag_nAWbnnOgZuxKWuTGXwrCG

struct tag_nAWbnnOgZuxKWuTGXwrCG
{
  MW_Handle_Type MW_ANALOGIN_HANDLE;
};

#endif                                 /* struct_tag_nAWbnnOgZuxKWuTGXwrCG */

#ifndef typedef_e_arduinodriver_ArduinoAnalog_T
#define typedef_e_arduinodriver_ArduinoAnalog_T

typedef struct tag_nAWbnnOgZuxKWuTGXwrCG e_arduinodriver_ArduinoAnalog_T;

#endif                             /* typedef_e_arduinodriver_ArduinoAnalog_T */

#ifndef struct_tag_gXF6Q3iquGlwRgoZrA70wG
#define struct_tag_gXF6Q3iquGlwRgoZrA70wG

struct tag_gXF6Q3iquGlwRgoZrA70wG
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  e_arduinodriver_ArduinoAnalog_T AnalogInDriverObj;
};

#endif                                 /* struct_tag_gXF6Q3iquGlwRgoZrA70wG */

#ifndef typedef_codertarget_arduinobase_inter_T
#define typedef_codertarget_arduinobase_inter_T

typedef struct tag_gXF6Q3iquGlwRgoZrA70wG codertarget_arduinobase_inter_T;

#endif                             /* typedef_codertarget_arduinobase_inter_T */

#ifndef struct_tag_4qD4qQedJTlgQ31V4WZwDF
#define struct_tag_4qD4qQedJTlgQ31V4WZwDF

struct tag_4qD4qQedJTlgQ31V4WZwDF
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
};

#endif                                 /* struct_tag_4qD4qQedJTlgQ31V4WZwDF */

#ifndef typedef_codertarget_arduinobase_int_b_T
#define typedef_codertarget_arduinobase_int_b_T

typedef struct tag_4qD4qQedJTlgQ31V4WZwDF codertarget_arduinobase_int_b_T;

#endif                             /* typedef_codertarget_arduinobase_int_b_T */

/* Parameters (default storage) */
typedef struct P_untitled_T_ P_untitled_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_untitled_T RT_MODEL_untitled_T;

#endif                                 /* untitled_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
