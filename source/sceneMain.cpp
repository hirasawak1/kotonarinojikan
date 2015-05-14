#include	"iextreme.h"
#include	"system/system.h"

#include	"sceneMain.h"
#include	"DataOwner.h"
#include	"Stage.h"
#include	"stub.h"
#include	"ChoicesMgr.h"


sceneMain::sceneMain(void)
{

}

bool sceneMain::Initialize()
{
	//	���ݒ�
	iexLight::SetFog(800, 1000, 0);
	//	����
	DataOwner::GetInst()->Init();

	// �A�b�v�f�[�g���\�b�h�o�^
	updateDgt = updateDgt.Cleate(CAMPUS, Campus::Inst(), &Campus::Update);
	// �����_�[���\�b�h�o�^
	renderDgt.Cleate(BACK, DataOwner::GetInst()->back, &BackHoge::Render);
	renderDgt.Cleate(BIGCLOCK, DataOwner::GetInst()->bigClock, &BigClockHoge::Render);
	renderDgt.Cleate(CLOCK_SELECT, DataOwner::GetInst()->choiceClock, &ChoicesMgr::Render);
	renderDgt.Cleate(CLOCK_MAIN, DataOwner::GetInst()->clock, &ClockHoge::Render);
	//renderDgt.Cleate(STAGE, DataOwner::GetInst()->stage, &StageHoge::Render);
	renderDgt.Cleate(GAME, DataOwner::GetInst()->gameMain, &Clock::Render);

	// ���C���L���[�ݒ�
	mainQueue.push_back(&sceneMain::StageSelect_Intro);
	Campus::Inst()->Zoom(970, 218, 2.5f);
	step = ZOOM_IN;

	return true;
}

sceneMain::~sceneMain()
{
}

void sceneMain::Update()
{
	updateDgt.RetentionRun();
	if (!(this->*mainQueue.front())())
	{
		mainQueue.pop_front();
	}
}

void sceneMain::Render()
{
	//	��ʃN���A
	DataOwner::GetInst()->view->Activate();
	DataOwner::GetInst()->view->Clear(0x00000080);

	renderDgt();

#ifdef _DEBUG
	char text[128];
	wsprintf(text, "Mode %s\n", str);
	IEX_DrawText(text, 10, 30, 200, 20, 0xFF808020);
#endif
}

//�@�^�C�g������̑J��
bool sceneMain::StageSelect_Intro()
{
	wsprintf(str, "StageSelect_Intro");

	DataOwner::GetInst()->choiceClock->Update();

	renderDgt = BACK;
	renderDgt += BIGCLOCK;
	renderDgt += CLOCK_SELECT;

	if (Campus::Inst()->IsZoomEnd())
	{
		// To AppendStage
		mainQueue.push_back(&sceneMain::AppendStage);
		POINT p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount());
		Campus::Inst()->Zoom(p.x, p.y, 7.0f);
		return false;
	}
	return true;
}

//�@�X�e�[�W�ǉ����o
bool sceneMain::AppendStage()
{
	wsprintf(str, "AppendStage");

	DataOwner::GetInst()->choiceClock->Update();

	renderDgt = BACK;
	renderDgt += BIGCLOCK;
	renderDgt += CLOCK_SELECT;

	if (Campus::Inst()->IsZoomEnd())
	{
		// To StageSelect
		mainQueue.push_back(&sceneMain::StageSelect);
		DataOwner::GetInst()->clock->Append();
		return false;
	}
	return true;
}

//�@�X�e�[�W�I�����
bool sceneMain::StageSelect()
{
	wsprintf(str, "StageSelect");

	DataOwner::GetInst()->choiceClock->Update();

	renderDgt = BACK;
	renderDgt += BIGCLOCK;
	renderDgt += CLOCK_SELECT;

	if (KEY_Get(KEY_ENTER) == 3)// �X�e�[�W�I��
	{
		// To GameMain Intro
		mainQueue.push_back(&sceneMain::GameMain_Intro);
		POINT p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount() - 1);
		Campus::Inst()->Zoom(p.x, p.y, 25.0f);
		step = ZOOM_IN;
		return false;
	}
	return true;
}

//�@�Q�[�����C���������o
bool sceneMain::GameMain_Intro()
{
	wsprintf(str, "GameMain_Intro");


	switch (step)
	{
		case ZOOM_IN:
			DataOwner::GetInst()->choiceClock->Update();
			renderDgt = BIGCLOCK;
			renderDgt += CLOCK_SELECT;
			if (Campus::Inst()->IsZoomEnd())
			{
				// �X�e�[�W���[�h
				DataOwner::GetInst()->gameMain->Initialize();
				//DataOwner::GetInst()->gameMain->LoadStage(DataOwner::GetInst()->clock->GetCount());
				POINT p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount() - 1);
				Campus::Inst()->Zoom(p.x, p.y, 7.0f);
				step = ZOOM_OUT;
			}
			break;
		case ZOOM_OUT:
			renderDgt = GAME;
			renderDgt += CLOCK_MAIN;
			if (Campus::Inst()->IsZoomEnd())
			{
				// To GameMain
				mainQueue.push_back(&sceneMain::GameMain);
				step = ZOOM_IN;
				return false;
			}
			break;
	}
	return true;

}

//�@�Q�[�����C�����
bool sceneMain::GameMain()
{
	wsprintf(str, "GameMain");

	renderDgt = GAME;

	// �Q�[�����C������
	DataOwner::GetInst()->gameMain->Update();
	//if (KEY_Get(KEY_D) == 3)//�@�Q�[������߂�
	//{
	//	mainQueue.push_back(&sceneMain::StageSelect_Intro);
	//}
	if (KEY_Get(KEY_C) == 3)//�@�Q�[���N���A
	{
		// To GameMain Outro
		mainQueue.push_back(&sceneMain::GameMain_Outro);
		POINT p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount() - 1);
		Campus::Inst()->Zoom(p.x, p.y, 25.0f);
		step = ZOOM_IN;
		return false;
	}
	return true;

}

//�@�Q�[�����C���N���A��̉��o
bool sceneMain::GameMain_Outro()
{
	wsprintf(str, "GameMain_Outro");

	switch (step)
	{
		case ZOOM_IN:
			renderDgt = GAME;
			renderDgt += CLOCK_MAIN;

			if (Campus::Inst()->IsZoomEnd())
			{
				if (DataOwner::GetInst()->clock->GetCount() < 12)//���v�̃X�e�[�W���ׂĉ������
				{
					POINT p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount());
					Campus::Inst()->Zoom(p.x, p.y, 7.0f);
				}
				else
				{
					Campus::Inst()->Zoom(978, 218, 2.5f);//�厞�v����
				}
				step++;
			}
			break;
		case ZOOM_OUT:
			renderDgt = BACK;
			renderDgt += BIGCLOCK;
			renderDgt += CLOCK_SELECT;

			DataOwner::GetInst()->choiceClock->Update();
			// To Append Stage
			mainQueue.push_back(&sceneMain::AppendStage);
			step = ZOOM_IN;
			return false;
			break;
	}
	return true;

}

