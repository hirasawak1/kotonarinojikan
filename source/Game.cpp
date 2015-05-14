
#include	"iextreme.h"
#include	"Control.h"
#include	"Clock.h"
#include	"Metronom.h"
#include	"Candle.h"

#include	"Game.h"

//*****************************************************************************************************
//
//	Game�N���X
//
//*****************************************************************************************************

CAMERA	m_Camera;

//----------------------------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Game::Game( void )
	{
		
	}

	//	�f�X�g���N�^
	Game::~Game( void )
	{
		delete	m_BG;		m_BG		=	NULL;
		delete	m_Clock;	m_Clock		=	NULL;
		delete	m_GameClock;	m_GameClock = NULL;
	}

	//	������
	bool	Game::Initialize( void )
	{
		//	�w�i�ݒ�
		m_BG = new iex2DObj( "DATA/BG/�a��.png" );

		//	���v�摜
		m_Clock = new iex2DObj( "DATA/timer.png" );

		m_GameClock = new Clock();
		m_Metronom = new Metronom();
		m_Candle = new Candle();

		return true;
	}

	//	�S�̏�����
	void	Game::MainInitialize( void )
	{
		//	�J����������
		m_Camera.c_pos.x	=	640;
		m_Camera.c_pos.y	=	360;
		m_Camera.length.x	=	0;
		m_Camera.length.y	=	0;
		m_Camera.scale		=	0;
		m_Camera.param		=	0.0f;

		GameInitialize();

	}

	//	�Q�[��������
	void	Game::GameInitialize( void )
	{
		m_GameClock->Initialize();
		m_Metronom->Initialize();
		m_Candle->Initialize();
	}

//----------------------------------------------------------------------------
//	�X�V
//----------------------------------------------------------------------------
	
	//	�X�V
	void	Game::Update( void )
	{
		GameUpdate();
	}

	//	�Q�[���{�ҍX�V
	void	Game::GameUpdate( void )
	{
		//m_GameClock->Update();
		m_Metronom->Update();
		//m_Candle->Update();
	}

//----------------------------------------------------------------------------
//	�`��
//----------------------------------------------------------------------------
	
	//	�`��
	void	Game::Render( void )
	{
		GameRender();
	}

	//	�Q�[���{�ҕ`��
	void	Game::GameRender( void )
	{
		//m_GameClock->Render();
		m_Metronom->Render();
		//m_Candle->Render();
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------
	
	//�@�͈͓��ɂ��邩���ׂ�
	bool	Game::CheckWithin( const POINT& p, const int& startX, const int& startY, const int size )
	{
		POINT	cursorPos = p;

		//	�J�[�\�����W�ݒ�
		GetCursorPos( &cursorPos );
		ScreenToClient( iexSystem::Window, &cursorPos );

		if ( ( startX < cursorPos.x ) && ( cursorPos.x < startX + size ) )
		{
			if ( ( startY < cursorPos.y ) && ( cursorPos.y < startY + size ) )
				return true;
		}
		return false;
	}

	//	POINT���
	void	Game::Lerp( POINT& out, const POINT p1, const POINT p2, float t )
	{
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3���֐���Ԓl�ɕϊ�

		out.x = ( long )( p1.x * ( 1.0f - rate ) + p2.x * rate );
		out.y = ( long )( p1.y * ( 1.0f - rate ) + p2.y * rate );
	}

