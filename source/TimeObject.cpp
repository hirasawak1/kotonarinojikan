#include	"TimeObject.h"
#include	"../IEX/iextreme.h"
#include	"Utility.h"

ImageFactory::~ImageFactory()
{
	for (std::pair<const int, iex2DObj*>& r : imageList)
		SafeDelete(r.second);
}

void ImageFactory::Init()
{
	imageList[CLOCK_BACK] = new iex2DObj("DATA/�A�i���O���v.png");
	paramList[CLOCK_BACK] = ImageParam(0, 0, 512, 512);
	imageList[CLOCK_HOUR] = new iex2DObj("DATA/�A�i���O���v(�Z���j�j.png");
	paramList[CLOCK_HOUR] = ImageParam(0, 0, 512, 512);
	imageList[CLOCK_MINUTE] = new iex2DObj("DATA/�A�i���O���v(�����j�j.png");
	paramList[CLOCK_MINUTE] = ImageParam(0, 0, 512, 512);
}

iex2DObj* ImageFactory::GetImage(ImageID id)
{
	return imageList[id];
}

ImageParam ImageFactory::GetParam(ImageID id)
{
	return paramList[id];
}


TimeObj::TimeObj()
{
}

TimeObj::~TimeObj()
{
}

void TimeObj::AppendImage(int idx, iex2DObj* image, const ImageParam& param)
{
	imageList[idx] = image;
	paramList[idx] = param;
}

const Vector2& TimeObj::GetPos()
{
	return pos;
}

void TimeObj::SetPos(const Vector2& pos)
{
	this->pos = pos;
}

float TimeObj::GetOrginSpeed()
{
	return orginSpeed;
}

void TimeObj::SetRelativeSpeed(float orginSpeed)
{
	speed = this->orginSpeed / orginSpeed;
}

bool TimeObj::IsCollision(const POINT& p)
{
	return p.x >= pos.x - w / 2 && p.x <= pos.x + w / 2 &&
		p.y >= pos.y - h / 2 && p.y <= pos.y + h / 2;
}

void TimeObj::Init(const Vector2& centerPos, int colW, int colH, float scale, float orginSpeed, int behavior)
{
	pos = centerPos;
	w = colW;
	h = colH;
	orginSpeed = orginSpeed;
	this->scale = scale;
	this->orginSpeed = orginSpeed;
	this->behavior = behavior;
}

Gimmick::Gimmick()
{
	obj = nullptr;
}

Gimmick::Gimmick(TimeObj* obj)
{
	this->obj = obj;
}

Gimmick::~Gimmick()
{

}

MoveGmk::MoveGmk() :SPEED(2.0f)
{
	nowNode = node.end();
	ringLoop = false;
}

MoveGmk::MoveGmk(TimeObj* obj) : MoveGmk()
{
	this->obj = obj;
}

void MoveGmk::Update()
{
	float moveX = nowNode->x - obj->GetPos().x;
	float moveY = nowNode->y - obj->GetPos().y;
	float len = sqrtf(moveX*moveX + moveY*moveY);
	if (len <= SPEED * 2)// �ړ��|�C���g�ɓ��B
	{
		if (ringLoop)// ���[�v���@�F�����O���[�v
		{
			nowNode++;
			if (nowNode == node.end())
				nowNode = node.begin();
		}
		else// ���[�v���@�F�܂�Ԃ����[�v
		{
			if (searchOrder)// �C�e���[�^�F������
			{
				nowNode++;
				if (nowNode == node.end())// �������B
				{
					nowNode--;
					searchOrder = false;
				}
			}
			else// �C�e���[�^�F�t����
			{
				if (nowNode == node.begin())// �擪���B
				{
					nowNode++;
					searchOrder = true;
				}
				else
					nowNode--;
			}
		}
		// �x�N�g���Čv�Z
		moveX = nowNode->x - obj->GetPos().x;
		moveY = nowNode->y - obj->GetPos().y;
		len = sqrtf(moveX*moveX + moveY*moveY);
	}
	moveX /= len;
	moveY /= len;
	moveX *= SPEED;
	moveY *= SPEED;

	Vector2 newPos;
	newPos.x = obj->GetPos().x + moveX;
	newPos.y = obj->GetPos().y + moveY;


	obj->SetPos(newPos);
}

void MoveGmk::Render()
{
	// do nothing
}

void MoveGmk::AppendNode(const Vector2& node)
{
	this->node.push_back(node);
	nowNode = this->node.begin();
}

void MoveGmk::SetLoop(bool IsRingLoop)
{
	ringLoop = IsRingLoop;
	searchOrder = true;
}


//TimeObject::TimeObject() :TimeObject(0, 0)
//{
//
//}
//
//TimeObject::TimeObject(int sw, int sh) : SOURCE_W(sw), SOURCE_H(sh)
//{
//
//}
//
//TimeObject::~TimeObject()
//{
//	SafeDelete(image);
//}
//
//
//bool TimeObject::CheckWithin(const POINT& p)
//{
//	if ((pos.x < p.x) && (p.x < pos.x + width))
//	{
//		if ((pos.y < p.y) && (p.y < pos.y + height))
//			return true;
//	}
//	return false;
//}
//
////�@�Q�[���{�ғ���֐�
//bool	TimeObject::CheckWithin(const POINT& p, const int& startX, const int& startY, const int size)
//{
//
//}
//
//void	TimeObject::Lerp(POINT& out, const POINT p1, const POINT p2, float t)
//{
//	float rate = t * t * (3.0f - 2.0f * t);   // 3���֐���Ԓl�ɕϊ�
//
//	out.x = (long)(p1.x * (1.0f - rate) + p2.x * rate);
//	out.y = (long)(p1.y * (1.0f - rate) + p2.y * rate);
//}
//
//void	TimeObject::Control(void)
//{
//
//}
//
//void	TimeObject::CalcPos(void)
//{
//
//}
//
//void	TimeObject::Check()
//{
//
//}
//
//void	TimeObject::ClearCheck()
//{
//
//}
//
//void	TimeObject::Shake()
//{
//
//}
//
//void	TimeObject::FlagUpdate()
//{
//
//}
//
//
////-----------------------------------------------------------------
////
////      TimeObjMgr class
////
////-----------------------------------------------------------------
//
//TimeObjMgr::TimeObjMgr()
//{
//
//}
//
//TimeObjMgr::~TimeObjMgr()
//{
//	for (TimeObject*& r : objList)
//		SafeDelete(r);
//	for (iex2DObj*& r : imageList)
//		SafeDelete(r);
//}
//
//void TimeObjMgr::Init()
//{
//	for (TimeObject*& r : objList)
//		SafeDelete(r);
//	for (iex2DObj*& r : imageList)
//		SafeDelete(r);
//
//	// �摜�f�[�^�Ƃ��X�e�[�W�f�[�^�Ƃ����[�h
//}
//
//void TimeObjMgr::Update()
//{
//
//}
//
//void TimeObjMgr::Render()
//{
//
//}
//
//void TimeObjMgr::CalcPos()
//{
//	float cx, cy, vx, vy;
//	cx = 640;
//	cy = 360;
//	float	l;
//
//	for (int i = 0; i < OBJ_MAX; i++)
//	{
//		vx = objList[i]->GetPosCC().x - cx;
//		vy = objList[i]->GetPosCC().y - cy;
//
//		l = sqrt(vx * vx + vy *vy);
//
//		vx /= l;
//		vy /= l;
//
//		if (selectNum != i)
//			objList[i]->SetShift({(int)(vx * m_Camera.scale * 2), (int)(vy * m_Camera.scale * 2)});
//	}
//}
//
//void TimeObjMgr::Control()
//{
//	//�ړ����Ȃ�return
//	if (t < 1.0f)return;
//	//	�G���^�[�őI��
//	if (KEY(KEY_ENTER) == 3)
//	if (obj[selectNum].speed == obj[0].speed && selectNum != 0)
//		clear = true;
//
//	//��I�u�W�F�N�g�I�𔻒�
//	for (int i = 0; i < OBJ_MAX; i++)
//	{
//		//�I���ς݂Ȃ�X���[
//		if (i == selectNum)continue;
//		//�}�E�X�N���b�N�`�F�b�N
//		if (Mouse::Click() && objList[i]->CheckWithin(Mouse::cursor))
//		{
//			selectNum = i;
//			//���̈ʒu��ۑ�
//			for (TimeObject*& r : objList)
//				r->SetStart(r->GetPosCC());
//			t = 0.0f;
//			break;
//		}
//	}
//}