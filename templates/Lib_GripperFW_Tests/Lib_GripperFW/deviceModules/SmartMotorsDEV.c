/** \file SmartMotorsDEV.c
*   \brief <a href="https://www.inmechasol.org/" target="_blank">IMS</a>:
		<a href="https://github.com/InMechaSol/ccNOos" target="_blank">ccNOos</a>,
		Implementation for straight C 

    Copyright 2021 <a href="https://www.inmechasol.org/" target="_blank">InMechaSol, Inc</a>    

    Licensed under the Apache License, Version 2.0(the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

Notes:
	(.c includes .h) - for straight C or
	(.cpp includes .c which includes .h) - for C++ wrapped straight C
	*Always compiled to a single compilation unit, either C or CPP, not both

*/
#include "SmartMotorsDEV.h"

struct smartMotorStruct createsmartMotorStruct()
{
    struct smartMotorStruct outStruct;
    outStruct.StatusOK = ui8FALSE;
    outStruct.StatusOKLast = ui8FALSE;
    outStruct.Connected = ui8FALSE;
    outStruct.ModuleID = 0;
    outStruct.MotorsCommSubPhase = ui8FALSE;
    outStruct.MotorsCommPhase = 0;
    outStruct.MotorControlModeCMD = bldc_coast;
    outStruct.MotorControlModeFbk = bldc_coast;
    
    outStruct.CommPeriodAbsoluteMaximum = 0;
    outStruct.CommPeriodAbsoluteMinimum = 1000;
    outStruct.CommPeriodAverage = 0;
    outStruct.PeridoAvgSampleIndex = 0;
    outStruct.PeriodAvgSampleSize = 0;

    outStruct.RotorPositionFbk = 0.0;
    outStruct.RotorPositionFbkStart = 0.0;
    outStruct.RotorPositionFbkStop = 0.0;
    outStruct.MotionRange = 0.0;
    outStruct.MotionRangeLimit = 0.0;
    outStruct.RotorPositionFbkLast = 0.0;
    outStruct.MotorTempDegC = 0.0;
    outStruct.MotorCurrentLimitCMD = MOTORINOMINAL;
    outStruct.MotorCurrentFbk = 0.0;
    outStruct.RotorVelocityFbk = 0.0;
    
    //outStruct.RotorVelocityCmd = 0.0;
    outStruct.millisNow = 0;
    outStruct.millisThen = 0;
    outStruct.millisThentoNow = 0;
    outStruct.OverCurrent = ui8TRUE;
    outStruct.OverTemp = ui8TRUE;

    outStruct.MotorPWMCmd = 0.0;
    outStruct.MotorPWMCmdReturned = 0.0;
    outStruct.RotorPositionCmd = 0.0;
    outStruct.LatchHighStopPos = ui8FALSE;
    outStruct.LatchLowStopPos = ui8FALSE;
    outStruct.RotorPositionHighStop = -LIMITSINITVAL;
    outStruct.RotorPositionLowStop = +LIMITSINITVAL;
    outStruct.TriggerNegativeMove = ui8FALSE;
    outStruct.TriggerPositiveMove = ui8FALSE;
    outStruct.TriggerStop = ui8FALSE;
    outStruct.TriggerPush = ui8FALSE;
    outStruct.TriggerHold = ui8FALSE;
    outStruct.MotionState = sm_stopped;
    outStruct.StalledCyclesCounter = 0;
    
    clearDataLogSamples(&outStruct);
    return outStruct;
}



#define P smartMotorStructPtrIn
#define POSITIONDELTA (P->RotorPositionFbk - P->RotorPositionFbkLast)
void clearDataLogSamples(struct smartMotorStruct* smartMotorStructPtrIn)
{
    int i;
    for (i = 0; i < MAXDATAENTRIES; i++)
    {
        P->MotorPWMCmdData[i] = 0.0;
        P->MotionStateData[i] = sm_stopped;
        P->MotorCurrentLimitData[i] = 0.0;
        P->MotorCurrentFbkData[i] = 0.0;
        P->RotorVelocityFbkData[i] = 0.0;
        P->RotorPositionFbkData[i] = 0.0;
        P->TriggersData[i] = ui8FALSE;
        P->MotorCtrlModeData[i] = bldc_unknown,
        P->MilliStampData[i] = 0u;
        P->CommPeriodAvgData[i] = 0u;
        P->ConditionsData[i] = 0u;

    }
    P->DataSize = 0;
    P->DataIndex = 0;
}
void captureDataLogSamples(struct smartMotorStruct* smartMotorStructPtrIn)
{
    P->MilliStampData[P->DataIndex] = getMillis();
    P->CommPeriodAvgData[P->DataIndex] = P->CommPeriodAverage;

    P->TriggersData[P->DataIndex] = ((P->TriggerHold << 4) | (P->TriggerPush << 3) | (P->TriggerPositiveMove<<2) | (P->TriggerNegativeMove<<1) | (P->TriggerStop) );
    P->MotionStateData[P->DataIndex] = P->MotionState;
    P->MotorCtrlModeData[P->DataIndex] = P->MotorControlModeCMD;
    P->ConditionsData[P->DataIndex] = ((P->Connected << 3) | (P->OverTemp << 2) | (P->OverCurrent << 1) | (P->StatusOK));

    P->MotorPWMCmdData[P->DataIndex] = P->MotorPWMCmd;

    P->MotorCurrentLimitData[P->DataIndex] = P->MotorCurrentLimitCMD;
    P->MotorCurrentFbkData[P->DataIndex] = P->MotorCurrentFbk;
    P->RotorVelocityFbkData[P->DataIndex] = P->RotorVelocityFbk;
    P->RotorPositionFbkData[P->DataIndex] = P->RotorPositionFbk;
        
    if (P->DataSize < MAXDATAENTRIES)
        P->DataSize++;

    if (++P->DataIndex >= MAXDATAENTRIES)
        P->DataIndex = 0;
}
void smartMotorLoop(struct smartMotorStruct* smartMotorStructPtrIn)
{
    // Run the motor loop once per comm cycle
    if (P->MotorsCommPhase == 0)
    {
        int i;

        // Determine summary status
        if (P->Connected == ui8TRUE)
        {
            // Determine over temp status
            if (P->MotorTempDegC <= MOTORMAXTEMP)
                P->OverTemp = ui8FALSE;
            else
                P->OverTemp = ui8TRUE;

            // Determine over current status
            if (P->MotorCurrentLimitCMD > 0)
            {
                if ( (P->MotorCurrentFbk > MOTORIMAX) || (P->MotorCurrentFbk < -MOTORIMAX))
                {
                    P->OverCurrent = ui8TRUE;                
                }
                else
                {
                    P->OverCurrent = ui8FALSE;
                }
            }
            else
                P->OverCurrent = ui8FALSE;            

            // Status OK?
            if (P->OverCurrent == ui8FALSE && P->OverTemp == ui8FALSE)
                P->StatusOK = ui8TRUE;
            else
                P->StatusOK = ui8FALSE;

            

            // If we've only just connected, the position feedback history isn't valid yet
            if (P->ConnectedLast != P->Connected)
            {
                P->RotorPositionFbkLast = P->RotorPositionFbk;
                //P->MotionStateLast = P->MotionState;
            }


            // smart motor device in the, ccgripper context, *has two moves:
            // 1) Positive Move
            // 2) Negative Move, and 
            // 3) a Stop Move
        
            // 1) Detect Positive Motion
            if (POSITIONDELTA > MOTORSTALLEDTHRESH)
            {
                P->StalledCyclesCounter = 0;
                P->MotionState = sm_positive;                  
            }
            // 2) Detect Negative Motion
            else if (POSITIONDELTA < -MOTORSTALLEDTHRESH)
            {
                P->StalledCyclesCounter = 0;
                P->MotionState = sm_negative;
            }
            // 3) Detect Stalled or Stopped
            else
            {
                // it has 2 stopped motion states
                // 1) Stalled, the unplanned stop
                // 2) Stopped, the planned stop
                if (P->StalledCyclesCounter > ROTORSTALLEDCYCLES) 
                {             
                    P->StalledCyclesCounter = 0;
                    if (P->MotorControlModeCMD == bldc_spinpwm)
                    {
                        if (P->MotorPWMCmd == 0.0)
                            P->MotionState = sm_stopped;
                        else
                            P->MotionState = sm_stalled;
                    }    
                    else
                        P->MotionState = sm_stopped;                    
                }
                else
                    P->StalledCyclesCounter++;
            }


            // Latch H/L Pos limits
            if (P->LatchHighStopPos == ui8TRUE)
            {
                P->LatchHighStopPos = ui8FALSE;
                if (P->RotorPositionHighStop < P->RotorPositionFbk)
                {
                    P->RotorPositionHighStop = P->RotorPositionFbk;
                    P->MotionRangeLimit = (P->RotorPositionHighStop - P->RotorPositionLowStop);
                }
                    
            }
            else if (P->LatchLowStopPos == ui8TRUE)
            {
                P->LatchLowStopPos == ui8FALSE;
                if (P->RotorPositionLowStop > P->RotorPositionFbk)
                {
                    P->RotorPositionLowStop = P->RotorPositionFbk;
                    P->MotionRangeLimit = (P->RotorPositionHighStop - P->RotorPositionLowStop);
                }
                    

            }
        }
        else
        {
            P->StatusOK = ui8FALSE;
        }  

        


        // Run Non-Linear Control based on finger motion state
        switch (P->MotionState)
        {
        case sm_negative:
        {
            // Guard Velocity Fbk
            if (P->RotorVelocityFbk < -ROTORVMAX)
                P->MotorPWMCmd += PWMDELTA;
            else if (P->RotorVelocityFbk < -0.5 * ROTORVMAX)
                P->MotorPWMCmd += 0.5 * PWMDELTA;
            else if (P->RotorVelocityFbk > -ROTORVNOMINAL)
                P->MotorPWMCmd -= 0.25 * PWMDELTA;
            if (P->MotorPWMCmd >= 0.0)
                P->MotorPWMCmd = -PWMDELTA;

            
        }break;
        case sm_positive:
        {
            // Guard Velocity Fbk
            if (P->RotorVelocityFbk > ROTORVMAX)
                P->MotorPWMCmd -= PWMDELTA;
            else if (P->RotorVelocityFbk > 0.5 * ROTORVMAX)
                P->MotorPWMCmd -= 0.5 * PWMDELTA;
            else if (P->RotorVelocityFbk < ROTORVNOMINAL)
                P->MotorPWMCmd += 0.25 * PWMDELTA;
            if (P->MotorPWMCmd < 0.0)
                P->MotorPWMCmd = PWMDELTA;

            
        }break;
        case sm_stalled:
        {
            if (P->MotorPWMCmd > 0.0)
                P->MotorPWMCmd += PWMDELTA;
            else
                P->MotorPWMCmd -= PWMDELTA;
        }break;
        case sm_stopped:
        {

        }break;
        default:break;
        }

        // React to API Triggers for Motion If Connected and OK
        if (P->Connected == ui8TRUE && P->StatusOK == ui8TRUE)
        {
            // 1) Begin Positive Motion
            if (P->TriggerPositiveMove == ui8TRUE)
            {
                P->MotorControlModeCMD = bldc_spinpwm;
                P->MotorCurrentLimitCMD = MOTORINOMINAL;

                // if the high stop has not been determined, go slow
                if (P->RotorPositionHighStop == -LIMITSINITVAL)
                    P->MotorPWMCmd = PWMDELTA;
                else
                    P->MotorPWMCmd = PWMNOMINAL;
                P->RotorPositionFbkStart = P->RotorPositionFbk;
                captureDataLogSamples(P);
                P->TriggerPositiveMove = ui8FALSE;
            }
            // 2) Begin Negative Motion
            else if (P->TriggerNegativeMove == ui8TRUE)
            {
                P->MotorControlModeCMD = bldc_spinpwm;
                P->MotorCurrentLimitCMD = MOTORINOMINAL;

                // if the low stop has not been determined, go slow
                if (P->RotorPositionLowStop == LIMITSINITVAL)
                    P->MotorPWMCmd = -PWMDELTA;
                else
                    P->MotorPWMCmd = -PWMNOMINAL;
                P->RotorPositionFbkStart = P->RotorPositionFbk;
                captureDataLogSamples(P);
                P->TriggerNegativeMove = ui8FALSE;
            }
            // 3) Trigger Push
            else if (P->TriggerPush == ui8TRUE)
            {
                P->MotorCurrentLimitCMD = MOTORIPUSH;
                captureDataLogSamples(P);
                P->TriggerPush = ui8FALSE;
            }
            // 3) Trigger Hold
            else if (P->TriggerHold == ui8TRUE)
            {
                if (P->RotorPositionHighStop != -LIMITSINITVAL && P->RotorPositionLowStop != LIMITSINITVAL)
                {
                    P->MotorCurrentLimitCMD = MOTORIHOLD;
                    captureDataLogSamples(P);
                }             
                P->TriggerHold = ui8FALSE;
            }
            // 5) Begin Stop Motion
            if (P->TriggerStop == ui8TRUE)
            {
                //P->MotorPWMCmd = 0;   
                P->MotorCurrentLimitCMD = MOTORINOMINAL;
                P->MotorControlModeCMD = bldc_coast;
                P->TriggerPositiveMove = ui8FALSE;
                P->TriggerNegativeMove = ui8FALSE;
                captureDataLogSamples(P);
                P->TriggerStop = ui8FALSE;
            }

        }
        else// Clear triggers otherwise
        {
            P->TriggerPositiveMove = ui8FALSE;
            P->TriggerNegativeMove = ui8FALSE;
            P->TriggerStop = ui8TRUE;
        }
        

        

        // Scale PWM Output for Current Protection
        if (smartMotorStructPtrIn->MotorCurrentFbk > smartMotorStructPtrIn->MotorCurrentLimitCMD)
        {
            // Current is above setpoint value
            smartMotorStructPtrIn->MotorPWMCmd -= 2*PWMDELTA;
            if (smartMotorStructPtrIn->MotorPWMCmd < 0.0)
                smartMotorStructPtrIn->MotorPWMCmd = 0.0;

        }
        else if (smartMotorStructPtrIn->MotorCurrentFbk < -smartMotorStructPtrIn->MotorCurrentLimitCMD)
        {
            // Current is below setpoint value
            smartMotorStructPtrIn->MotorPWMCmd += 2*PWMDELTA;
            if (smartMotorStructPtrIn->MotorPWMCmd > 0.0)
                smartMotorStructPtrIn->MotorPWMCmd = 0.0;
        }

        // Protect the Final Outputs 
        if ( (smartMotorStructPtrIn->MotorControlModeCMD != bldc_spinpwm) ||
            (smartMotorStructPtrIn->StatusOK == ui8FALSE) 
            )
            smartMotorStructPtrIn->MotorPWMCmd = 0.0;
        //else if ((!(P->RotorPositionHighStop == -LIMITSINITVAL)) && 
        //    (P->RotorPositionFbk > (P->RotorPositionHighStop)) &&
        //    (smartMotorStructPtrIn->MotorPWMCmd > 0.0)
        //    )
        //    smartMotorStructPtrIn->MotorPWMCmd = 0.0;
        //else if ((!(P->RotorPositionLowStop == LIMITSINITVAL)) && 
        //    (P->RotorPositionFbk < (P->RotorPositionLowStop + OUTERRANGEMULTIPLIER * MOTORSTALLEDTHRESH)) &&
        //    (smartMotorStructPtrIn->MotorPWMCmd < 0.0)
        //    )
        //    smartMotorStructPtrIn->MotorPWMCmd = 0.0;
        else if (smartMotorStructPtrIn->MotorPWMCmd > PWMMAX)
            smartMotorStructPtrIn->MotorPWMCmd = PWMMAX;
        else if (smartMotorStructPtrIn->MotorPWMCmd < -PWMMAX)
            smartMotorStructPtrIn->MotorPWMCmd = -PWMMAX;
        
     
        // Capture History    
        if ((P->MotionStateLast != P->MotionState) || (P->StatusOKLast != P->StatusOK))
        {
            if (P->MotionState == sm_stalled || P->MotionState == sm_stopped)
            {
                P->RotorPositionFbkStop = P->RotorPositionFbk;
                P->MotionRange = (P->RotorPositionFbkStop - P->RotorPositionFbkStart);
            }
            captureDataLogSamples(P);
        }
        P->RotorPositionFbkLast = P->RotorPositionFbk;
        P->MotionStateLast = P->MotionState;
        P->StatusOKLast = P->StatusOK;
        P->ConnectedLast = P->Connected;
               
    }
}
#undef P
