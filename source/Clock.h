
#ifndef		__CLOCK_H__
#define	__CLOCK_H__

//****************************************************************************************
//
//	Clock�N���X
//
//****************************************************************************************
class Clock
{
private:
	struct CLOCK_OBJ	//	�Ƃ肠�������v�̒u����
	{
		POINT		pos;			//	�ʒu
		POINT		start;			//	���̈ʒu
		POINT		end;			//	�ړ���̈ʒu
		POINT		shift;			//	�J�����p���ꕝ
		float		speed;			//	���x

		float		minuteAngle;	//	���j�̌���
		float		hourAngle;		//	���j�̌���
	};

private:
	//	�p�����[�^
	static	const	int		CLOCK_MAX = 4;
	CLOCK_OBJ	obj[4];		//	���v�\����
	POINT		length;		//	����
	float		org_speed;	//	���̃X�s�[�h
	float		cul_speed;	//	���ݑI�𒆂̂��������̃X�s�[�h
	int			selectNum;	//	�I�𒆂̔ԍ�
	bool		clear;		//	�N���A
	float		t;			//	����
	int			step;		//	�X�e�b�v

private:
	//	�I�u�W�F�N�g
	iex2DObj*	m_Clock;	//	���v�摜

	//	�p�����[�^
	int			m_State;		//	�V�[���J��

public:
	//	�������E���
	Clock(void);
	~Clock(void);

	//�@�������E�ǂݍ���
	bool	Initialize(void);
	void	InitObj(void);		//	�I�u�W�F�N�g������
	void	SetObj(int n, int x, int y, float hAngle, float mAngle, float s);

	//�@�X�V
	void	Update(void);

	//	�`��
	void	Render(void);

	//�@MAIN_STATE_GAME����֐�
	void	Move(void);		//	����
	bool	CheckWithin(const POINT& p, const int& startX, const int& startY, const int size);	//	�͈͓����ǂ������ׂ�
	void	Lerp(POINT& out, const POINT p1, const POINT p2, float t);	//	POINT���
	void	Control(void);	//	����
	void	CalcPos(void);	//	�ʒu�֌W�v�Z

	//	���擾�E�ݒ�
	void	GetTitleInfo(float hAngle, float mAngle);
};

//****************************************************************************************
#endif // !__CLOCK_H__
