#ifndef BRAKESTART8_H
#define BRAKESTART8_H

#include "VdjDsp8.h"

#include <stdio.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include <cstdlib>

//---------------------------------------------------------------------------
class CBrakeStart8 : public IVdjPluginBufferDsp8
{
public:
	CBrakeStart8();
	~CBrakeStart8();
	HRESULT VDJ_API OnLoad();
	HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *infos);
	ULONG   VDJ_API Release();
	HRESULT VDJ_API OnParameter(int id);
	HRESULT VDJ_API OnGetParameterString(int id, char *outParam, int outParamSize);
	HRESULT VDJ_API OnStart();
	HRESULT VDJ_API OnStop();
	short*  VDJ_API OnGetSongBuffer(int pos, int nb);
	
private:
	void OnButton(int button);
	void OnSlider(int slider);
	HRESULT OnProcessStart(short *buffer,int pos,int nb);
	HRESULT OnProcessStop(short *buffer, int pos,int nb);
	
	int m_ButtonDown[8];
	float m_SliderValue[2];
	int m_bBeats;

	int m_StartPos;
	float m_StartTime;
	float m_StopTime;
	int m_Status;
	short* m_BufferOut;
	double m_brakespeed;
	double m_startupspeed;
	double m_rampStartTime;
	double m_rampStopTime;

protected:
	typedef enum _ID_Interface
	{
		ID_INIT,
		ID_BUTTON_1,
		ID_BUTTON_2,
		ID_BUTTON_3,
		ID_BUTTON_4,
		ID_BUTTON_5,
		ID_BUTTON_6,
		ID_SLIDER_1,
		ID_SLIDER_2,
		ID_SWITCH_1,
		ID_BUTTON_7,
		ID_BUTTON_8
	} ID_Interface;

	#define min(a,b) (((a) < (b)) ? (a) : (b))
	#define SQ(x)  (x * x)
	const float MAX_TIME = 32.0f;	// slider run between 0 and 32 secondes

};

#endif /* BRAKESTART8_H */