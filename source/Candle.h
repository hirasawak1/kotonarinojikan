
#ifndef __CANDLE_H__
#define	__CANDLE_H__

//****************************************************************************************
//
//	Candle�N���X
//
//****************************************************************************************
class Candle
{
private:
	struct CANDLE
	{
		POINT		pos;			//	�ʒu
		POINT		start;		//	���̈ʒu
		POINT		end;			//	�ړ���̈ʒu
		POINT		shift;		//	�J�����p���ꕝ
		float			speed;		//	���x
		float			height;		//	����
	};

private:
	//	�I�u�W�F�N�g���
	static	const	int	CANDLE_MAX = 4;
	CANDLE		obj[4];
	iex2DObj*		candle;
	iex2DObj*		fire;
	iex2DObj*		melt_candle;
	iex2DObj*		light;

	//	�p�����[�^
	POINT		length;			//	����
	float			org_speed;	//	���̃X�s�[�h
	float			cul_speed;	//	���ݑI�𒆂̂��������̃X�s�[�h
	int			selectNum;	//	�I�𒆂̔ԍ�
	bool			clear;			//	�N���A
	float			t;					//	����
	int			step;			//	�X�e�b�v

public:
	//	�������E���
	Candle( void );
	~Candle( void );
	void	Initialize( void );
	void	SetObj( int n, int x, int y, float height );

	//	�X�V
	void	Update( void );

	//	�`��
	void	Render( void );

	//	����֐�
	void	Move( void );
	bool	CheckWithin( const POINT& p, const int& startX, const int& startY, const int size );	//	�͈͓����ǂ������ׂ�
	void	Lerp( POINT& out, const POINT p1, const POINT p2, float t );	//	POINT���
	void	Control( void );	//	����
	void	CalcPos( void );	//	�ʒu�֌W�v�Z

	//	���擾�E�ݒ�
	void	SetTitleInfo( float height );
};

//****************************************************************************************
#endif // !__CANDLE_H__
