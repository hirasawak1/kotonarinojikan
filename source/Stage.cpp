#include	"Stage.h"
#include	"TextLoader.h"
#include	<string>
#include	<sstream>
#include	"Clock.h"
#include	"DataOwner.h"

namespace
{
	const float SPEED = 0.01f;
	const float OK_LEN = 3.0f;
}

Object::~Object()
{
	delete[] childX;
	childX = nullptr;
	delete[] childY;
	childY = nullptr;
}

void Object::Setting(int x, int y, float scale, float speed, int behaviorId, int gimmickId, bool shuffle, int childNum)
{
	this->x = (float)x; this->y = (float)y;
	this->scale = scale; this->speed = speed;
	this->behaviorId = behaviorId;
	this->gimmickId = gimmickId;
	this->canShuffle = shuffle;
	this->childNum = childNum + 1;
	if (childNum > 1)
	{
		type = TYPE::MOVE;
		childX = new int[childNum];
		childY = new int[childNum];

		childX[0] = x;
		childY[0] = y;
	}
}

void Object::SetChild(int id, int x, int y)
{
	childX[id + 1] = x;
	childY[id + 1] = y;
}

void Object::Update()
{
	if (childNum <= 1) return;

	int nextPosId = nowPosId + 1;
	if (nextPosId > childNum)
	{
		nextPosId = 0;
	}
	float moveX = childX[nextPosId] - childX[nowPosId];
	float moveY = childY[nextPosId] - childY[nowPosId];

	float length = sqrtf(moveX*moveX + moveY*moveY);
	moveX /= length;
	moveY /= length;

	moveX *= SPEED;
	moveY *= SPEED;

	x += moveX;
	y += moveY;

	float diffX = childX[nextPosId];
	float diffY = childY[nextPosId];
	length = diffX*diffX + diffY*diffY;
	if (length < OK_LEN*OK_LEN)
	{
		nowPosId++;
	}
}

//*********************************************************************

StageMNG::StageMNG()
{
}

StageMNG::~StageMNG()
{

	for (TimeObj*& r : objList)
		SafeDelete(r);

}

void StageMNG::LoadStage(const int stageNum)
{
	//���̔j��
	objMax = 0;

	//stageNum����t�@�C�����̐ݒ�
	std::stringstream buf;
	buf << stageNum;
	//�t�@�C�������ʕ����̐ݒ�
	std::string fileName = "DATA/BG/stage" + buf.str() + ".sdt";
	TextLoader loader;
	loader.Load((char*)fileName.c_str());
	//�X�e�[�W�^�C�v ��������� 1�����x(frame) ��
	stageType = (TYPE)loader.LoadInt();
	judgeNum = loader.LoadInt();
	judgeTimer = loader.LoadInt();

	//��
	objMax = loader.LoadInt();
	//�ݒ�p�ϐ�
	Vector2 pos;
	float scale, speed;
	int childNum;
	//�ݒ�
	for (int i = 0; i < objMax; i++)
	{//x y scale speed ���̑� �q�� ���[�v ���W...
		pos.x = loader.LoadInt();
		pos.y = loader.LoadInt();
		scale = loader.LoadFloat();
		speed = loader.LoadFloat();
		int behavior = loader.LoadInt();
		int gimmick = loader.LoadInt();
		bool shuffle = (bool)loader.LoadInt();
		childNum = loader.LoadInt();

		TimeObj* obj = nullptr;
		switch (stageType)
		{
			case StageMNG::CLOCK:
				obj = new Clock;
				obj->AppendImage(Clock::BACK,
								 DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::CLOCK_BACK),
								 DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::CLOCK_BACK));
				obj->AppendImage(Clock::HOUR,
								 DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::CLOCK_HOUR),
								 DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::CLOCK_HOUR));
				obj->AppendImage(Clock::MINUTE,
								 DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::CLOCK_MINUTE),
								 DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::CLOCK_MINUTE));
				obj->Init(pos, 512, 512, scale, speed, behavior);
				break;
			case StageMNG::CANDOL:
				break;
			case StageMNG::METRO:
				break;
			default:
				break;
		}
		if (obj != nullptr)
			objList.push_back(obj);

		if (childNum > 0)
		{
			MoveGmk* gmk=new MoveGmk(obj);
			gmk->SetLoop(loader.LoadInt() != 0);
			gmk->AppendNode(pos);
			for (int j = 0; j < childNum; j++)
			{
				pos.x = loader.LoadInt();
				pos.y = loader.LoadInt();
				gmk->AppendNode(pos);
			}
			gimmickList.push_back(gmk);
		}
	}
	float startSpeed = objList.front()->GetOrginSpeed();
	for (TimeObj*& r : objList)
	{
		r->SetRelativeSpeed(startSpeed);
	}
}

void StageMNG::Update()
{
	for (TimeObj*& r : objList)
		r->Update();
	for (Gimmick*& r : gimmickList)
		r->Update();
}

void StageMNG::Render()
{
	for (TimeObj*& r : objList)
		r->Render();
	for (Gimmick*& r : gimmickList)
		r->Render();
}