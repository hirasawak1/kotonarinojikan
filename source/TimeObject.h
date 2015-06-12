#ifndef _TIMEOBJECT_H_
#define _TIMEOBJECT_H_

//#include "Game.h"
#include "../IEX/iextreme.h"
#include <map>
<<<<<<< HEAD
#include <list>
#include <vector>
=======
>>>>>>> remotes/origin/horita


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
<<<<<<< HEAD
		MINUS_10 = 0, MINUS_9, MINUS_8, MINUS_7, MINUS_6, MINUS_5, MINUS_4, MINUS_3, MINUS_2, MINUS_1, NUM_0,
		PLUS_1, PLUS_2, PLUS_3, PLUS_4, PLUS_5, PLUS_6, PLUS_7, PLUS_8, PLUS_9, PLUS_10
	};
	iex2DObj*				 image;		//���ȊǗ�
	std::map<int, ImageParam> param;

	//---------- method ------------
public:
	~Number();
	void Init();
	void Render(int num, int x, int y, float scale, bool campus = true);
=======
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
>>>>>>> remotes/origin/horita
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
<<<<<<< HEAD
		MOVE, STOP, CHECK
	}state = MOVE;
	bool IsShuffled = false;

=======
		MOVE, STOP, CHECK,SUCCESS
	}state = MOVE;
	bool IsShuffled = false;
>>>>>>> remotes/origin/horita
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
<<<<<<< HEAD
	
=======
	bool haveFlag = false;							// �t���b�O�������
	static const int SUCCESS_EFFECT_TIME = 30;		// �������o���Ԓ萔
	int SuccessCnt = 0;								// �������o�p�ϐ�
	const float CHAIN_MAX_SCALE = 3.0f;				// �A�����o�p�ő�g�嗦
	static const int CHAIN_EFFECT_MAX = 5;			// �A�����o�p�ő�g�嗦���B�A����
	static int SuccessChain;						// �A���J�E���^
	int Mine_SChain;								// �����̘A���J�E���^
	bool Gold_Effect = false;						// ���t���b�O���ʔ�����
>>>>>>> remotes/origin/horita

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
<<<<<<< HEAD
};

// �M�~�b�N���N���X
class Gimmick
{
	//--------- field ----------
protected:
	TimeObj* obj = nullptr;					// �M�~�b�N��K�p����I�u�W�F�N�g(�O���Ǘ�)

	//--------- method -----------
public:
	Gimmick();
	Gimmick(TimeObj* obj);
	virtual ~Gimmick();
	virtual void Update() = 0;
	virtual void Render() = 0;
};

// ���s�ړ��M�~�b�N
class MoveGmk :public Gimmick
{
	//----------- field ------------
private:
	std::list<Vector2> node;				// �ړ��|�C���g
	std::list<Vector2>::iterator nowNode;	// �ړ����̃m�[�h
	bool ringLoop = false;					// �����O���[�v�F�܂�Ԃ����[�v
	bool searchOrder = true;				// �C�e���[�^�̕���(�܂�Ԃ����[�v�p)
	const float SPEED;						// �ړ��X�s�[�h

	//----------- method -----------
public:
	MoveGmk();
	MoveGmk(TimeObj* obj);
	void Update()override;
	void Render()override;
	void AppendNode(const Vector2& node);
	void SetLoop(bool IsRingLoop);
=======
	__forceinline static void ResetChain(){ SuccessChain = 0; }
	__forceinline static void AddChain(){ SuccessChain++; }
	__forceinline static int GetChain(){ return SuccessChain; }
	__forceinline bool GetGold_Effect(){ return Gold_Effect; }
	__forceinline void SetGold_Effect(bool IsGoldEffect){ Gold_Effect = IsGoldEffect; }
	__forceinline int GetSuccessCnt(){ return SuccessCnt; }
	__forceinline void SetMine_SChain(int Chain){ Mine_SChain = Chain; }
>>>>>>> remotes/origin/horita
};

class FlagGmk :public Gimmick
{
	//--------- field --------------
public:
	enum TYPE
	{
		BLACK, GOLD
	};
private:
	iex2DObj*	back;		// �O���Ǘ�
	ImageParam  backParam;
	Number*		number;		// �O���Ǘ�
	Vector2		pos;
	TYPE		type;
	int			num;
	bool		checked;

	//----------- method ------------
public:
	FlagGmk(TimeObj* timeObj);
	~FlagGmk();
	void Init(iex2DObj* back, ImageParam backParam, Number* number);
	void Update()override;
	void Render()override;
	void SetNum(int num);
	int GetNum();
	void SetPos(const Vector2& pos);
	bool IsChecked();
	void SetChecked(bool checked);
	TYPE GetType();
	void SetType(TYPE type);
	void SetBack(iex2DObj* back, ImageParam backParam);
};

class FlagMgr
{
	//----------- field ---------------
private:
	iex2DObj* listBack;
	iex2DObj* blockBack;
	iex2DObj* goldBat;
	std::map<TimeObj*, FlagGmk*> flagList;
	std::map<int, int> speedList;		// <�X�s�[�h,��>
	TimeObj* nowObj;
	int		MissCount;

	//------------ method --------------
public:
	FlagMgr();
	~FlagMgr();
	void Init();
	void Update();
	void Render();
	void SetSpeedList(const std::map<int, int>& list);
	void AppendFlag(TimeObj* obj, bool next = true);
	bool StartCheck();// �`�F�b�N�t�F�[�Y�J�n
	bool CheckNext();// �ړ��J�n�A�Ȃ����false
	void CheckFlag();
	bool IsFinishEffect();// ���o�I��
	POINT GetNowObjPos();
	int		GetMissCount(){ return MissCount; }
	void	SetMissCount(int mc){ MissCount = mc; }
private:
	inline int NextSpeed(int nowSpeed);
	inline int BeforeSpeed(int nowSpeed);
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