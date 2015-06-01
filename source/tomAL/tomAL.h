#ifndef _tomAL_h_
#define _tomAL_h_

#include "OpenAL\include\al.h"
#include "OpenAL\include\alc.h"

//#include"ogg\include\ogg\ogg.h"
//#include"ogg\include\vorbis\vorbisfile.h"

#include <math.h>
#include <memory>
#include <windows.h>

class tomALBase;

namespace STREAMING_MODE
{
	enum
	{
		NORMAL = 0,
		FADE_IN,
		FADE_OUT,
	};
}

namespace SOUND_EFFECT
{
	enum
	{
		NONE = 0,
		DISTORTION,
		DELAY,
	};
}

typedef struct DISTORTION
{
	float level; //1�`0(1���}�b�N�X�A�ׂꂫ���ĉ����������Ȃ��̂Œ���)
}DISTORTION;

class DELAY
{
private:
	ALuint* delaySource = nullptr;
	int timer = 0;//�J�E���^
	int playCount = 0;
	bool isPlay = false;
public:
	int delayTime = 0;//�f�B���C�����Ȃ�܂ł̎���(�P��:�t���[��)
	float attenuation = 0;//�����l(1�`0)
	int time = 0;//�f�B���C���̉�

	friend tomALBase;
};

class tomALBase
{
protected:
	ALuint buffer = 0;
	ALuint source = 0;
	float vol = 1.0f;
	float workVol = 0;
	unsigned int rate = 0;
	unsigned int dataSize = 0;

	int effect = 0;
	float distortionParam = 0;

	DELAY* delayParam = nullptr;
	

	struct PlayData
	{
		unsigned int rate = 0;
		unsigned int dataSize = 0;
		ALenum format = 0;
	}playData;

	void Distortion( char* data, int dataSize );

	void Delay( char* data, int dataSize );

	void DelayPlay()
	{
		delayParam->isPlay = true;
	}
public:
	tomALBase(){};
	virtual ~tomALBase()
	{
		alDeleteSources( 1, &source );
		alDeleteBuffers( 1, &buffer );
	}

	virtual bool Load( char* filename ) = 0;

	void setEffect( int type, void* param );

	virtual void Update();
};

class tomALInmemory : public tomALBase
{
private:
	bool LoadWav( char* filename );
	//bool LoadOgg( char* filename );

	bool Load( char* filename );

	char* data = nullptr;
public:
	tomALInmemory(){};
	~tomALInmemory();

	void Set( char* filename );

	void Play();
	void Pause();
	void Stop();

	bool IsPlay();
};

class tomALStreaming : public tomALBase
{
private:
	ALuint bgmbuffer[2];

	bool LoadWav( char* data );
	//bool LoadOgg( char* filename );

	bool Load( char* filename ){ return true; }

	HANDLE	hStrThread;
	unsigned int dwThreadId;
	unsigned int dwThrdParam;

	char* data;
	unsigned int inMemoryDataSize = 0;

	char dataName[64];

	unsigned int readDataSize = 0;
	unsigned int dataStartPoint = 0;

	bool loop = true;

	int mode = 0;
	float fadePalam = 0;
public:
	tomALStreaming( ){};
	~tomALStreaming( );

	void Set( char* filename, int mode = STREAMING_MODE::NORMAL, int param = 60 );

	void Play();
	void Pause();
	void Stop();

	void Stream();

	void Update();

	bool IsPlay( );

	void setMode( int m, int param = 60 );
};

class tomALManager
{
private:
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;

	tomALInmemory** inmemory = nullptr;
	tomALStreaming** streaming = nullptr;

	int inmemoryNum = 0;
	int streamingNum = 0;
public:
	tomALManager(){};
	~tomALManager();

	void Init( int inmemorySound = 32, int streamingSound = 5 );

	void CreateStreamingPlayer( int No );

	void SetInmemory( int No, char* filename );
	void SetStreaming( int No, char* filename, int mode = STREAMING_MODE::NORMAL, int param = 60 );

	void PlayInmemory( int No );
	void PauseInmemory( int No );
	void StopInmemory( int No );
	bool IsPlayInmemory( int No );
	void SetEffectInmemory( int No, int type, void* param );

	void PlayStreaming( int No );
	void PauseStreaming( int No );
	void StopStreaming( int No );
	bool IsPlayStreaming( int No );
	void SetModeStreaming( int No, int m, int param = 60 );
	void SetEffectStreaming( int No, int type, void* param );

	void Update();
};

#endif