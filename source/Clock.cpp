
#include	"iextreme.h"
#include	"Control.h"
#include	"Game.h"

#include	"Clock.h"

//****************************************************************************************
//
//	Clock�N���X
//
//****************************************************************************************

//----------------------------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Clock::Clock( void ) : m_Clock( NULL )
	{

	}

	//	�f�X�g���N�^
	Clock::~Clock( void )
	{
		delete	m_Clock;	m_Clock = NULL;
	}

	//	������
	bool	Clock::Initialize( void )
	{
		//	���v�摜
		m_Clock			=		new iex2DObj( "DATA/timer.png" );

		//	�ϐ�������
		org_speed		=		PI / 180;
		selectNum		=		0;
		cul_speed		=		0.0f;
		clear					=		false;
		t						=		1.0f;
		step					=		0;

		//	�I�u�W�F�N�g��񏉊���
		InitObj();

		return true;
	}

	//	�I�u�W�F�N�g������
	void	Clock::InitObj( void )
	{
		//	���̃I�u�W�F�N�g
		SetObj( 0, 640, 360, 30.0f, 0.0f, 1.0f );

		//	���̃I�u�W�F�N�g
		SetObj( 1, 940, -40, 120.0f, 0.0f, 2.0f );
		SetObj( 2, 340, 60, 300.0f, 0.0f, 0.5f );
		SetObj( 3, 540, -340, 240.0f, 0.0f, 1.0f );
	}

	//	�I�u�W�F�N�g���ݒ�
	void	Clock::SetObj( int n, int x, int y, float hAngle, float mAngle, float s )
	{
		obj[n].pos.x				=		x;
		obj[n].pos.y				=		y;
		obj[n].hourAngle		=		PI / 180.0f * hAngle;
		obj[n].minuteAngle =		PI / 180.0f * mAngle;
		obj[n].speed			=		s;

		//	�����ʒu�ۑ�
		obj[n].start		=		obj[n].pos;
		obj[n].end		=		obj[n].pos;
		obj[n].shift.x	=		0;
		obj[n].shift.y	=		0;
	}

//----------------------------------------------------------------------------
//	�X�V
//----------------------------------------------------------------------------

	//	�X�V
	void	Clock::Update( void )
	{
		//	����
		Move();

		switch ( step )
		{
		case 0:		//	�Y�[���A�E�g
			m_Camera.param += PI / 180 * CameraInfo::ZOOM_SPEED;
			m_Camera.scale = ( int )( CameraInfo::ZOOM_MAX * sinf( m_Camera.param ) );
			CalcPos();	//	����v�Z
			if ( m_Camera.scale <= 0 )
			{
				m_Camera.param = 0.0f;
				m_Camera.scale = 0;
				step++;
			}
			break;

		case 1:		//	�Q�[���J�n
			Control();
			break;
		}

		//	�p�����[�^���Z
		t += 0.01f;
		if ( t >= 1.0f )	t = 1.0f;
	}

//----------------------------------------------------------------------------
//	�`��
//----------------------------------------------------------------------------

	//	�`��
	void	Clock::Render( void )
	{
		//	�ϐ�����
		DWORD	color[4] = { 0xFFFF7777, 0xFF77FF77, 0xFF7777FF, 0xFFFF77FF };
		int	x, y, w, h, sx, sy, sw, sh;
		POINT	p;

		for ( int i = 0; i < CLOCK_MAX; i++ )
		{
			//	���v�`��
			x = obj[i].pos.x - 150 - m_Camera.scale / 2 + obj[i].shift.x / 2;
			y = obj[i].pos.y - 150 - m_Camera.scale / 2 + obj[i].shift.y / 2;
			p.x = obj[i].pos.x + obj[i].shift.x;
			p.y = obj[i].pos.y + obj[i].shift.y;
			w = h = 300 + m_Camera.scale;
			sx = sy = 0;
			sw = sh = 256;
			m_Clock->Render(x, y, w, h, sx, sy, sw, sh, RS_COPY, color[i]);

			//	���j�`��
			sx = 256;
			m_Clock->Render(x, y, w, h, sx, sy, sw, sh, p, obj[i].minuteAngle);

			//	���j�`��
			sx = 0;
			sy = 256;
			m_Clock->Render( x, y, w, h, sx, sy, sw, sh, p, obj[i].hourAngle );

			//	�X�s�[�h�\��
			x = obj[i].pos.x + 100;
			y = obj[i].pos.y + 100;
		}
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//�@�͈͓��ɂ��邩���ׂ�
	bool	Clock::CheckWithin( const POINT& p, const int& startX, const int& startY, const int size )
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
	void	Clock::Lerp( POINT& out, const POINT p1, const POINT p2, float t )
	{
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3���֐���Ԓl�ɕϊ�

		out.x = ( long )( p1.x * ( 1.0f - rate ) + p2.x * rate );
		out.y = ( long )( p1.y * ( 1.0f - rate ) + p2.y * rate );
	}

	//	����
	void	Clock::Move( void )
	{
		//	���v�̐j�̉�]
		for ( int i = 0; i < CLOCK_MAX; i++ )
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

				//	���̃X�s�[�h�ŉ�]
				obj[i].minuteAngle += org_speed;
				obj[i].hourAngle += org_speed / 12.0f;
			}
			else
			{
				//	�ŏI�̈ʒu�����߂�
				obj[i].end.x = obj[i].start.x + length.x;
				obj[i].end.y = obj[i].start.y + length.y;

				//	�ړ�
				Lerp( obj[i].pos, obj[i].start, obj[i].end, t );

				//	��]
				obj[i].minuteAngle += org_speed * ( obj[i].speed / cul_speed );
				obj[i].hourAngle += ( org_speed / 12.0f ) * ( obj[i].speed / cul_speed );
			}
		}
	}

	//	�R���g���[��
	void	Clock::Control( void )
	{
		//	�G���^�[�őI��
		if ( KEY( KEY_ENTER ) == 3 )
		if ( obj[selectNum].speed == obj[0].speed && selectNum != 0 )
			clear = true;

		//	�}�E�X�N���b�N�`�F�b�N
		for ( int n = 0; n < 4; n++ )
		{
			//	�}�E�X�J�[�\�����͈͓���������
			if ( CheckWithin(Mouse::cursor, obj[n].pos.x - 150, obj[n].pos.y - 150, GameInfo::WNDSIZE ) )
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
						for ( int i = 0; i < CLOCK_MAX; i++ )
							obj[i].start = obj[i].pos;

						//	�p�����[�^�ݒ�
						t = 0.0f;
					}
				}
			}
		}
	}

	//	�ʒu�֌W�v�Z
	void	Clock::CalcPos( void )
	{
		float cx, cy, vx, vy;
		cx = 640;
		cy = 360;
		float	l;

		for ( int i = 0; i < CLOCK_MAX; i++ )
		{
			vx = obj[i].pos.x - cx;
			vy = obj[i].pos.y - cy;

			l = sqrt( vx * vx + vy *vy );

			vx /= l;
			vy /= l;

			if ( selectNum != i )
			{
				obj[i].shift.x = ( int )( vx * m_Camera.scale * 2 );
				obj[i].shift.y = ( int )( vy * m_Camera.scale * 2 );
			}
		}
	}

//----------------------------------------------------------------------------
//	���擾�E�ݒ�
//----------------------------------------------------------------------------

	//	�^�C�g�����擾
	void	Clock::GetTitleInfo( float hAngle, float mAngle )
	{
		obj[0].hourAngle		=	hAngle;
		obj[0].minuteAngle = mAngle;
	}