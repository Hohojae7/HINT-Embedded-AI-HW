/* *************************************************************************
 *                                                                       *
 *                      HYUNDAI MOTOR GROUP                              *
 *                                                                       *
 *                      All rights reserved                              *
 *                                                                       *
 *************************************************************************  
 *
 *************************************************************************
 *    Administrative Information (automatically filled in by ODIN)       *
 *************************************************************************
 ****************************************************************************

 * Name : 

 * Description:

 * Version : 1.0

 *****************************************************************************

 * Project : autron_autosar_edu_Warrior_mpc5606b_Base_R181026
 * Component: /ARRoot/App_CC
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool  : ODIN
 * Author: CastChoi
 * Date  : �� 12�� 29 10:30:32 2018
 ****************************************************************************/

#include "Rte_App_CC.h"

/*PROTECTED REGION ID(FileHeaderUserDefinedIncludes :Runnable_CC_100ms) ENABLED START */
/* Start of user defined includes  - Do not remove this comment */
/* End of user defined includes - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedConstants :Runnable_CC_100ms) ENABLED START */
/* Start of user defined constant definitions - Do not remove this comment */
/* End of user defined constant definitions - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedVariables :Runnable_CC_100ms) ENABLED START */
/* Start of user variable defintions - Do not remove this comment  */
/* End of user variable defintions - Do not remove this comment  */
/*PROTECTED REGION END */
#define App_CC_START_SEC_CODE                   
#include "App_CC_MemMap.h"
FUNC (void, App_CC_CODE) Runnable_CC_100ms // return value & FctID 
(
		void
)
{

	uint32 read1;
	Std_ReturnType retRead1;
	sint32 read2;
	Std_ReturnType retRead2;
	sint32 read3;
	Std_ReturnType retRead3;
	sint32 read4;
	Std_ReturnType retRead4;
	sint32 write5;
	Std_ReturnType retWrite5;
	sint32 write6;
	Std_ReturnType retWrite6;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Runnable_CC_100ms) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	 /*  -------------------------------------- Data Read -----------------------------------------  */
	  retRead1 = Rte_Read_CC_Recv1_ACCEL_VALUE(&read1);
	  retRead2 = Rte_Read_CC_Recv1_TARGET_SPEED(&read2);
	  retRead3 = Rte_Read_CC_Recv2_SPEED(&read3);
	  retRead4 = Rte_Read_CC_Recv2_CC_TRIGGER(&read4);

	  /*  -------------------------------------- Server Call Point  --------------------------------  */

	  /*  -------------------------------------- CDATA ---------------------------------------------  */

	  /*  -------------------------------------- Data Write ----------------------------------------  */
	  write5 = 0;
	  write6 = 0;
	  read3 = read3 * 100;

	  if (read4 > 5000) {
	    write6 = read1;
	    if (read3 - read2 < -read2 * 100) {
	      write6 += 500;
	    } else if (read3 > read2) {
	      write6 -= 1000;
	    }
	  }

	  retWrite5 = Rte_Write_CC_Send_BRAKE(write5);
	  retWrite6 = Rte_Write_CC_Send_ACCEL(write6);

	  /*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Runnable_CC_100ms) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}

#define App_CC_STOP_SEC_CODE                       
#include "App_CC_MemMap.h"
