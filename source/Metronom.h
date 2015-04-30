
#ifndef		__METRONOM_H__
#define	__METRONOM_H__

//****************************************************************************************
//
//		Metronom�N���X
//
//****************************************************************************************
class Metronom
{
private:
	struct METRONOM
	{
		POINT		pos;			//	�ʒu
		POINT		start;		//	���̈ʒu
		POINT		end;			//	�ړ���̈ʒu
		POINT		shift;		//	�J�����p���ꕝ
		float			speed;		//	���x
		float			param;		//	��]�p
		float			height;		//	����
		float			angle;		//	����
	};

private:
	//	�I�u�W�F�N�g���
	static	const	int	METRONOM_MAX = 4;
	METRONOM		obj[4];
	iex2DObj*		back;
	iex2DObj*		metronom;
	iex2DObj*		needle;
	
	//	�p�����[�^
	POINT		length;			//	����
	float			org_speed;	//	���̃X�s�[�h
	float			cul_speed;	//	���ݑI�𒆂̂��������̃X�s�[�h
	int			selectNum;	//	�I�𒆂̔ԍ�
	bool			clear;			//	�N���A
	float			t;					//	����
	int			step;			//	�X�e�b�v
	float			plus_speed;	//	�X�s�[�h���Z�l

public:
	//	�������E���
	Metronom( void );
	~Metronom( void );
	void	Initialize( void );
	void	SetObj( int n, int x, int y, float height, float speed );

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
	void	SetTitleInfo( float angle );
};

//****************************************************************************************
#endif // !__METRONOM_H__
