// headers
// // standard
#include <cstdio>
#include <iostream>
#include <limits>
// // 3rd-party
#include "../lib/portaudio/include/portaudio.h"
// defines
#define dProcECode(vECode)                           \
	({                                               \
		if(vECode != cECodeNone)                     \
		{                                            \
			auto vEText = ::Pa_GetErrorText(vECode); \
			std::printf("error: %s\n", vEText);      \
			return vECode;                           \
		}                                            \
	}) // dProcECode
// aliases
using tSample		= char;
using tSampleLimits = ::std::numeric_limits<tSample>;
using tECode		= ::PaError;
// consdef
constexpr auto cNull = nullptr;
// // errors
constexpr auto cECodeNone	   = ::paNoError;
constexpr auto cECodeADevIndex = 0x10;
// // buffers
constexpr auto cBufferFrameCount = paFramesPerBufferUnspecified;
// // channels
constexpr unsigned cIChanCount = 1;
constexpr unsigned cOChanCount = 2;
// // samples
constexpr unsigned		   cSampleRate = 8'000;
constexpr ::PaSampleFormat cSampleEnum = paInt8;
// // // limits
constexpr tSample cSampleMin = tSampleLimits::min();
constexpr tSample cSampleMax = tSampleLimits::max();
constexpr tSample cSampleMid = 0;
// // // steps
constexpr int cSampleNegStep = -1;
constexpr int cSamplePosStep = +1;
// typedef
typedef struct tTestData
{
	tSample vPhaseL;
	tSample vPhaseR;
} tTestData;
// actions
static int fWaveSawCallback(
	const void						 *pBufferI,
	void							 *pBufferO,
	unsigned long					  vBufferFrameCount,
	const ::PaStreamCallbackTimeInfo *pTimeInfo,
	::PaStreamCallbackFlags			  vStatusFlags,
	void							 *pUserData
)
{
	tTestData *pData  = (tTestData *)pUserData;
	tSample	  *pIterO = (tSample *)pBufferO;
	tSample	  *pIterI = (tSample *)pBufferI;
	// write to the buffer
	for(unsigned vIter = 0; vIter < vBufferFrameCount; vIter++)
	{
		*(pIterO++) = pData->vPhaseL;
		*(pIterO++) = pData->vPhaseR;
		// simple sawtooth phaser that ranges between min and max
		pData->vPhaseL += cSampleNegStep;
		if(pData->vPhaseL <= cSampleMin) { pData->vPhaseL -= cSampleMin; }
		pData->vPhaseR += cSamplePosStep;
		if(pData->vPhaseR >= cSampleMax) { pData->vPhaseR -= cSampleMax; }
	} // write to the buffer
	return cECodeNone;
} // fWaveSawCallback
static int fEchoCallback(
	const void						 *pBufferI,
	void							 *pBufferO,
	unsigned long					  vBufferFrameCount,
	const ::PaStreamCallbackTimeInfo *pTimeInfo,
	::PaStreamCallbackFlags			  vStatusFlags,
	void							 *pUserData
)
{
	tSample *pIterO = (tSample *)pBufferO;
	tSample *pIterI = (tSample *)pBufferI;
	// write to the buffer
	for(unsigned vIter = 0; vIter < vBufferFrameCount; vIter++)
	{
		*(pIterO++) = *(pIterI);
		*(pIterO++) = *(pIterI);
		pIterI++;
	} // write to the buffer
	return cECodeNone;
} // fEchoCallback
static int fIModifyCallback(
	const void						 *pBufferI,
	void							 *pBufferO,
	unsigned long					  vBufferFrameCount,
	const ::PaStreamCallbackTimeInfo *pTimeInfo,
	::PaStreamCallbackFlags			  vStatusFlags,
	void							 *pUserData
)
{
	tSample *pIterO = (tSample *)pBufferO;
	tSample *pIterI = (tSample *)pBufferI;
	// write to the buffer
	for(unsigned vIter = 0; vIter < vBufferFrameCount; vIter++)
	{
		*(pIterO++) = *(pIterI);
		*(pIterO++) = *(pIterI);
		pIterI++;
	} // write to the buffer
	return cECodeNone;
} // fEchoCallback
int fPrintADevInfo(const ::PaDeviceInfo *pADevInfo)
{
	// meta
	int				 vADevInfoVnum = pADevInfo->structVersion;
	const char		*vADevName	   = pADevInfo->name;
	::PaHostApiIndex vADevApiIndex = pADevInfo->hostApi;
	std::printf("[Vnum]=(%d)=[Vnum]\n", vADevInfoVnum);
	std::printf("[Name]=(%s)=[Name]\n", vADevName);
	std::printf("[ApiIndex]=(%d)=[ApiIndex]\n", vADevApiIndex);
	// chan
	int vADevIChanCount = pADevInfo->maxInputChannels;
	int vADevOChanCount = pADevInfo->maxOutputChannels;
	std::printf("[IChanCount]=(%d)=[IChanCount]\n", vADevIChanCount);
	std::printf("[OChanCount]=(%d)=[OChanCount]\n", vADevOChanCount);
	// latency
	::PaTime vADevILateLower = pADevInfo->defaultLowInputLatency;
	::PaTime vADevILateUpper = pADevInfo->defaultHighInputLatency;
	::PaTime vADevOLateLower = pADevInfo->defaultLowOutputLatency;
	::PaTime vADevOLateUpper = pADevInfo->defaultHighOutputLatency;
	std::printf("[ILateLower]=(%d)=[ILateLower]\n", vADevILateLower);
	std::printf("[ILateUpper]=(%d)=[ILateUpper]\n", vADevILateUpper);
	std::printf("[OLateLower]=(%d)=[OLateLower]\n", vADevOLateLower);
	std::printf("[OLateUpper]=(%d)=[OLateUpper]\n", vADevOLateUpper);
	// samples
	double vADevSampleRate = pADevInfo->defaultSampleRate;
	std::printf("[SampleRate]=(%d)=[SampleRate]\n", vADevSampleRate);
	// final
	return cECodeNone;
} // fPrintADevInfo
int fPrintADevInfo(int vADevIndex)
{
	std::printf("[%d]=(\n", vADevIndex);
	const ::PaDeviceInfo *pADevInfo = ::Pa_GetDeviceInfo(vADevIndex);
	fPrintADevInfo(pADevInfo);
	std::printf(")=[%d]\n", vADevIndex);
	return cECodeNone;
} // fPrintADevInfo
int main(int argc, const char **argv)
{
	// datadef
	tECode	  vECode = cECodeNone;
	tTestData vTData = {
		.vPhaseL = cSampleMid,
		.vPhaseR = cSampleMid,
	};
	// // streams
	::PaStream *pStream = cNull;
	// // devices
	int					  vADevCount = 0;
	int					  vIDevIndex = 0;
	int					  vODevIndex = 0;
	const ::PaDeviceInfo *pIDevInfo	 = cNull;
	const ::PaDeviceInfo *pODevInfo	 = cNull;
	// // user input
	char vUserKey = '\0';
	int	 vUserNum = 0;
	// actions
	// // init
	vECode = ::Pa_Initialize();
	dProcECode(vECode);
	// // // devices
	vADevCount = ::Pa_GetDeviceCount();
	if(vADevCount < 0) { dProcECode(vADevCount); }
	std::printf("[ADevCount]=(%d)=[ADevCount]\n", vADevCount);
	std::printf("[ADevInfoList]=(\n");
	for(auto vADevIndex = 0; vADevIndex < vADevCount; vADevIndex++)
	{
		fPrintADevInfo(vADevIndex);
	} // process device info
	std::printf(")=[ADevInfoList]\n");
	// // // // idev
	std::printf("\n[IDevIndex]=(");
	std::cin >> vIDevIndex;
	std::printf(")=[IDevIndex]\n");
	if(vIDevIndex >= 0 && vIDevIndex < vADevCount)
	{
		fPrintADevInfo(vIDevIndex);
		pIDevInfo = ::Pa_GetDeviceInfo(vIDevIndex);
	}
	else
	{
		std::printf("invalid IDevIndex=%d\n", vIDevIndex);
		return cECodeADevIndex;
	}
	// // // // odev
	std::printf("\n[ODevIndex]=(");
	std::cin >> vODevIndex;
	std::printf(")=[ODevIndex]\n");
	if(vODevIndex >= 0 && vODevIndex < vADevCount)
	{
		fPrintADevInfo(vODevIndex);
		pODevInfo = ::Pa_GetDeviceInfo(vODevIndex);
	}
	else
	{
		std::printf("invalid ODevIndex=%d\n", vODevIndex);
		return cECodeADevIndex;
	}
	// // // streams
	vECode = ::Pa_OpenDefaultStream(
		&pStream,
		cIChanCount,
		cOChanCount,
		cSampleEnum,
		cSampleRate,
		cBufferFrameCount,
		&fIModifyCallback,
		&vTData
	);
	dProcECode(vECode);
	// // work
	vECode = ::Pa_StartStream(pStream);
	dProcECode(vECode);
	while(vUserKey != 'x') { std::cin >> vUserKey; }
	vECode = ::Pa_StopStream(pStream);
	dProcECode(vECode);
	// // quit
	vECode = ::Pa_CloseStream(pStream);
	dProcECode(vECode);
	vECode = ::Pa_Terminate();
	dProcECode(vECode);
	// final
	return vECode;
}
