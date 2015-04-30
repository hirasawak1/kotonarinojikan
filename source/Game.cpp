
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

		//	�V�[���ݒ�
		m_State = MAIN_STATE_INIT;

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

		TitleInitialize();
		GameInitialize();

		//	�^�C�g����
		m_State = MAIN_STATE_GAME;
	}

	//	�^�C�g��������
	void	Game::TitleInitialize( void )
	{
		m_Title.obj.pos.x		=	640;
		m_Title.obj.pos.y		=	360;
		m_Title.obj.hourAngle	=	PI / 180.0f * 30.0f;
		m_Title.obj.minuteAngle =	0.0f;
		m_Title.obj.start		=	m_Title.obj.pos;
		m_Title.obj.end			=	m_Title.obj.pos;
		m_Title.step			=	0;
		m_Title.t				=	0.0f;
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
		switch ( m_State )
		{
		case MAIN_STATE_INIT:	//	break�������̂܂܉���
			MainInitialize();

		case MAIN_STATE_TITLE:	//	�^�C�g��
			TitleUpdate();
			break;

		case MAIN_STATE_GAME:	//	�Q�[���{��
			GameUpdate();
			break;
		}
	}

	//	�^�C�g���X�V
	void	Game::TitleUpdate( void )
	{		
		//	��ɉ�]
		m_Title.obj.minuteAngle +=	PI / 180 * 1.0f;
		m_Title.obj.hourAngle	+= ( PI / 180 ) / 12.0f;

		//	�Q�[���{�̂Ɠ���
		m_GameClock->GetTitleInfo( m_Title.obj.hourAngle, m_Title.obj.minuteAngle );

		switch ( m_Title.step )
		{
		case 0:		//	�I��
			if ( CheckWithin( Mouse::cursor, m_Title.obj.pos.x - 150, m_Title.obj.pos.y - 150, GameInfo::WNDSIZE ) )
			{
				if ( Mouse::Click() )
				{
					m_Title.step++;
				}
			}
			break;

		case 1:		//	�Y�[���A�b�v
			m_Camera.param += PI / 180 * CameraInfo::ZOOM_SPEED;
			m_Camera.scale = ( int )( CameraInfo::ZOOM_MAX * sinf( m_Camera.param ) );

			//	���ȏ�߂Â�����
			if ( m_Camera.scale >= CameraInfo::ZOOM_MAX )
			{
				m_Camera.param = PI / 2;
				m_Camera.scale = CameraInfo::ZOOM_MAX;
				m_State = MAIN_STATE_GAME;

				//	�؂�ւ��O�ɃY�����v�Z���Ă���
				m_GameClock->CalcPos();
			}
			break;
		}
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
		//	�w�i�`��
		//m_BG->Render( 0, 0, 1280, 720, 0, 0, 2048, 1024 );

		switch ( m_State )
		{
		case MAIN_STATE_INIT:
			break;

		case MAIN_STATE_TITLE:
			TitleRender();
			break;

		case MAIN_STATE_GAME:
			GameRender();
			break;
		}
	}

	//	�^�C�g���`��
	void	Game::TitleRender( void )
	{
		int	x, y, w, h, sx, sy, sw, sh;

		//	���v�`��
		x = m_Title.obj.pos.x - 150 - m_Camera.scale / 2;
		y = m_Title.obj.pos.y - 150 - m_Camera.scale / 2;
		w = h = 300 + m_Camera.scale;
		sx = sy = 0;
		sw = sh = 256;
		m_Clock->Render( x, y, w, h, sx, sy, sw, sh, RS_COPY, 0xFFFF7777 );

		//	���j�`��
		sx = 256;
		m_Clock->Render( x, y, w, h, sx, sy, sw, sh, m_Title.obj.pos, m_Title.obj.minuteAngle );

		//	���j�`��
		sx = 0;
		sy = 256;
		m_Clock->Render( x, y, w, h, sx, sy, sw, sh, m_Title.obj.pos, m_Title.obj.hourAngle );
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

