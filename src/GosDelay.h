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
 *         Simple Delay.
 *
 * For more information about gosDelay visit:
 *     http://gommagomma.net/gos95/vst
 * This code comes with ABSOLUTELY NO WARRANTY.
 */

 
#ifndef __EFFECT_TEMPLATE_H__
#define __EFFECT_TEMPLATE_H__

#include <audioeffectx.h>
#include "Program.h"


#define DELAY_MAX_LENGTH 1


class GosDelay : public AudioEffectX
{
public:
    GosDelay(audioMasterCallback audioMaster);
    ~GosDelay();

    // Processing
    virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames);
    //virtual void resume();

    // Program
    virtual void setProgram(VstInt32 program);
    virtual void setProgramName(char* name);
    virtual void getProgramName(char* name);
    virtual bool getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text);

    // Parameters
    virtual void setParameter(VstInt32 index, float value);
    virtual float getParameter(VstInt32 index);
    virtual void getParameterLabel(VstInt32 index, char* label);
    virtual void getParameterDisplay(VstInt32 index, char* text);
    virtual void getParameterName(VstInt32 index, char* text);

	// plug-in infos
    virtual bool getEffectName(char* name);
	virtual bool getProductString(char* text);
    virtual bool getVendorString(char* text);
    virtual VstInt32 getVendorVersion() { return 1000;};
    virtual VstPlugCategory getPlugCategory() { return kPlugCategEffect; }

protected:
    void  initialize();
    float fDelayLevel, fDelayTime, fFeedback;
	float fSampleRate;
    float *delayBufferStartLeft, *delayBufferLeft;
	float *delayBufferStartRight, *delayBufferRight;
	int   bufferIndex, bufferSize;
    Program *programs;
};


#endif /* !defined(__EFFECT_TEMPLATE_H__) */
