#include "../IEX/iextreme.h"
#include	"system/System.h"
#include	"system/Framework.h"
#include "../EDX/EDXLIB.h"
#include "DataOwner.h"
#include "sceneSelect.h"
#include "sceneTitle.h"
#include "sceneMain.h"
#include "Control.h"
#include "Campus.h"
#include "Pumpkin.h"
#include "TransitionBat.h"


bool sceneSelect::Initialize(){
	FOR(i,STAGEPIC_MAX){
		char str[256];
		if (i<OPEN_STAGE)sprintf(str, "DATA\\ステージ選択画面\\ステージ\\%d.png", i + 1);
		else sprintf(str, "DATA\\ステージ選択画面\\ステージ\\0.png");
		bat[i].Init(str, i * BAT_X_SPACE, (i % 2 == 0) ? -50 : 50);
	}

	Back1 = new EDX::EDX_2DObj("DATA\\ステージ選択画面\\back1.png");
	Back2 = new EDX::EDX_2DObj("DATA\\ステージ選択画面\\back2.png");
	Skip_bat[0] = new EDX::EDX_2DObj("DATA\\ステージ選択画面\\koumori10.png");
	Skip_bat[1] = new EDX::EDX_2DObj("DATA\\ステージ選択画面\\koumori-10.png");

	Next_Title = new EDX::EDX_2DObj("DATA\\ステージ選択画面\\titlehe.png");
	Next_Game = new EDX::EDX_2DObj("DATA\\ステージ選択画面\\kettei.png");

	Campus::GetInst()->Init();
	//カメラ初期位置
	EDX::Vector pos = bat[0].GetPos();
	POINT pointpos = AddPoint(GetPoint(pos.x, pos.y), GetPoint(BAT_PIC_WH / 2, BAT_PIC_WH / 2));
	pointpos.y += CAM_OFFSET_Y;
	Campus::GetInst()->SetPos(pointpos);

	NextScene = nullptr;
	SceneState = SCENE_FADEIN;
	save_SelectStage = SelectStage = 0;
	Pumpkin::GetInst()->Init();
	Pumpkin::GetInst()->Reset(false);
	Pumpkin::GetInst()->SetMaxY();
	TransitionBat::GetInst()->Init();
	TransitionBat::GetInst()->SetNextStep(TransitionBat::TBAT_STATE::UP);
	TransitionBat::GetInst()->TimeReset();
	NextTitle = true;
	WaitTime = 0;
	Next_Alpha = 0;
	return true;
}

void sceneSelect::Update(){
	FOR(i, STAGEPIC_MAX){
		bat[i].Update();
	}
	switch (SceneState){
	case SCENE_FADEIN:S_FADEIN(); break;
	case SCENE_SELECT:S_SELECT(); break;
	case SCENE_NEXT:S_NEXT(); break;
	}
	TransitionBat::GetInst()->Update();
	Campus::GetInst()->Update();
	Pumpkin::GetInst()->Update();
}

void sceneSelect::Render(){
	//かぼちゃ画面
	iexView v; v.Clear();
	//バック
	Back2->Draw(0, 0);
	FOR(i, STAGEPIC_MAX){
		bat[i].Render();
	}
	Campus::GetInst()->Draw();

	Pumpkin::GetInst()->Render();

	//かぼちゃ
	//遷移ボタン
	if (SceneState == SCENE_SELECT && Pumpkin::GetInst()->IsMoveEnd()){
		Next_Alpha+=20;
		if (Next_Alpha > 255)Next_Alpha = 255;
		DWORD sw_alpha = _ARGB(Next_Alpha, 255, 255, 255);
		Next_Title->SetARGB(sw_alpha);
		Next_Game->SetARGB(sw_alpha);
		Skip_bat[0]->SetARGB(sw_alpha);
		Skip_bat[1]->SetARGB(sw_alpha);

		Next_Title->Draw(SWITCH_TITLE_X, SWITCH_Y, SWITCH_TITLE_WIDTH, SWITCH_HEIGHT, 0, 0, Next_Title->info.Width, Next_Title->info.Height);
		Next_Game->Draw(SWITCH_GAME_X, SWITCH_Y, SWITCH_GAME_WIDTH, SWITCH_HEIGHT, 0, 0, Next_Game->info.Width, Next_Game->info.Height);
		Skip_bat[0]->Draw(SKIP_BAT_X0, SKIP_BAT_Y);
		Skip_bat[1]->Draw(SKIP_BAT_X1, SKIP_BAT_Y);
	}
	TransitionBat::GetInst()->Render();

}

sceneSelect::~sceneSelect(){
	if (NextTitle)	Pumpkin::GetInst()->Release();
	TransitionBat::GetInst()->Release();
	FOR(i, STAGEPIC_MAX){
		bat[i].Release();
	}
	SAFE_DELETE(Skip_bat[0]);
	SAFE_DELETE(Skip_bat[1]);
	SAFE_DELETE(Back1);
	SAFE_DELETE(Back2);

	SAFE_DELETE(Next_Title);
	SAFE_DELETE(Next_Game);
	Campus::GetInst()->Init();
}

bool PushCheck(int px, int py,int x,int y,int w,int h){
	if (EDX::MouseGet(EDX::EDX_CLICK_L) != 1)return
		false;
	if (x < px&&px < w + x){
		if (y < py&&py < h + y){
			return true;
		}
	}
	return false;
}

void sceneSelect::S_FADEIN(){
	WaitTime++;
	if (WaitTime > WAIT_TIME){
		Pumpkin::GetInst()->SetOpen(true);
		if (Pumpkin::GetInst()->IsMoveEnd())SceneState++;
	}
}

void sceneSelect::S_SELECT(){
	if (PushCheck(Mouse::cursor.x, Mouse::cursor.y, SWITCH_TITLE_X, SWITCH_Y, SWITCH_TITLE_WIDTH, SWITCH_HEIGHT)){
		NextTitle = true;
		WaitTime = 0;
		Pumpkin::GetInst()->SetOpen(false);
		NextScene = new sceneTitle;
		sceneTitle::IsReturnTitle = true;
		TransitionBat::GetInst()->SetStep(TransitionBat::TBAT_STATE::DOWN);
		TransitionBat::GetInst()->SetNextStep(TransitionBat::TBAT_STATE::CENTER);
		TransitionBat::GetInst()->TimeReset();
		SceneState++;
		return;
	}
	//プレイ画面へ
	if (Campus::GetInst()->IsMoveEnd()){
		if (PushCheck(Mouse::cursor.x, Mouse::cursor.y, SWITCH_GAME_X, SWITCH_Y, SWITCH_GAME_WIDTH, SWITCH_HEIGHT)){
			DataOwner::GetInst()->stageNo = SelectStage + 1;
			NextTitle = false;
			WaitTime = 0;
			Pumpkin::GetInst()->SetOpen(false);
			NextScene = new sceneMain;
			SceneState++;
			return;
		}
	}

	//クリック
	POINT mouse = AddPoint(Campus::GetInst()->GetPos(), Mouse::cursor);
	FOR(i, OPEN_STAGE){
		if (bat[i].ClickCheck(mouse.x, mouse.y))save_SelectStage = i;
	}
	if (PushCheck(Mouse::cursor.x, Mouse::cursor.y, SKIP_BAT_X0, SKIP_BAT_Y, 230, 140)){
		save_SelectStage = SelectStage + 10;
	}
	if (PushCheck(Mouse::cursor.x, Mouse::cursor.y, SKIP_BAT_X1, SKIP_BAT_Y, 230, 140)){
		save_SelectStage = SelectStage - 10;
	}
	if (save_SelectStage <= 0)save_SelectStage = 0;
	if (save_SelectStage >= OPEN_STAGE)save_SelectStage = OPEN_STAGE - 1;

	EDX::Vector pos = bat[SelectStage].GetPos();
	POINT pointpos = AddPoint(GetPoint(pos.x, pos.y), GetPoint(BAT_PIC_WH / 2, BAT_PIC_WH / 2));
	pointpos.y += CAM_OFFSET_Y;
	if (save_SelectStage != SelectStage){
		SelectStage = save_SelectStage;
		Campus::GetInst()->TimeReset();
		Campus::GetInst()->SetNextPos(pointpos);
	}
	else if(!Campus::GetInst()->IsMoveEnd()){
		Campus::GetInst()->SetNextPos(pointpos);
	}
	else{
		Campus::GetInst()->SetPos(pointpos);
	}
}

void sceneSelect::S_NEXT(){
	WaitTime++;
	if ((Pumpkin::GetInst()->IsMoveEnd() && WaitTime > WAIT_TIME*0.5 &&TransitionBat::GetInst()->IsMoveEnd() )||
		(NextTitle&&TransitionBat::GetInst()->IsMoveEnd())){
		MainFrame->ChangeScene(NextScene);
	}
}