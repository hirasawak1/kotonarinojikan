#ifndef _SCENEMAIN_H_
#define _SCENEMAIN_H_

#include	"Game.h"
class StageMNG;
class FlagMgr;

class sceneMain :public Scene
{
	//-------- field ---------
private:
	enum State
	{
		BEGIN,MAIN,PAUSE,CHECK,END
	}state;

	struct CheckClock
	{
		POINT	pos;			//�@�ʒu
		float	ShortAngle;		//�@�Z�j�p�x
		float	LongAngle;		//	���j�p�x
	};
	iex2DObj*	rCheckClock;
	
	StageMNG* stage;
	FlagMgr* flag;
	CheckClock	check_obj;	//�@���莞�v�\����
	int judgeNum = 0;
	int judgeTimer = 0;
	int stageID;// �v���b�g�񋟗p
	iex2DObj* back;// �v���b�g�񋟗p

	static int timelimit;



	//-------- method ---------
public:
	sceneMain();
	~sceneMain();
	bool Initialize()override;
	void Update()override;
	void Render()override;

	int GetTimelimit(){ return timelimit; }
	static void SetTimelimit(int tl){ timelimit = tl; }
	static void DecreaseTimit(int minus){ SetTimelimit(timelimit - minus); }
};

#endif //#ifndef _SCENEMAIN_H_
