#include	"iextreme.h"
#include	"system/system.h"

#include	"sceneMain.h"
#include	"DataOwner.h"
#include	"stub.h"


sceneMain::sceneMain(void)
{
}

bool sceneMain::Initialize()
{
	//	���ݒ�
	iexLight::SetFog(800, 1000, 0);
	//	����

	DataOwner::GetInst()->Init();

	updateDgt = updateDgt.Cleate(CAMPUS, Campus::Inst(), &Campus::Update);

	renderDgt.Cleate(BACK, DataOwner::GetInst()->back, &BackHoge::Render);
	renderDgt.Cleate(BIGCLOCK, DataOwner::GetInst()->bigClock, &BigClockHoge::Render);
	renderDgt.Cleate(CLOCK_SELECT, DataOwner::GetInst()->clock, &ClockHoge::RenderSelect);
	renderDgt.Cleate(CLOCK_MAIN, DataOwner::GetInst()->clock, &ClockHoge::Render);
	renderDgt.Cleate(STAGE, DataOwner::GetInst()->stage, &StageHoge::Render);

	mainQueue.push_back(&sceneMain::StageSelect_Intro);
	step = 0;

	game = new Game;

	return true;
}

sceneMain::~sceneMain()
{
	delete	game;
}

void sceneMain::Update()
{
	updateDgt.RetentionRun();
	if ((this->*mainQueue.front())())
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

//�@��ԏ��߁i��񂾂��j
bool sceneMain::StageSelect_Intro()
{
	wsprintf(str, "StageSelect_Intro");

	renderDgt = BACK;
	renderDgt += BIGCLOCK;
	renderDgt += CLOCK_SELECT;

	if (Campus::Inst()->IsZoomEnd())
	{
		mainQueue.push_back(&sceneMain::AppendStage);
		Vector3 p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount());
		Campus::Inst()->Zoom(p.x, p.y, 7.0f);
		return true;
	}
	else
	{
		Campus::Inst()->Zoom(970, 218, 2.5f);
		return false;
	}
}

//�@�X�e�[�W�N���A�㎩���I�Ɏ��̃X�e�[�W��
bool sceneMain::AppendStage()
{
	wsprintf(str, "AppendStage");

	renderDgt = BACK;
	renderDgt += BIGCLOCK;
	renderDgt += CLOCK_SELECT;
	if (Campus::Inst()->IsZoomEnd())
	{
		mainQueue.push_back(&sceneMain::StageSelect);
		DataOwner::GetInst()->clock->Append();
		return true;
	}
	else
	{
		return false;
	}
}

//�@�X�e�[�W�I�����
bool sceneMain::StageSelect()
{
	wsprintf(str, "StageSelect");

	renderDgt = BACK;
	renderDgt += BIGCLOCK;
	renderDgt += CLOCK_SELECT;
	if (KEY_Get(KEY_ENTER) == 3)// �X�e�[�W�I��
	{
		mainQueue.push_back(&sceneMain::GameMain_Intro);
		Vector3 p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount() - 1);
		Campus::Inst()->Zoom(p.x, p.y, 25.0f);
		return true;
	}
	return false;
}

//�@�X�e�[�W����Ƃ��i�Y�[������Ƃ��j
bool sceneMain::GameMain_Intro()
{
	wsprintf(str, "GameMain_Intro");
	switch (step)
	{
		case 0:
			if (Campus::Inst()->IsZoomEnd())
			{
				Vector3 p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount() - 1);
				Campus::Inst()->Zoom(p.x, p.y, 7.0f);
				step++;
			}
			renderDgt = BIGCLOCK;
			renderDgt += CLOCK_SELECT;
			return false;
			break;
		case 1:
			renderDgt = STAGE;
			renderDgt += CLOCK_MAIN;
			if (Campus::Inst()->IsZoomEnd())
			{
				mainQueue.push_back( &sceneMain::GameMain);
				step = 0;
				return true;
			}
			else
			{
				return false;
			}
			break;
	}
}

//�@�v���C���
bool sceneMain::GameMain()
{
	wsprintf(str, "GameMain");

	renderDgt = STAGE;
	renderDgt += CLOCK_MAIN;
	if (KEY_Get(KEY_ENTER) == 3)// �Q�[���N���A
	{
		mainQueue.push_back(&sceneMain::GameMain_Outro);
		Vector3 p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount() - 1);
		Campus::Inst()->Zoom(p.x, p.y, 25.0f);
		return true;
	}
	return false;
}

//�@�N���A���ăv���C��ʂ���X�e�[�W�I����ʂ֖߂�Ƃ��i�Y�[���A�E�g����Ƃ��j
bool sceneMain::GameMain_Outro()
{
	wsprintf(str, "GameMain_Outro");

	switch (step)
	{
		case 0:
			renderDgt = STAGE;
			renderDgt += CLOCK_MAIN;
			if (Campus::Inst()->IsZoomEnd())
			{
				if (DataOwner::GetInst()->clock->GetCount() < 12)
				{
					Vector3 p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount());
					Campus::Inst()->Zoom(p.x, p.y, 7.0f);
				}
				else
				{
					Campus::Inst()->Zoom(978, 218, 2.5f);
				}
				step++;
			}
			return false;
			break;
		case 1:
			renderDgt = BACK;
			renderDgt += BIGCLOCK;
			renderDgt += CLOCK_SELECT;
			mainQueue.push_back(&sceneMain::AppendStage);
			step = 0;
			return true;
			break;
	}
}

