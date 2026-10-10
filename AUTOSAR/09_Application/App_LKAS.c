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
 * Component: /ARRoot/App_LKAS
 * Runnable : All Runnables in SwComponent
 *****************************************************************************
 * Tool  : ODIN
 * Author: CastChoi
 * Date  : �� 12�� 29 13:28:13 2018
 ****************************************************************************/

#include "Rte_App_LKAS.h"

/*PROTECTED REGION ID(FileHeaderUserDefinedIncludes :Runnable_LKAS_100ms) ENABLED START */
/* Start of user defined includes  - Do not remove this comment */
/* End of user defined includes - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedConstants :Runnable_LKAS_100ms) ENABLED START */
/* Start of user defined constant definitions - Do not remove this comment */
/* End of user defined constant definitions - Do not remove this comment */
/*PROTECTED REGION END */

/*PROTECTED REGION ID(FileHeaderUserDefinedVariables :Runnable_LKAS_100ms) ENABLED START */
/* Start of user variable defintions - Do not remove this comment  */
/* End of user variable defintions - Do not remove this comment  */
/*PROTECTED REGION END */
#define App_LKAS_START_SEC_CODE                   
#include "App_LKAS_MemMap.h"
FUNC (void, App_LKAS_CODE) Runnable_LKAS_100ms // return value & FctID 
(
		void
)
{

	sint32 read1;
	Std_ReturnType retRead1;
	sint32 read2;
	Std_ReturnType retRead2;
	sint32 write3;
	Std_ReturnType retWrite3;
	sint32 write4;
	Std_ReturnType retWrite4;

	/* Local Data Declaration */

	/*PROTECTED REGION ID(UserVariables :Runnable_LKAS_100ms) ENABLED START */
	/* Start of user variable defintions - Do not remove this comment  */
	/* End of user variable defintions - Do not remove this comment  */
	/*PROTECTED REGION END */
	Std_ReturnType retValue = RTE_E_OK;
	 /*  -------------------------------------- Data Read -----------------------------------------  */
	  retRead1 = Rte_Read_LKAS_Recv_STEER_VALUE(&read1);
	  retRead2 = Rte_Read_LKAS_Recv_LKAS_TRIGGER(&read2);

	  /*  -------------------------------------- Server Call Point  --------------------------------  */

	  /*  -------------------------------------- CDATA ---------------------------------------------  */

	  /*  -------------------------------------- Data Write ----------------------------------------  */
	  if(read2 > 5000) {
	    if(read1 > 0) {
	      if(read1 > 10000) {
	        write4 = 10000;
	      } else {
	        write4 = read1;
	      }
	      write3 = 0;
	    } else if(read1 < 0) {
	      if(read1 < -10000) {
	        write3 = 10000;
	      } else {
	        write3 = -read1;
	      }
	      write4 = 0;
	    }
	  } else {
	    write3 = 0;
	    write4 = 0;
	  }

	  retWrite3 = Rte_Write_LKAS_Send_RIGHT_STEER(write3);
	  retWrite4 = Rte_Write_LKAS_Send_LEFT_STEER(write4);
	  /*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Trigger Interface ---------------------------------  */

	/*  -------------------------------------- Mode Management -----------------------------------  */

	/*  -------------------------------------- Port Handling -------------------------------------  */

	/*  -------------------------------------- Exclusive Area ------------------------------------  */

	/*  -------------------------------------- Multiple Instantiation ----------------------------  */

	/*PROTECTED REGION ID(User Logic :Runnable_LKAS_100ms) ENABLED START */
	/* Start of user code - Do not remove this comment */
	/* End of user code - Do not remove this comment */
	/*PROTECTED REGION END */

}

#define App_LKAS_STOP_SEC_CODE                       
#include "App_LKAS_MemMap.h"
