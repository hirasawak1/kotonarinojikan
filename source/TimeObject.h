#ifndef _TIMEOBJECT_H_
#define _TIMEOBJECT_H_

//#include "Game.h"
#include "../IEX/iextreme.h"
#include <map>

// ���v�E�X�C�E���g���m�[���̊��N���X
//class TimeObject
//{
//protected:
//	iex2DObj*	image = nullptr;		//�@�摜
//	const int	SOURCE_W = 0;			//�@�摜�\�[�X�̃N���b�v��
//	const int	SOURCE_H = 0;			//	�摜�\�[�X�̃N���b�v����
//	int			width = 0;				//�@�摜�`�敝
//	int			height = 0;				//�@�摜�`�捂��
//	POINT		pos = {0, 0};			//	�ʒu
//	POINT		start = {0, 0};			//	���̈ʒu
//	POINT		end = {0, 0};			//	�ړ���̈ʒu
//	POINT		shift = {0, 0};			//	�J�����p���ꕝ
//	float		speed = .0f;			//	���x
//
//	int			flag = 0;				//�@�t���O���d
//	int			state = 0;				//�@������
//	int			fallpow = 0;			//�@���s���o�p
//	bool		SuccessFlg = false;		//�@�������o�p
//	int			SuccessCnt = 0;			//�@�������o�p
//
//	//�t���O��ށ��d
//	enum
//	{
//		FLAG_NOT,
//		FLAG_RED,
//		FLAG_GREEN,
//		FLAG_BLUE,
//	};
//	//���v�����ԁ��d
//	enum
//	{
//		STATE_WAIT,
//		STATE_SUCCESS,
//		STATE_BERAK,
//	};
//
//	POINT		length;		//	����
//	float		org_speed;	//	���̃X�s�[�h
//	float		cul_speed;	//	���ݑI�𒆂̂��������̃X�s�[�h
//	int			selectNum;	//	�I�𒆂̔ԍ�
//	bool		clear;		//	�N���A
//	float		t;			//	����
//	int			step;		//	�X�e�b�v
//	bool		CheckFlg;	//�@�`�F�b�N�J�n���d
//
//	//�@���莸�s���p
//	static const int SHAKE_TIME_MAX = 30;
//	const float SHAKE_POWER = 10.0f;
//	bool		ShakeFlg;
//	int			ShakeCnt;
//	Vector2		ShakeVar;
//
//	//�@���萬�����p
//	static const int SUCCESS_TIME_MAX = 30;
//	const float SUCCESS_SCALE_MAX = 2.0f;
//
//	//�����t���b�O
//	int DefaultHaveFlag[4];
//	int HaveFlag[4];
//
//public:
//	TimeObject();
//	TimeObject(int sw, int sh);
//	virtual ~TimeObject();
//
//	virtual void Update() = 0;
//	virtual void Render() = 0;
//	virtual bool CheckWithin(const POINT& p) { return false; };
//	virtual POINT& GetPosCC() = 0;
//	void SetPosCC(const POINT& pos);
//	void SetShift(const POINT& shift);
//	void SetStart(const POINT& pos);
//
//	//�@�Q�[���{�ғ���֐�
//	bool	CheckWithin(const POINT& p, const int& startX, const int& startY, const int size);	//	�͈͓����ǂ������ׂ�
//	void	Lerp(POINT& out, const POINT p1, const POINT p2, float t);	//	POINT���
//	void	Control(void);	//	����
//	void	CalcPos(void);	//	�ʒu�֌W�v�Z
//
//	void	Check();			//�@���聦�d
//	void	ClearCheck();		//�@���聦�d
//	void	Shake();
//	void	FlagUpdate();
//};

// �摜�̕`��p�����[�^
struct ImageParam
{
	int x, y;
	int w, h;
	ImageParam()
	{
		x = y = w = h = 0;
	}
	ImageParam(int x, int y, int w, int h):x(x), y(y), w(w), h(h) {}
};

class Number
{
	//---------- field -----------
private:
	enum
	{
		MINUS=-1
	};
	iex2DObj*	image;		//���ȊǗ�
	const int	WIDTH;
	const int	HEIGHT;

	//---------- method ------------
public:
	Number();
	~Number();
	void Init();
	void RenderCC(int num, int x, int y, float scale,float alpha=1.0f, bool campus = true);
private:
	void Render(int n, int x, int y, float scale, float alpha,bool campus);
	inline int idxCalc(int base, int exponent);
};

// TimeObj�̉摜�Ǘ�
class ImageFactory
{
	//------- field ---------
public:
	enum ImageID
	{
		CLOCK_BACK, CLOCK_HOUR, CLOCK_MINUTE, FRAG_BLACK,FRAG_GOLD
	};
private:
	std::map<int, iex2DObj*> imageList;				// �摜�f�[�^
	std::map<int, ImageParam> paramList;			// �摜�\�[�X�p�����[�^

	//-------- method ----------
public:
	~ImageFactory();
	void Init();
	iex2DObj* GetImage(ImageID id);
	ImageParam GetParam(ImageID id);
};

class TimeObj
{
	//---------- field -------------
public:
	enum State
	{
		MOVE, STOP, CHECK
	}state = MOVE;
	bool IsShuffled = false;
protected:
	int id = 0;
	std::map<int, iex2DObj*> imageList;				// �摜�f�[�^(�O���Ǘ�)
	std::map<int, ImageParam> dst;					// �摜�`��p�����[�^
	std::map<int, ImageParam> src;					// �摜�\�[�X�p�����[�^
	Vector2 pos;									// ���S�̃��[���h���W
	int colw = 0, colh = 0;							// �����蔻��p������
	float scale = 0.0f;								// �`��T�C�Y
	float orginSpeed = 0.0f;						// ���Ԍo�ߐ�΃X�s�[�h
	float speed = 0.0f;								// ���Ԍo�ߑ��΃X�s�[�h
	int behavior = 0;								// �����A���S���Y��

	//------------- method ----------------
public:
	TimeObj();
	virtual ~TimeObj();
	virtual void AppendImage(int idx, iex2DObj* image, const ImageParam& source);
	int GetID()const;
	const Vector2& GetPos()const;
	virtual void SetPos(const Vector2& pos);
	void SetOrginSpeed(float speed);
	float GetOrginSpeed();
	// ��ƂȂ�X�s�[�h�����Ƃɑ��΃X�s�[�h������
	void SetRelativeSpeed(float orginSpeed);
	// ���[���h���W�Ƃ̓����蔻��(��`)
	bool IsCollision(const POINT& p);
	//					�`��̒��S���W�A�����蔻��̕������A�g�嗦�A�@�@�@���̃X�s�[�h�A�@�@����ID
	void Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float orginSpeed, int behavior);
	virtual void Update() = 0;
	virtual void Render() = 0;
	void SetState(TimeObj::State s);
};

//class TimeObjMgr
//{
//protected:
//	const static int OBJ_MAX = 64;
//	const static int IMAGE_MAX = 8;
//	TimeObject* objList[OBJ_MAX] = {nullptr};
//	iex2DObj* imageList[IMAGE_MAX] = {nullptr};
//	int selectNum = 0;
//	CAMERA camera;
//
//public:
//	TimeObjMgr();
//	~TimeObjMgr();
//	void Init();
//	void Update();
//	void Render();
//protected:
//	void CalcPos();
//	void Control();
//
//};

#endif