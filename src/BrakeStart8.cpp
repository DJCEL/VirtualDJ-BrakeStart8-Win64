#include "BrakeStart8.h"

//---------------------------------------------------------------------------
CBrakeStart8::CBrakeStart8()
{
	m_StartPos = -1;
	m_Status = -1;
	m_brakespeed = 0;
	m_startupspeed = 0;
	m_rampStartTime = 0;
	m_rampStopTime = 0.0f;
	m_BufferOut = nullptr;
	m_StartTime = 0;
	m_StopTime = 0;
	m_bBeats = 0;
	ZeroMemory(m_ButtonDown, 8 * sizeof(int));
	ZeroMemory(m_SliderValue, 2 * sizeof(float));
}
//---------------------------------------------------------------------------
CBrakeStart8::~CBrakeStart8()
{

}
//---------------------------------------------------------------------------
HRESULT VDJ_API CBrakeStart8::OnLoad()
{
	HRESULT hr = S_FALSE;	
	
	hr = DeclareParameterSwitch(&m_bBeats,ID_SWITCH_1,"Beats","BT",0.0f);
	hr = DeclareParameterButton(&m_ButtonDown[0],ID_BUTTON_1,"200ms","200ms");
	hr = DeclareParameterButton(&m_ButtonDown[1],ID_BUTTON_2,"300ms","300ms");
	hr = DeclareParameterButton(&m_ButtonDown[2],ID_BUTTON_3,"400ms","400ms");
	hr = DeclareParameterButton(&m_ButtonDown[3],ID_BUTTON_4,"500ms","500ms");
	hr = DeclareParameterButton(&m_ButtonDown[4],ID_BUTTON_5,"1s","1s");
	hr = DeclareParameterButton(&m_ButtonDown[5],ID_BUTTON_6,"12s","12s");
	hr = DeclareParameterButton(&m_ButtonDown[6],ID_BUTTON_7,"1bt","1bt");
	hr = DeclareParameterButton(&m_ButtonDown[7],ID_BUTTON_8,"2bt","2bt");
	hr = DeclareParameterSlider(&m_SliderValue[0],ID_SLIDER_1,"Stop Length","STOP",0.0625f); // 2s by default
    hr = DeclareParameterSlider(&m_SliderValue[1],ID_SLIDER_2,"Start Length","START",0.0625f); // 2s by default

	OnParameter(ID_INIT);
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CBrakeStart8::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
	infos->Author = "DJ CEL";
	infos->PluginName = "BrakeStart8";
	infos->Description = "Plugin Turntable Brake/Start.";
	infos->Version = "2.3";
    infos->Flags = 0x00;

	return S_OK;
}
//---------------------------------------------------------------------------
ULONG  VDJ_API CBrakeStart8::Release()
{
	HRESULT hr;
	SAFE_DELETE_ARRAY(m_BufferOut)

	if(m_rampStopTime>100)
	{
		char cmd[64]="";
		sprintf_s(cmd,64*sizeof(char),"setting 'rampStopTime' 0.0");
		hr = SendCommand(cmd);
		hr = SendCommand("save_config");
	}
		
	if(m_rampStartTime>100)
	{
		char cmd[64]="";
		sprintf_s(cmd, 64 * sizeof(char),"setting 'rampStartTime' 0.0");
		hr = SendCommand(cmd);
		hr = SendCommand("save_config");
	}

	delete this;
	return 0;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CBrakeStart8::OnParameter(int id)
{
	if (id == ID_INIT)
	{
		for (int i = ID_SLIDER_1; i <= ID_SLIDER_2; i++) OnSlider(i);
	}
	else
	{
		OnSlider(id);
		OnButton(id);
	}
		
	return S_OK;
}
//---------------------------------------------------------------------------
void CBrakeStart8::OnSlider(int id)
{
	float SliderVal;
	switch (id)
	{
		case ID_SLIDER_1:
			SliderVal = m_SliderValue[0];
			if (m_bBeats)
			{
				if (SliderVal == 0.0f)                          m_StopTime = 0.125f;
				else if (SliderVal > 0.0f && SliderVal < 0.125f)   m_StopTime = 0.25f;
				else if (SliderVal >= 0.125f && SliderVal < 0.25f)    m_StopTime = 0.5f;
				else if (SliderVal >= 0.25f && SliderVal < 0.375f)   m_StopTime = 1.0f;
				else if (SliderVal >= 0.375f && SliderVal < 0.5f)     m_StopTime = 2.0f;
				else if (SliderVal >= 0.5f && SliderVal < 0.625f)   m_StopTime = 4.0f;
				else if (SliderVal >= 0.625 && SliderVal < 0.75f)    m_StopTime = 6.0f;
				else if (SliderVal >= 0.75f && SliderVal < 0.875f)   m_StopTime = 8.0f;
				else if (SliderVal >= 0.875f && SliderVal < 1.0f)     m_StopTime = 16.0f;
				else if (SliderVal == 1.0f)                          m_StopTime = 32.0f;
			}
			else m_StopTime = SliderVal * MAX_TIME;
			break;

		case ID_SLIDER_2:
			SliderVal = m_SliderValue[1];
			if (m_bBeats)
			{
				if (SliderVal == 0.0f)                          m_StartTime = 0.125f;
				else if (SliderVal > 0.0f && SliderVal < 0.125f)   m_StartTime = 0.25f;
				else if (SliderVal >= 0.125f && SliderVal < 0.25f)    m_StartTime = 0.5f;
				else if (SliderVal >= 0.25f && SliderVal < 0.375f)   m_StartTime = 1.0f;
				else if (SliderVal >= 0.375f && SliderVal < 0.5f)     m_StartTime = 2.0f;
				else if (SliderVal >= 0.5f && SliderVal < 0.625f)   m_StartTime = 4.0f;
				else if (SliderVal >= 0.625 && SliderVal < 0.75f)    m_StartTime = 6.0f;
				else if (SliderVal >= 0.75f && SliderVal < 0.875f)   m_StartTime = 8.0f;
				else if (SliderVal >= 0.875f && SliderVal < 1.0f)     m_StartTime = 16.0f;
				else if (SliderVal == 1.0f)                          m_StartTime = 32.0f;
			}
			else m_StartTime = SliderVal * MAX_TIME;
			break;
	}
}
//---------------------------------------------------------------------------
void CBrakeStart8::OnButton(int id)
{
	switch (id)
	{
	case ID_BUTTON_1:
	case ID_BUTTON_2:
	case ID_BUTTON_3:
	case ID_BUTTON_4:
	case ID_BUTTON_5:
	case ID_BUTTON_6:
		m_bBeats = 0;
		if (m_ButtonDown[0]) m_StopTime = m_StartTime = 0.2f;
		else if (m_ButtonDown[1]) m_StopTime = m_StartTime = 0.3f;
		else if (m_ButtonDown[2]) m_StopTime = m_StartTime = 0.4f;
		else if (m_ButtonDown[3]) m_StopTime = m_StartTime = 0.5f;
		else if (m_ButtonDown[4]) m_StopTime = m_StartTime = 1.0f;
		else if (m_ButtonDown[5]) m_StopTime = m_StartTime = 12.0f;
		m_SliderValue[0] = m_StopTime / MAX_TIME;
		m_SliderValue[1] = m_StartTime / MAX_TIME;
		break;

	case ID_BUTTON_7:
	case ID_BUTTON_8:
		m_bBeats = 1;
		if (m_ButtonDown[6]) m_StopTime = m_StartTime = 1.0f;
		else if (m_ButtonDown[7]) m_StopTime = m_StartTime = 2.0f;
		m_SliderValue[0] = m_StopTime / MAX_TIME;
		m_SliderValue[1] = m_StartTime / MAX_TIME;
		break;
	}
}
//---------------------------------------------------------------------------
HRESULT CBrakeStart8::OnGetParameterString(int id, char *outParam, int outParamSize)
{	
	switch(id)
	{
		case ID_SLIDER_1:
			if(m_bBeats)
			{
					if (m_StopTime == 0.125f) sprintf_s(outParam, outParamSize, "1/8 beat");
				else if (m_StopTime == 0.25f) sprintf_s(outParam, outParamSize, "1/4 beat");
				else if (m_StopTime == 0.5f) sprintf_s(outParam, outParamSize, "1/2 beat");
				else if (m_StopTime == 1.0f) sprintf_s(outParam, outParamSize, "1 beat");
				else if (m_StopTime == 2.0f) sprintf_s(outParam, outParamSize, "2 beats");
				else if (m_StopTime == 4.0f) sprintf_s(outParam, outParamSize, "4 beats");
				else if (m_StopTime == 6.0f) sprintf_s(outParam, outParamSize, "6 beats");
				else if (m_StopTime == 8.0f) sprintf_s(outParam, outParamSize, "8 beats");
				else if (m_StopTime == 16.0f) sprintf_s(outParam, outParamSize, "16 beats");
				else if (m_StopTime == 32.0f) sprintf_s(outParam, outParamSize, "32 beats");
			}
			else sprintf_s(outParam, outParamSize, "%.2f seconds", m_StopTime);
			break;

		case ID_SLIDER_2:
			if(m_bBeats)
			{
					 if (m_StartTime == 0.125f) sprintf_s(outParam, outParamSize, "1/8 beat");
				else if (m_StartTime == 0.25f) sprintf_s(outParam, outParamSize, "1/4 beat");
				else if (m_StartTime == 0.5f) sprintf_s(outParam, outParamSize, "1/2 beat");
				else if (m_StartTime == 1.0f) sprintf_s(outParam, outParamSize, "1 beat");
				else if (m_StartTime == 2.0f) sprintf_s(outParam, outParamSize, "2 beats");
				else if (m_StartTime == 4.0f) sprintf_s(outParam, outParamSize, "4 beats");
				else if (m_StartTime == 6.0f) sprintf_s(outParam, outParamSize, "6 beats");
				else if (m_StartTime == 8.0f) sprintf_s(outParam, outParamSize, "8 beats");
				else if (m_StartTime == 16.0f) sprintf_s(outParam, outParamSize, "16 beats");
				else if (m_StartTime == 32.0f) sprintf_s(outParam, outParamSize, "32 beats");
			}
			else sprintf_s(outParam, outParamSize, "%.2f seconds", m_StartTime);
			break;
	}

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CBrakeStart8::OnStart()
{
	HRESULT hr1 = S_FALSE;
	HRESULT hr2 = S_FALSE;
	HRESULT hr3 = S_FALSE;
	HRESULT hr4 = S_FALSE;
	HRESULT hr5 = S_FALSE;
	HRESULT hr6 = S_FALSE;
	HRESULT hr7 = S_FALSE;
	HRESULT hr8 = S_FALSE;
	HRESULT hr9 = S_FALSE;
	HRESULT hr10 = S_FALSE;
	HRESULT hr11 = S_FALSE;
	double res = 0;

	m_StartPos = SongPos;

	hr1 = GetInfo("setting 'rampStopTime'",&res);
	if(hr1==S_OK) m_rampStopTime = res;
	else m_rampStopTime = -1.0f;

	hr2 = GetInfo("setting 'rampStartTime'",&res);
	if(hr2==S_OK) m_rampStartTime = res;
	else m_rampStartTime = -1.0f;

	hr3 = GetInfo("brakespeed",&res);
	if(hr3==S_OK) m_brakespeed = res;
	else m_brakespeed = -1.0f;
 
	hr4 = GetInfo("startupspeed",&res);
	if(hr4==S_OK) m_startupspeed = res;
	else m_startupspeed = -1.0f;

	if(m_brakespeed!=0 || m_startupspeed!=0)
	{
		hr5 = SendCommand("brakespeed 0%");
		hr6 = SendCommand("startupspeed 0%");
	}

	if(m_rampStopTime!=0 || m_rampStartTime!=0 )
	{
		hr7 = SendCommand("setting 'rampStopTime' 0.0");
		hr8 = SendCommand("setting 'rampStartTime' 0.0");
		hr9 = SendCommand("save_config");
	}
	
    hr10 = GetInfo("play",&res);
	if(hr10==S_OK) m_Status = (int) res;
	else m_Status = -1;

	if(m_Status == 0) hr11 = SendCommand("play");

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CBrakeStart8::OnStop()
{
	HRESULT hr1 = S_FALSE;
	HRESULT hr2 = S_FALSE;
	HRESULT hr3 = S_FALSE;
	HRESULT hr4 = S_FALSE;
	HRESULT hr5 = S_FALSE;

	m_StartPos = -1;

	if(m_brakespeed!=0 || m_startupspeed!=0)
	{
		char cmd[64]="";
		sprintf_s(cmd,64*sizeof(char),"brakespeed %.2f", m_brakespeed);
		hr1 = SendCommand(cmd);
		sprintf_s(cmd, 64 * sizeof(char), "startupspeed %.2f", m_startupspeed);
		hr2 = SendCommand(cmd);
	}

	if(m_rampStopTime!=0 || m_rampStartTime!=0)
	{
		if(m_rampStopTime>100) m_rampStopTime  = 0.0f;
		if(m_rampStartTime>100) m_rampStartTime  = 0.0f;

		char cmd[64]="";
		sprintf_s(cmd, 64 * sizeof(char), "setting 'rampStopTime' %.2f", m_rampStopTime);
		hr3 = SendCommand(cmd);
		sprintf_s(cmd, 64 * sizeof(char), "setting 'rampStartTime' %.2f", m_rampStartTime);
		hr4 = SendCommand(cmd);
		hr5 = SendCommand("save_config");
	}
	return S_OK;
}
//---------------------------------------------------------------------------
short* CBrakeStart8::OnGetSongBuffer(int pos, int nb)
{
	HRESULT hr = S_FALSE;

	SAFE_DELETE_ARRAY(m_BufferOut);
	m_BufferOut = new short[2 * nb];
	ZeroMemory(m_BufferOut,2*nb*sizeof(short));

	if(m_Status == 0) //if song is stopped
	{
		hr = OnProcessStart(m_BufferOut, pos, nb);
	}
	else if(m_Status == 1) //if song is playing
	{
		hr = OnProcessStop(m_BufferOut, pos,nb);
	}

	return m_BufferOut;
}
//---------------------------------------------------------------------------
HRESULT CBrakeStart8::OnProcessStart(short *buffer,int pos,int nb)
{
	HRESULT hr = S_FALSE;
	short *samples = NULL;
	int NewPos = 0;
	int NewNb = 0;
	int i = 0;
	int j = 0;
	double p = 0;
	int pj = 0;
	int n = 0;
	double r = 0;
	double volume = 0;
	int length = 0;
	double sLeftCur, sRightCur, sLeftNext, sRightNext, sLeft, sRight = 0;
	int SizeToEnd = 0;
	int EndPos = 0;

	if(m_bBeats) length = int(m_StartTime * SongBpm);
	else length = int(m_StartTime * SampleRate);

	EndPos = m_StartPos + length; // Position of the end of the effect

    if(pos < m_StartPos) // Before the beginning of the effect
	{
		ZeroMemory(buffer,2*nb*sizeof(short));
	}
	else if(pos >= m_StartPos && pos < EndPos) // Between the beginning of the effect and the end of it
	{
		SizeToEnd = EndPos - pos; // Number of samples until the end of the effect
		n = min(SizeToEnd,nb); // always 'nb' except at the end of the effect

		NewPos = int(m_StartPos + length / 2.0f);
		NewNb = int(length / 2.0f) + 1;

		hr = GetSongBuffer(NewPos, NewNb,&samples); // we get 2 * NewNb samples from NewPos
		if(hr!=S_OK) return S_FALSE;

		for(i = 0; i < n ; i++)
		{
			j = pos + i - m_StartPos;
			volume = j / (double)length; // increase of the volume
			p = (double) SQ(j) / (2.0f * length); // increase of the pitch (from 0 to length/2)
			pj = (int) floor(p);
			r = p - pj;

			sLeftCur   = (double) samples[2*pj];
			sRightCur  = (double) samples[2*pj+1];
			sLeftNext  = (double) samples[2*pj+2];
			sRightNext = (double) samples[2*pj+3];

			sLeft  = sLeftCur * (1-r) + sLeftNext  * r;
			sRight = sRightCur * (1-r) + sRightNext * r;

			buffer[2*i]   = (short)( sLeft  * volume ); // Left Channel
			buffer[2*i+1] = (short)( sRight * volume ); // Right Channel
		}

	
		if(n < nb) // current 'nb' buffer not fully filled so we need to add samples (from n to nb)
		{
			NewPos = pos + n;
			NewNb = nb - n;

			hr = GetSongBuffer(NewPos, NewNb, &samples); // on recupere 2*NewNb samples à partir de NewPos
		    if(hr!=S_OK) return S_FALSE;

			short *dst = buffer + 2 * n;
		
		    memcpy(dst,samples,2*NewNb *sizeof(short)); // on recopie 2*NewNb samples
		}
	}
	else if(pos >= EndPos) // After the end of the effect
	{
		hr = GetSongBuffer(pos,nb,&samples);
		if(hr!=S_OK) return S_FALSE;
		
		memcpy(buffer,samples, 2*nb*sizeof(short));
		
		hr = SendCommand("effect_active off");
	}

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT CBrakeStart8::OnProcessStop(short *buffer, int pos,int nb)
{
	HRESULT hr = S_OK;
	short *samples = NULL;
	int NewPos = 0;
	int NewNb = 0;
	int i = 0;
	int j = 0;
	double p = 0;
	int pj = 0;
	int n = 0;
	double r = 0;
	double volume = 0;
	int length = 0;
	double sLeftCur, sRightCur, sLeftNext, sRightNext, sLeft, sRight = 0;
	int SizeToEnd = 0;
	int EndPos = 0;

	if(m_bBeats) length = int(m_StopTime * SongBpm);
	else length = int(m_StopTime * SampleRate);

	EndPos = m_StartPos + length; // Position of the end of the effect

	if(pos < m_StartPos) // Avant le début de l'effet
	{
		hr = GetSongBuffer(pos,nb,&samples); // on recupere 2*nb samples (nb left, nb right)
		if(hr!=S_OK) return S_FALSE;
		
		memcpy(buffer,samples, 2*nb*sizeof(short)); // on recopie 2*nb samples
	}
	else if(pos >= m_StartPos && pos < EndPos) // Entre le debut de l'effet et la fin de l'effet
	{
		SizeToEnd = EndPos - pos; //  Number of samples until the end of the effect
		n = min(SizeToEnd,nb);   // always 'nb' except at the end of the effect
		
		NewPos = m_StartPos;
		NewNb = int(length / 2.0f) + 1;

		hr = GetSongBuffer(NewPos, NewNb,&samples); // on recupere 2*NewNb samples a partir de NewPos
		if(hr!=S_OK) return S_FALSE;

		for(i = 0; i < n ; i++)
		{
			j = pos + i - m_StartPos;	
			volume = (length - j) / (double)length;  // decrease of the volume
			p = j - (double) SQ(j) / (2.0f * length); // decrease of the pitch (from 0 to length/2)
			pj = (int) floor(p);
			r = p - pj;

			sLeftCur   = (double) samples[2*pj];
			sRightCur  = (double) samples[2*pj+1];
			sLeftNext  = (double) samples[2*pj+2];
			sRightNext = (double) samples[2*pj+3];

			sLeft  = sLeftCur  * (1-r) + sLeftNext  * r;
			sRight = sRightCur * (1-r) + sRightNext * r;

			buffer[2*i]   = (short)( sLeft  * volume); // Left Channel
			buffer[2*i+1] = (short)( sRight * volume); // Right Channel
		}

		if(n < nb) // current 'nb' buffer not fully filled so we need to add samples (from n to nb)
		{
			NewNb = nb - n;
			short *dst = buffer + 2 * n;

			ZeroMemory(dst, 2*NewNb*sizeof(short));
		}
	}
	
	if(pos >= EndPos) // After the end of the effect
	{
		hr = SendCommand("Pause");
		hr = SendCommand("effect_active off");
	}

	return S_OK;	
}

