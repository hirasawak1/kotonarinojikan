
#ifndef	__CLOCK_H__
#define	__CLOCK_H__

#include "TimeObject.h"
#include "Campus.h"

//****************************************************************************************
//
//	Clock�N���X
//
//****************************************************************************************
//class Clock :public TimeObject
//{
//private:
//	//static	const	int		CLOCK_MAX = 4;
//	//Clock*			obj[CLOCK_MAX];
//
//	float		minuteAngle;	//	���j�̌���
//	float		hourAngle;		//	���j�̌���
//
//
//public:
//	//	�������E���
//	Clock(void);
//	~Clock(void);
//
//	//�@�������E�ǂݍ���
//	bool	Initialize(void);
//	void	InitObj(void);		//	�I�u�W�F�N�g������
//	void	SetObj(int n, int x, int y, float hAngle, float mAngle, float s);
//
//	//�@�X�V
//	void	Update(void);
//
//	//	�`��
//	void	Render(void);
//
//	void	Move(void);		//	����
//
//	//-------------------------------------------------------
//	//----------------- develop:�x�c ------------------------
//	//-------------------------------------------------------
//
//	///<summary>����p�Ƃ̓����蔻��</summary>
//	///<param name="p">�I�u�W�F�N�g�̃X�N���[�����W</param>
//	bool CheckWithin(const POINT& p)
//	{
//		return CheckWithin(Campus::Inst()->TransCampusPos(p), obj[0]->pos.x - 150, obj[0]->pos.y - 150, 300);
//	}
//
//	///<summary>���v�̍��W�擾</summary>
//	///<return>���v�̒��S���W</return>
//	POINT& GetPosCC()
//	{
//		return obj[0]->pos;
//	}
//
//	//-------------------------------------------------------
//	//----------------�����܂Ł@develop:�x�c-----------------
//	//-------------------------------------------------------
//
//};

class Clock :public TimeObj
{
	//----------- field --------------
public:
	enum Behavior
	{
		STOP=-2, CHECK,
		SMOOTH=0, STEPING
	};
	enum Image
	{
		BACK, HOUR, MINUTE
	};
private:
	static const int FPS = 60;			// frame per second
	static const int MPH = 60;			// minute per hour
	static const int HOUR_CYCLE = 12;	// ����̎��Ԑ�
	static const float ANGLE_1MINUTE;	// �ꕪ�̊p�x
	static const float ANGLE_1HOUR;		// �ꎞ�Ԃ̊p�x
	int timeCount = 0;					// �b�J�E���g
	int frameCount = 0;					// �t���[���J�E���g
	float hourAngle = 0;				// �Z�j�̊p�x
	float minuteAngle = 0;				// ���j�̊p�x

	//---------- method -------------
public:
	Clock();
	void Init(const Vector2& centerPos, int colW, int colH, float scale, float speed, Behavior behavior);
	void Update()override;
	void Render()override;
private:
	void Update_Time();
	void Update_Check();
	void Update_Smooth();
	void Update_Steping();
};

//****************************************************************************************
#endif // !__CLOCK_H__
