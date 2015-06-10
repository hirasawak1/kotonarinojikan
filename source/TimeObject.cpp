#include	"TimeObject.h"
#include	"../IEX/iextreme.h"
#include	"Utility.h"
#include	"Campus.h"

ImageFactory::~ImageFactory()
{
	for(std::pair<const int, iex2DObj*>& r : imageList)
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
	imageList[FRAG_BLACK] = new iex2DObj("DATA/�Q�[�����/-10�`10�̃R�E��������/koumori.png");
	paramList[FRAG_BLACK] = ImageParam(0, 0, 256, 128);
	imageList[FRAG_GOLD] = new iex2DObj("DATA/�Q�[�����/-10�`10�̃R�E��������/koumori_kin.png");
	paramList[FRAG_GOLD] = ImageParam(0, 0, 256, 128);
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
{}

TimeObj::~TimeObj()
{}

void TimeObj::AppendImage(int idx, iex2DObj* image, const ImageParam& param)
{
	imageList[idx] = image;
	dst[idx] = ImageParam();
	src[idx] = param;
}

int TimeObj::GetID()const
{
	return id;
}

const Vector2& TimeObj::GetPos()const
{
	return pos;
}

void TimeObj::SetPos(const Vector2& pos)
{
	this->pos = pos;
}

void TimeObj::SetOrginSpeed(float speed)
{
	orginSpeed = speed;
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
	return (p.x >= pos.x - colw*scale / 2) && (p.x <= pos.x + colw*scale / 2) &&
		(p.y >= pos.y - colh*scale / 2) && (p.y <= pos.y + colh*scale / 2);
}

void TimeObj::Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float orginSpeed, int behavior)
{
	this->id = id;
	pos = centerPos;
	colw = colW;
	colh = colH;
	orginSpeed = orginSpeed;
	this->scale = scale;
	this->orginSpeed = orginSpeed;
	this->behavior = behavior;
	state = State::MOVE;
}

void TimeObj::SetState(TimeObj::State s)
{
	state = s;
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