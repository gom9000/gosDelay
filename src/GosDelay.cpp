/*
 * GoS'95  gosDelay
 *   ________        _________________.________
 *  /  _____/  ____ /   _____/   __   \   ____/
 * /   \  ___ /  _ \\_____  \\____    /____  \
 * \    \_\  (  <_> )        \  /    //       \
 *  \______  /\____/_______  / /____//______  /
 *         \/              \/               \/
 * Copyright (c) 2009 by Alessandro Fraschetti
 * (gos95@gommagomma.net). All Rights Reserved.
 *
 * gosDelay:
 *
 * For more information about gosDelay visit:
 *     http://gommagomma.net/gos95/vst
 * This code comes with ABSOLUTELY NO WARRANTY.
 */


#include <stdio.h>
#include <string.h>
#include "GosDelay.h"


/*
 * Creates the plug-in.
 */
GosDelay::GosDelay(audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, kNumPrograms, kNumParams)
{
	programs = new Program[numPrograms];
    fDelayLevel = fDelayTime = fFeedback = .0f;
	fSampleRate = getSampleRate();

    if (programs) setProgram(0);

	setNumInputs(2);
	setNumOutputs(2);
	setUniqueID('gDel');
	canProcessReplacing();
	canDoubleReplacing();

	initialize();
}


/*
 * Destroyes the plug-in.
 * Releases al program resources.
 */
GosDelay::~GosDelay()
{
    if (programs) delete[] programs;
	if (delayBufferLeft) delete[] delayBufferLeft;
	if (delayBufferRight) delete[] delayBufferRight;
}


/*
 * Changes the current program.
 */
void GosDelay::setProgram(VstInt32 program)
{
	Program* p = &programs[program];

	curProgram = program;
	setParameter(kDelayLevel, p->fDelayLevel);	
	setParameter(kDelayTime, p->fDelayTime);
	setParameter(kFeedback, p->fFeedback);
}


/*
 * Sets the name of the current program.
 */
void GosDelay::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}


/*
 * Gets the name of the current program.
 */
void GosDelay::getProgramName(char* name)
{
    if (!strcmp(programs[curProgram].name, "Init"))
        sprintf(name, "%s %d", programs[curProgram].name, curProgram + 1);
    else
        vst_strncpy(name, programs[curProgram].name, kVstMaxProgNameLen);
}


/*
 * Returns the name of the program referenced by index.
 */
bool GosDelay::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
	{
        vst_strncpy(text, programs[index].name, kVstMaxProgNameLen);
        return true;
	}
    return false;
}


/*
 * Sets the value of the parameter referenced by index.
 */
void GosDelay::setParameter(VstInt32 index, float value)
{
	Program* p = &programs[curProgram];

	switch (index)
	{
		case kDelayLevel :
		    fDelayLevel = p->fDelayLevel = value;
			break;
		case kDelayTime :
		    fDelayTime = p->fDelayTime = value;
			bufferSize = int(fDelayTime * fSampleRate);
			break;
		case kFeedback :
		    fFeedback = p->fFeedback = value;
			break;
	}
}


/*
 * Gets the value of the parameter referenced by index.
 */
float GosDelay::getParameter(VstInt32 index)
{
	float v = 0;

	switch (index)
	{
		case kDelayLevel : v = fDelayLevel;	break;
		case kDelayTime : v = fDelayTime; break;
		case kFeedback : v = fFeedback; break;
	}
	return v;
}



/*
 * Returns the parameter label referenced by index.
 */
void GosDelay::getParameterLabel(VstInt32 index, char *label)
{
	switch (index)
	{
		case kDelayLevel : vst_strncpy(label, "dB", kVstMaxParamStrLen); break;
		case kDelayTime : vst_strncpy(label, "ms", kVstMaxParamStrLen); break;
		case kFeedback :
		    if (fFeedback >0)
			    vst_strncpy(label, "dB", kVstMaxParamStrLen);
			else
			    vst_strncpy(label, "Off", kVstMaxParamStrLen);
			break;
	}
}


/*
 * Returns the data-type of parameter referenced by index.
 */
void GosDelay::getParameterDisplay(VstInt32 index, char *text)
{
	switch (index)
	{
	    case kDelayLevel :
		    dB2string(fDelayLevel, text, kVstMaxParamStrLen);
			break;
		case kDelayTime :
		    float2string(fDelayTime, text, kVstMaxParamStrLen);
		    break;
		case kFeedback :
		    if (fFeedback > 0) dB2string(fFeedback, text, kVstMaxParamStrLen);
		    break;
	}
}


/*
 * Returns name of the parameter referenced by index.
 */
void GosDelay::getParameterName(VstInt32 index, char *label)
{
	switch (index)
	{
		case kDelayLevel : vst_strncpy (label, "Level", kVstMaxParamStrLen); break;
		case kDelayTime : vst_strncpy (label, "Delay", kVstMaxParamStrLen); break;
		case kFeedback : vst_strncpy (label, "Feedback", kVstMaxParamStrLen); break;
	}
}



/*
 * Returns the plug-in name.
 */
bool GosDelay::getEffectName(char* name)
{
	vst_strncpy (name, "gosDelay", kVstMaxEffectNameLen);
	return true;
}


/*
 * Returns the plug-in product name.
 */
bool GosDelay::getProductString(char* text)
{
	vst_strncpy (text, "gos Delay", kVstMaxProductStrLen);
	return true;
}


/*
 * Returns the plug-in vendor name.
 */
bool GosDelay::getVendorString (char* text)
{
	vst_strncpy (text, "GoS'95", kVstMaxVendorStrLen);
	return true;
}


//-----------------------------------------------------------------------------------------

void GosDelay::initialize()
{
    bufferSize = int(DELAY_MAX_LENGTH * fSampleRate);
	bufferIndex = 0;

    delayBufferLeft = new float[bufferSize];
    delayBufferRight = new float[bufferSize];

    for(int i=0; i<bufferSize; i++) {
        delayBufferLeft[i] = delayBufferRight[i] = .0;
	}

	delayBufferStartLeft = delayBufferLeft;
	delayBufferStartRight = delayBufferRight;
}


void GosDelay::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    while (--sampleFrames >= 0)
    {
   	    *out1++ = *in1 + *delayBufferLeft;
		*out2++ = *in2 + *delayBufferRight;

        *delayBufferLeft = (*in1++ + *delayBufferLeft++ * fFeedback) * fDelayLevel;
		*delayBufferRight = (*in2++ + *delayBufferRight++ * fFeedback) * fDelayLevel;

        if (++bufferIndex >= bufferSize) {
            delayBufferLeft = delayBufferStartLeft;
			delayBufferRight = delayBufferStartRight;
			bufferIndex = 0;
		}
    }
}

//-----------------------------------------------------------------------------------------
void GosDelay::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

    while (--sampleFrames >= 0)
    {
        *out1++ = *in1 + (double)*delayBufferLeft;
		*out2++ = *in2 + (double)*delayBufferRight;

        *delayBufferLeft = (*in1++ + *delayBufferLeft++ *fFeedback) * fDelayLevel;
		*delayBufferRight = (*in2++ + *delayBufferRight++ * fFeedback) * fDelayLevel;

        if (++bufferIndex >= bufferSize) {
            delayBufferLeft = delayBufferStartLeft;
			delayBufferRight = delayBufferStartRight;
			bufferIndex = 0;
		}
    }
}
