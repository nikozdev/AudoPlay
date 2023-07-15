// headers
// // standard
#include <cstdio>
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
constexpr auto cECodeNone = ::paNoError;
// // buffers
constexpr auto cBufferFrameCount = 0xff; // paFramesPerBufferUnspecified
// // channels
constexpr unsigned cIChanCount = 0;
constexpr unsigned cOChanCount = 2;
// // samples
constexpr unsigned		   cSampleRate = 44'100;
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
/* this routine will be called by the audio engine when audio is needed
 * it may called at interrupt level on some machines
 * > so don't do anything that could mess up the system
 * > > like calling malloc() or free()
 */
static int fTestCallback(
	const void						 *pBufferI,
	void							 *pBufferO,
	unsigned long					  vBufferFrameCount,
	const ::PaStreamCallbackTimeInfo *pTimeInfo,
	::PaStreamCallbackFlags			  vStatusFlags,
	void							 *pUserData
)
{
	// unused
	(void)pBufferI;
	// get the stream-passed data
	tTestData *pData = (tTestData *)pUserData;
	tSample	  *pIter = (tSample *)pBufferO;
	// write to the buffer
	for(unsigned vIter = 0; vIter < vBufferFrameCount; vIter++)
	{
		*(pIter++) = pData->vPhaseL;
		*(pIter++) = pData->vPhaseR;
		// simple sawtooth phaser that ranges between min and max
		pData->vPhaseL += cSampleNegStep;
		if(pData->vPhaseL <= cSampleMin) { pData->vPhaseL -= cSampleMin; }
		pData->vPhaseR += cSamplePosStep;
		if(pData->vPhaseR >= cSampleMax) { pData->vPhaseR -= cSampleMax; }
	} // write to the buffer
	return cECodeNone;
}
int main(int argc, const char **argv)
{
	// datadef
	tECode	  vECode = cECodeNone;
	tTestData vTData = {
		.vPhaseL = cSampleMid,
		.vPhaseR = cSampleMid,
	};
	::PaStream *pStream = cNull;
	// actions
	// // init
	vECode = ::Pa_Initialize();
	dProcECode(vECode);
	vECode = ::Pa_OpenDefaultStream(
		&pStream,
		cIChanCount,
		cOChanCount,
		cSampleEnum,
		cSampleRate,
		cBufferFrameCount,
		&fTestCallback,
		&vTData
	);
	dProcECode(vECode);
	// // work
	vECode = ::Pa_StartStream(pStream);
	dProcECode(vECode);
	::Pa_Sleep(3'000);
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
