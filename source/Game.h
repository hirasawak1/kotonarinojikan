
#ifndef __GAME_H__
#define __GAME_H__

//*****************************************************************************************************
//
//	Game�N���X
//
//*****************************************************************************************************

//----------------------------------------------------------------------------------
//	�N���X�O���錾
//----------------------------------------------------------------------------------
	class		Clock;
	class		Metronom;
	class		Candle;

//----------------------------------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------------------------------
	namespace GameInfo
	{
		//	�摜��
		const	int		WNDSIZE = 300;
	}
	namespace CameraInfo
	{
		const	int		ZOOM_MAX = 850;
		const	float		ZOOM_SPEED = 2.0f;
	}

//----------------------------------------------------------------------------------
//	�\����
//----------------------------------------------------------------------------------
	struct CAMERA
	{
		POINT	c_pos;
		POINT	length;
		float		param;
		int		scale;
	};
	struct CLOCK_OBJ	//	�Ƃ肠�������v�̒u����
	{
		POINT		pos;			//	�ʒu
		POINT		start;			//	���̈ʒu
		POINT		end;			//	�ړ���̈ʒu
		POINT		shift;			//	�J�����p���ꕝ
		float		minuteAngle;	//	���j�̌���
		float		hourAngle;		//	���j�̌���
		float		speed;			//	���x
	};
	struct METRONOM
	{
		POINT		pos;			//	�ʒu
		POINT		start;		//	���̈ʒu
		POINT		end;			//	�ړ���̈ʒu
		POINT		shift;		//	�J�����p���ꕝ
		float			speed;		//	���x
		float			height;		//	����
		float			angle;		//	����
	};
	struct CANDLE
	{
		POINT		pos;			//	�ʒu
		POINT		start;		//	���̈ʒu
		POINT		end;			//	�ړ���̈ʒu
		POINT		shift;		//	�J�����p���ꕝ
		float			speed;		//	���x
		float			height;		//	����
	};

//----------------------------------------------------------------------------------
//	extern�錾
//----------------------------------------------------------------------------------
	extern	CAMERA	m_Camera;

//----------------------------------------------------------------------------------
//	Game�N���X
//----------------------------------------------------------------------------------
class Game
{
public:

private:

private:
	//	�I�u�W�F�N�g
	iex2DObj*	m_BG;		//	�w�i
	iex2DObj*	m_Clock;	//	���v�摜
	
	//	�p�����[�^
	Clock*				m_GameClock;		//	���v�I�����̓���
	Metronom*		m_Metronom;			//	���g���m�[���I�����̓���
	Candle*			m_Candle;				//	�낤�����I�����̓���
	
	int			m_State;		//	�V�[���J��

public:
	//	�������E���
	Game( void );
	~Game( void );
	
	//�@�������E�ǂݍ���
	bool	Initialize( void );
	void	MainInitialize( void );
	void	GameInitialize( void );
	
	//�@�X�V
	void	Update( void );
	void	GameUpdate( void );

	//		�`��
	void	Render( void );
	void	GameRender( void );

	//�@MAIN_STATE_GAME����֐�
	bool	CheckWithin( const POINT& p, const int& startX, const int& startY, const int size );	//	�͈͓����ǂ������ׂ�
	void	Lerp( POINT& out, const POINT p1, const POINT p2, float t );	//	POINT���
};

//*****************************************************************************************************
#endif