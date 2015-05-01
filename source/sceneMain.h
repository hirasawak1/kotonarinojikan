#ifndef _SCENEMAIN_H_
#define _SCENEMAIN_H_

#include <deque>
#include "Delegate.h"
#include	"Game.h"

class sceneMain :public Scene
{
	//-------- field ---------
private:
	enum UpdateDgt
	{
		CAMPUS
	};
	enum RenderDgt
	{
		BACK, BIGCLOCK, CLOCK_SELECT,CLOCK_MAIN, STAGE,GAME
	};
	enum UpdateStep
	{
		ZOOM_IN,ZOOM_OUT
	};
	DelegateMgr updateDgt, renderDgt;
	std::deque<bool(sceneMain::*)(void)> mainQueue;
	int step;

#ifdef _DEBUG
	char str[64];
#endif
	//-------- method ---------
public:
	sceneMain();
	~sceneMain();
	bool Initialize()override;
	void Update()override;
	void Render()override;

	bool StageSelect_Intro();
	bool AppendStage();
	bool StageSelect();
	bool GameMain_Intro();
	bool GameMain();
	bool GameMain_Outro();
};

#endif //#ifndef _SCENEMAIN_H_
