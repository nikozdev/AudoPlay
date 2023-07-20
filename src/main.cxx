// headers
// // standard
#include <cmath>
#include <random>
#include <cstdio>
#include <iostream>
#include <limits>
// // 3rd-party
#include "../lib/portaudio/include/portaudio.h"
#include "../lib/lTermArgs/src/lTermArgs.hxx"
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
// // errors
using tECode = ::PaError;
// // samples
using tSample		= char;
using tSampleLimits = ::std::numeric_limits<tSample>;
// // random
using tRandomEng = std::mt19937; // slow but powerful mersenne_twister
using tRandomDev = std::random_device;
using tRandomDis = std::uniform_int_distribution<int>;
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
typedef struct tWaveSawData
{
	tSample vPhaseL;
	tSample vPhaseR;
} tWaveSawData;
// actions
int fRandomCallback(
	const void						 *pBufferI,
	void							 *pBufferO,
	unsigned long					  vBufferFrameCount,
	const ::PaStreamCallbackTimeInfo *pTimeInfo,
	::PaStreamCallbackFlags			  vStatusFlags,
	void							 *pUserData
);
int fWaveSinCallback(
	const void						 *pBufferI,
	void							 *pBufferO,
	unsigned long					  vBufferFrameCount,
	const ::PaStreamCallbackTimeInfo *pTimeInfo,
	::PaStreamCallbackFlags			  vStatusFlags,
	void							 *pUserData
);
int fWaveSawCallback(
	const void						 *pBufferI,
	void							 *pBufferO,
	unsigned long					  vBufferFrameCount,
	const ::PaStreamCallbackTimeInfo *pTimeInfo,
	::PaStreamCallbackFlags			  vStatusFlags,
	void							 *pUserData
);
int fEchoCallback(
	const void						 *pBufferI,
	void							 *pBufferO,
	unsigned long					  vBufferFrameCount,
	const ::PaStreamCallbackTimeInfo *pTimeInfo,
	::PaStreamCallbackFlags			  vStatusFlags,
	void							 *pUserData
);
int fIModifyCallback(
	const void						 *pBufferI,
	void							 *pBufferO,
	unsigned long					  vBufferFrameCount,
	const ::PaStreamCallbackTimeInfo *pTimeInfo,
	::PaStreamCallbackFlags			  vStatusFlags,
	void							 *pUserData
);
int fPrintADevInfo(const ::PaDeviceInfo *pADevInfo);
int fPrintADevInfo(int vADevIndex);
int main(int argc, char **argv)
{
	::nTermArgs::tCmd vTermArgs;
	vTermArgs.fSetHelpText(
		R"(aSoundAgent - a command line digital audio signal processor)"
	);
	vTermArgs.fParse(argc, argv);
	// datadef
	tECode		 vECode		  = cECodeNone;
	tWaveSawData vWaveSawData = {
		.vPhaseL = cSampleMid,
		.vPhaseR = cSampleMid,
	};
	tRandomDev vRandomDev;
	tRandomEng vRandomEng{ vRandomDev() };
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
#if 0
#elif 0
		&fWaveSinCallback,
		cNull
#elif 0
		&fWaveSawCallback,
		&vWaveSawData
#elif 0
		&fEchoCallback,
		cNull
#elif 0
		&fIModifyCallback,
		cNull
#else
		&fRandomCallback,
		&vRandomEng
#endif
	);
	dProcECode(vECode);
	// // work
	vECode = ::Pa_StartStream(pStream);
	dProcECode(vECode);
	while(vUserKey != 'q') { std::cin >> vUserKey; }
	vECode = ::Pa_StopStream(pStream);
	dProcECode(vECode);
	// // quit
	vECode = ::Pa_CloseStream(pStream);
	dProcECode(vECode);
	vECode = ::Pa_Terminate();
	dProcECode(vECode);
	// final
	return vECode;
} // main
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
int fRandomCallback(
	const void						 *pBufferI,
	void							 *pBufferO,
	unsigned long					  vBufferFrameCount,
	const ::PaStreamCallbackTimeInfo *pTimeInfo,
	::PaStreamCallbackFlags			  vStatusFlags,
	void							 *pUserData
)
{
	tSample	   *pIterO	   = (tSample *)pBufferO;
	tSample	   *pIterI	   = (tSample *)pBufferI;
	tRandomEng *pRandomEng = (tRandomEng *)pUserData;
	tRandomDis	vRandomDis{ cSampleMin, cSampleMax };
	// write to the buffer
	for(unsigned vIter = 0; vIter < vBufferFrameCount; vIter++)
	{
		*(pIterO++) = vRandomDis(*pRandomEng);
		*(pIterO++) = vRandomDis(*pRandomEng);
	} // write to the buffer
	return cECodeNone;
} // fRandomCallback
int fWaveSinCallback(
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
		auto vTime	= (float)pTimeInfo->currentTime;
		auto vRad	= vTime * M_PI / 180.0;
		*(pIterO++) = (tSample)(std::sin(vRad) * cSampleMax);
		*(pIterO++) = (tSample)(std::sin(vRad) * cSampleMax);
	} // write to the buffer
	return cECodeNone;
} // fWaveSinCallback
int fWaveSawCallback(
	const void						 *pBufferI,
	void							 *pBufferO,
	unsigned long					  vBufferFrameCount,
	const ::PaStreamCallbackTimeInfo *pTimeInfo,
	::PaStreamCallbackFlags			  vStatusFlags,
	void							 *pUserData
)
{
	tWaveSawData *pData	 = (tWaveSawData *)pUserData;
	tSample		 *pIterO = (tSample *)pBufferO;
	tSample		 *pIterI = (tSample *)pBufferI;
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
int fEchoCallback(
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
int fIModifyCallback(
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
} // fModifyCallback
