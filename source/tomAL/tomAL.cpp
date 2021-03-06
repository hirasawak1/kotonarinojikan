#include "tomAL.h"

void tomALBase::setEffect( int type, void* param )
{
	switch( type )
	{
	case SOUND_EFFECT::DISTORTION:
	{
									 effect = type;
									 DISTORTION* distortion = (DISTORTION*)param;
									 distortionParam = 1 - distortion->level;
									 break;
	}
	case SOUND_EFFECT::DELAY:
		effect = type;
		delayParam = (DELAY*)param;
		break;
	}
}

void tomALBase::Distortion( char* data, int dataSize )
{
	for( int i = 0; i < dataSize; i++ )
	{
		switch( playData.format )
		{
		case AL_FORMAT_MONO8:
		case AL_FORMAT_STEREO8:
			data[i] -= 128;
			if( data[i] < (int)(-distortionParam * 128) )
			{
				data[i] = (int)(-distortionParam * 128);
			}
			if( data[i] > (int)(distortionParam * 128) )
			{
				data[i] = (int)(distortionParam * 128);
			}
			break;
		case AL_FORMAT_MONO16:
		case AL_FORMAT_STEREO16:
			short work;
			memcpy( &work, &data[i], 2 );
			if( work < (int)(-distortionParam * 32767) )
			{
				work = (int)(-distortionParam * 32767);
			}
			if( work >( int )(distortionParam * 32767) )
			{
				work = (int)(distortionParam * 32767);
			}
			memcpy( &data[i], &work, 2 );
			i++;
			break;
		}

	}
}

void tomALBase::Delay( char* data, int dataSize )
{
	if( delayParam->delaySource != nullptr )
	{
		for( int i = 0; i < delayParam->time; i++ )
		{
			alDeleteSources( 1, &delayParam->delaySource[i] );
		}
		delete[] delayParam->delaySource;
	}
	delayParam->delaySource = new ALuint[delayParam->time];

	float workAttenuation = delayParam->attenuation;
	for( int i = 0; i < delayParam->time; i++ )
	{
		ALuint workBuffer;
		alGenBuffers( 1, &workBuffer );
		alBufferData( workBuffer, playData.format, data, dataSize, playData.rate );

		alGenSources( 1, &delayParam->delaySource[i] );
		alSourcef( delayParam->delaySource[i], AL_GAIN, vol * workAttenuation );
		alSourcei( delayParam->delaySource[i], AL_BUFFER, workBuffer );

		alDeleteBuffers( 1, &workBuffer );

		workAttenuation *= delayParam->attenuation;
	}
	delayParam->timer = 0;
	delayParam->playCount = 0;

	delayParam->isPlay = true;
}

void tomALBase::Update()
{
	switch( effect )
	{
	case SOUND_EFFECT::DELAY:
		if( delayParam->isPlay == false )
		{
			break;
		}
		delayParam->timer++;
		if( delayParam->delayTime <= delayParam->timer )
		{
			alSourcePlay( delayParam->delaySource[delayParam->playCount] );
			delayParam->playCount++;
			delayParam->timer = 0; 
		}
		if( delayParam->playCount >= delayParam->time )
		{
			delayParam->playCount = 0;
			delayParam->isPlay = false;
		}
		break;
	}
}

//=====================================================================================================================
//
//=====================================================================================================================

void tomALManager::Init( int inmemorySound, int streamingSound )
{
	device = alcOpenDevice( NULL );

	context = alcCreateContext( device, NULL );
	alcMakeContextCurrent( context );

	inmemoryNum = inmemorySound;
	inmemory = new tomALInmemory*[inmemoryNum];
	for( int i = 0; i < inmemoryNum; i++ )
	{
		inmemory[i] = nullptr;
	}

	streamingNum = streamingSound;
	streaming = new tomALStreaming*[streamingNum];
	for( int i = 0; i < streamingNum; i++ )
	{
		streaming[i] = nullptr;
	}
}

tomALManager::~tomALManager()
{
	alcDestroyContext( context );
	alcCaptureCloseDevice( device );

	for( int i = 0; i < inmemoryNum; i++ )
	{
		if( inmemory[i] != nullptr )
		{
			delete inmemory[i];
			inmemory[i] = nullptr;
		}
	}
	if( inmemory != nullptr )
	{
		delete inmemory;
		inmemory = nullptr;
	}

	for( int i = 0; i < streamingNum; i++ )
	{
		if( streaming[i] != nullptr )
		{
			delete streaming[i];
			streaming[i] = nullptr;
		}
	}
	if( streaming != nullptr )
	{
		delete streaming;
		streaming = nullptr;
	}
}

void tomALManager::SetInmemory( int No, char* filename )
{
	inmemory[No] = new tomALInmemory();
	inmemory[No]->Set( filename );
}

void tomALManager::CreateStreamingPlayer( int No )
{
	streaming[No] = new tomALStreaming( );
}

void tomALManager::SetStreaming( int No, char* filename, int mode, int param )
{
	streaming[No]->Set( filename, mode, param );
}

void tomALManager::PlayInmemory( int No )
{
	inmemory[No]->Play();
}

void tomALManager::PauseInmemory( int No )
{
	inmemory[No]->Pause();
}

void tomALManager::StopInmemory( int No )
{
	inmemory[No]->Stop( );
}

bool tomALManager::IsPlayInmemory( int No )
{
	return inmemory[No]->IsPlay( );
}

void tomALManager::SetEffectInmemory( int No, int type, void* param )
{
	inmemory[No]->setEffect( type, param );
}

void tomALManager::PlayStreaming( int No )
{
	streaming[No]->Play();
}

void tomALManager::PauseStreaming( int No )
{
	streaming[No]->Pause();
}

void tomALManager::StopStreaming( int No )
{
	streaming[No]->Stop();
}

bool tomALManager::IsPlayStreaming( int No )
{
	return streaming[No]->IsPlay( );
}

void tomALManager::SetModeStreaming( int No, int m, int param )
{
	streaming[No]->setMode( m, param );
}

void tomALManager::SetEffectStreaming( int No, int type, void* param )
{
	streaming[No]->setEffect( type, param );
}

void tomALManager::Update()
{
	for( int i = 0; i < inmemoryNum; i++ )
	{
		if( inmemory[i] != nullptr )
		{
			inmemory[i]->Update( );
		}
	}
	for( int i = 0; i < streamingNum; i++ )
	{
		if( streaming[i] != nullptr )
		{
			streaming[i]->Update( );
		}
	}
}