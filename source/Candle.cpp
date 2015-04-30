
#include	"iextreme.h"
#include	"Game.h"
#include	"Control.h"

#include	"Candle.h"

//****************************************************************************************
//
//	Candle�N���X
//
//****************************************************************************************

//--------------------------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Candle::Candle( void )
	{
	
	}

	//	�f�X�g���N�^
	Candle::~Candle( void )
	{
		delete	candle;			candle = NULL;
		delete	fire;					fire = NULL;
		delete	melt_candle;	melt_candle = NULL;
		delete	light;				light = NULL;
	}

	//	������
	void	Candle::Initialize( void )
	{
		candle = new iex2DObj( "DATA/�낤����.png" );
		fire = new iex2DObj( "DATA/�낤����(��).png" );
		melt_candle = new iex2DObj( "DATA/�낤����(�n��).png" );
		light = new iex2DObj( "DATA/�낤�����i������j.png" );

		//	�ϐ�������
		org_speed = PI / 180;
		selectNum = 0;
		cul_speed = 0.0f;
		clear = false;
		t = 0.0f;
		step = 0;

		//	���̃I�u�W�F�N�g
		SetObj( 0, 640, 360, 1.0f );

		//	���̃I�u�W�F�N�g
		SetObj( 1, 940, -40, 0.1f );
		SetObj( 2, 340, 60, 0.5f );
		SetObj( 3, 540, -340, 1.0f );
	}

	//	�I�u�W�F�N�g�ݒ�
	void	Candle::SetObj( int n, int x, int y, float height )
	{
		obj[n].pos.x = x;
		obj[n].pos.y = y;
		obj[n].height = height;
		obj[n].shift.x = 0;
		obj[n].shift.y = 0;
		obj[n].start = obj[n].pos;
		obj[n].end = obj[n].pos;
	}

//--------------------------------------------------------------------------------------------
//	�X�V�E�`��
//--------------------------------------------------------------------------------------------

	//	�X�V
	void	Candle::Update( void )
	{
		//	����
		Move();

		//	����
		Control();

		//	�p�����[�^���Z
		t += 0.01f;
		if ( t >= 1.0f )		t = 1.0f;
	}

	//	�`��
	void	Candle::Render( void )
	{
		int	x, y, w, h, sx, sy, sw, sh;

		for ( int i = 0; i < CANDLE_MAX; i++ )
		{
			x = obj[i].pos.x - GameInfo::WNDSIZE / 2 + obj[i].shift.x / 2;
			y = obj[i].pos.y - GameInfo::WNDSIZE / 2 + obj[i].shift.y / 2;
			w = GameInfo::WNDSIZE + obj[i].shift.x;
			h = GameInfo::WNDSIZE + obj[i].shift.y;
			sx = sy = 0;
			sw = sh = 512;
			candle->Render( x, y, w, h, sx, sy, sw, sh );
			light->Render( x, y - 120, w, h, sx, sy, sw, sh );
			fire->Render( x, y - 120, w, h, sx, sy, sw, sh );
			melt_candle->Render( x, y, w, h, sx, sy, sw, sh );
			
		}
	}

//--------------------------------------------------------------------------------------------
//	����֐�
//--------------------------------------------------------------------------------------------

	//	����
	void	Candle::Move( void )
	{
		//	���v�̐j�̉�]
		for ( int i = 0; i < CANDLE_MAX; i++ )
		{
			//	�I�𒆂̃I�u�W�F�N�g
			if ( i == selectNum )
			{
				//	���S����̋��������߂�
				length.x = 640 - obj[i].start.x;
				length.y = 360 - obj[i].start.y;

				//	�ŏI�̈ʒu�����߂�
				obj[i].end.x = obj[i].start.x + length.x;
				obj[i].end.y = obj[i].start.y + length.y;

				//	���S�Ɉړ�( �ړ���� )
				Lerp( obj[i].pos, obj[i].start, obj[i].end, t );

				//	�I�𒆂̃I�u�W�F�N�g�̃X�s�[�h��ۑ�
				cul_speed = obj[i].speed;
			}
			else
			{
				//	�ŏI�̈ʒu�����߂�
				obj[i].end.x = obj[i].start.x + length.x;
				obj[i].end.y = obj[i].start.y + length.y;

				//	�ړ�
				Lerp( obj[i].pos, obj[i].start, obj[i].end, t );
			}
		}
	}

	//�@�͈͓��ɂ��邩���ׂ�
	bool	Candle::CheckWithin( const POINT& p, const int& startX, const int& startY, const int size )
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
	void	Candle::Lerp( POINT& out, const POINT p1, const POINT p2, float t )
	{
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3���֐���Ԓl�ɕϊ�

		out.x = ( long )( p1.x * ( 1.0f - rate ) + p2.x * rate );
		out.y = ( long )( p1.y * ( 1.0f - rate ) + p2.y * rate );
	}

	//	�R���g���[��
	void	Candle::Control( void )
	{
		//	�G���^�[�őI��
		if ( KEY( KEY_ENTER ) == 3 )
		if ( obj[selectNum].speed == obj[0].speed && selectNum != 0 )
			clear = true;

		//	�}�E�X�N���b�N�`�F�b�N
		for ( int n = 0; n < CANDLE_MAX; n++ )
		{
			//	�}�E�X�J�[�\�����͈͓���������
			if ( CheckWithin( Mouse::cursor, obj[n].pos.x - 150, obj[n].pos.y - 150, GameInfo::WNDSIZE ) )
			{
				//	�N���b�N����
				if ( Mouse::Click() )
				{
					//	�ړ����I�����Ă��邩�`�F�b�N
					if ( t < 1.0f )		return;

					//	�I�𒆂̃I�u�W�F�N�g����Ȃ�������
					if ( selectNum != n )
					{
						//	�I�������I�u�W�F�N�g�̔ԍ�
						selectNum = n;

						//	���̈ʒu�ۑ�
						for ( int i = 0; i < CANDLE_MAX; i++ )
							obj[i].start = obj[i].pos;

						//	�p�����[�^�ݒ�
						t = 0.0f;
					}
				}
			}
		}
	}

	//	�ʒu�֌W�v�Z
	void	Candle::CalcPos( void )
	{
		float cx, cy, vx, vy;
		cx = 640;
		cy = 360;
		float	l;

		for ( int i = 0; i < CANDLE_MAX; i++ )
		{
			vx = obj[i].pos.x - cx;
			vy = obj[i].pos.y - cy;

			l = sqrt( vx * vx + vy * vy );

			vx /= l;
			vy /= l;

			if ( selectNum != i )
			{
				obj[i].shift.x = ( int )( vx * m_Camera.scale * 2 );
				obj[i].shift.y = ( int )( vy * m_Camera.scale * 2 );
			}
		}
	}

	//-----------------------------------------------------------------------------------------------
//	���擾�E�ݒ�
//-----------------------------------------------------------------------------------------------

	//	�^�C�g��������擾
	void	Candle::SetTitleInfo( float height )
	{
		obj[0].height = height;
	}