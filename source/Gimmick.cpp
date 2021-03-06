#include "Gimmick.h"
#include "DataOwner.h"
#include "Campus.h"

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

TimeObj* Gimmick::GetObj()
{
	return obj;
}

MoveGmk::MoveGmk():SPEED(2.0f)
{
	nowNode = node.end();
	ringLoop = false;
}

MoveGmk::MoveGmk(TimeObj* obj): MoveGmk()
{
	this->obj = obj;
}

void MoveGmk::Update()
{
	float moveX = nowNode->x - obj->GetPos().x;
	float moveY = nowNode->y - obj->GetPos().y;
	float len = sqrtf(moveX*moveX + moveY*moveY);
	if(len <= SPEED * 2)// 移動ポイントに到達
	{
		if(ringLoop)// ループ方法：リングループ
		{
			nowNode++;
			if(nowNode == node.end())
				nowNode = node.begin();
		}
		else// ループ方法：折り返しループ
		{
			if(searchOrder)// イテレータ：順方向
			{
				nowNode++;
				if(nowNode == node.end())// 末尾到達
				{
					nowNode--;
					searchOrder = false;
				}
			}
			else// イテレータ：逆方向
			{
				if(nowNode == node.begin())// 先頭到達
				{
					nowNode++;
					searchOrder = true;
				}
				else
					nowNode--;
			}
		}
		// ベクトル再計算
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

FlagGmk::FlagGmk(TimeObj* timeObj):Gimmick(timeObj)
{
	back = nullptr;
	number = nullptr;
	checked = false;
	alpha = 1.0f;
	fromOffset = Vector2(0, 0);
	targetOffset = Vector2(0, 0);
	effectTime = 0.0f;
	state = FADE_IN;
}

FlagGmk::~FlagGmk() {}

void FlagGmk::Init(iex2DObj* back, ImageParam backParam, Number* number)
{
	this->back = back;
	this->backParam = backParam;
	this->number = number;
	pos = Vector2(0, 0);
	num = 0;
	checked = false;
	alpha = 1.0f;
	SetState(State::FADE_IN);
}

void FlagGmk::Update()
{
	pos = obj->GetPos();

	if(state == STAY)return;

	effectTime += 1.0f / 30;
	if(effectTime > 1.0f)
		effectTime = 1.0f;
	switch(state)
	{
		case FlagGmk::FADE_IN:
			alpha += 1.0f / 30;
			if(alpha >= 1.0f&&effectTime >= 1.0f)
			{
				alpha = 1.0f;
				state = State::STAY;
			}
			break;
		case FlagGmk::FADE_OUT:
			alpha -= 1.0f / 30;
			if(alpha <= 0.0f&&effectTime >= 1.0f)
				alpha = 0.0f;
			break;
	}

}

void FlagGmk::Render()
{
	float rate = effectTime * effectTime * (3.0f - 2.0f * effectTime);   // 3次関数補間値に変換
	Vector2 offset = Vector2(
		fromOffset.x * (1.0f - rate) + targetOffset.x * rate,
		fromOffset.y * (1.0f - rate) + targetOffset.y * rate);

	Campus::GetInst()->Add(back,
						   pos.x + offset.x - backParam.w / 2 * 0.7f,
						   pos.y + offset.y - backParam.h * 2 + 30,
						   backParam.w*0.7f, backParam.h*0.7f,
						   backParam.x, backParam.y, backParam.w, backParam.h, GetPoint(0, 0), 0.0f, 1.0f, 0UL, ARGB((int)(0xFF * alpha), 0xFF, 0xFF, 0xFF));
	number->RenderCC(num, pos.x + offset.x - 10, pos.y + offset.y - backParam.h * 2 + 70, 0.5f, alpha);
}

void FlagGmk::SetNum(int num)
{
	this->num = num;
}

int FlagGmk::GetNum()
{
	return num;
}

void FlagGmk::SetPos(const Vector2& pos)
{
	this->pos = pos;
}

bool FlagGmk::IsChecked()
{
	return checked;
}

void FlagGmk::SetChecked(bool checked)
{
	this->checked = checked;
}

FlagGmk::TYPE FlagGmk::GetType()
{
	return type;
}

void FlagGmk::SetType(FlagGmk::TYPE type)
{
	this->type = type;
}

void FlagGmk::SetBack(iex2DObj* back, ImageParam backParam)
{
	this->back = back;
	this->backParam = backParam;
}

void FlagGmk::SetState(FlagGmk::State state)
{
	this->state = state;
	effectTime = 0.0f;
	switch(state)
	{
		case FlagGmk::FADE_IN:
			fromOffset = Vector2(30, -30);// 右上からフェードイン
			targetOffset = Vector2(0, 0);
			alpha = 0.0f;
			break;
		case FlagGmk::STAY:
			fromOffset = Vector2(0, 0);
			targetOffset = Vector2(0, 0);
			alpha = 1.0f;
			break;
		case FlagGmk::FADE_OUT:
			fromOffset = Vector2(0, 0);
			targetOffset = Vector2(-30, -30);// 左上にフェードアウト
			alpha = 1.0f;
			break;
	}
}

FlagGmk::State FlagGmk::GetState()
{
	return state;
}

bool FlagGmk::IsEnd()
{
	return state == FADE_OUT&&effectTime <= 1.0f&&alpha <= 0.0f;
}

FlagMgr::FlagMgr()
{
	listBack = nullptr;
	blockBack = nullptr;
	goldBack = nullptr;
	state = State::MAIN;
	nowFlag = blackFlag.end();
	haveGold = 0;
}

FlagMgr::~FlagMgr()
{
	ReleaseImage();
	ReleaseList();
}

void FlagMgr::Init()
{
	ReleaseImage();
	ReleaseList();
	listBack = new iex2DObj("DATA/ゲーム画面/UI枠.png");
	blockBack = new iex2DObj("DATA/ゲーム画面/スピードボード.png");
	goldBack = new iex2DObj("DATA/ゲーム画面/コウモリ残数.png");

	state = State::MAIN;
}

void FlagMgr::Update()
{
	Vector2 p;
	switch(state)
	{
		case FlagMgr::MAIN:
			break;
		case FlagMgr::MOVE_NEXT:
			p = (*nowFlag)->GetObj()->GetPos();
			Campus::GetInst()->SetNextPos(GetPoint(p.x, p.y));
			if(Campus::GetInst()->IsMoveEnd())
			{
				p = (*nowFlag)->GetObj()->GetPos();
				Campus::GetInst()->SetPos(GetPoint(p.x, p.y));
				state = State::CHECK;
			}
			break;
		case FlagMgr::CHECK:
			p = (*nowFlag)->GetObj()->GetPos();
			Campus::GetInst()->SetPos(GetPoint(p.x, p.y));
			CheckFlag();
			effectTimer = 20;
			state = State::EFFECT;
			break;
		case FlagMgr::EFFECT:
			if(IsFinishEffect())
			{
				if(CheckNext())
				{
					p = (*nowFlag)->GetObj()->GetPos();
					Campus::GetInst()->SetNextPos(GetPoint(p.x, p.y));
					Campus::GetInst()->TimeReset();
					state = State::MOVE_NEXT;
				}
				else
				{
					state = State::MAIN;
				}
			}

			break;
		default:
			break;
	}

	for(auto it = releaseFlag.begin(); it != releaseFlag.end();)
	{
		(*it)->Update();
		if((*it)->IsEnd())
		{
			SafeDelete(*it);
			it = releaseFlag.erase(it);
		}
		else
			it++;
	}
	for(auto& r : blackFlag)
		r->Update();
	for(auto& r : goldFlag)
		r->Update();
}

void FlagMgr::Render()
{
	for(auto& r : releaseFlag)
		r->Render();
	for(auto& r : goldFlag)
		r->Render();
	for(auto& r : blackFlag)
		r->Render();
	Campus::GetInst()->Draw();
	listBack->Render(1130, 0, 512, 1024, 0, 0, 512, 1024);
	int offsety = 86;
	for(std::pair<const int, int>& r : speedList)
	{
		blockBack->Render(1175, offsety, 100, 72, 0, 0, 100, 80);
		DataOwner::GetInst()->number->RenderCC(r.first, 1175 + 52, offsety + 20, 0.15f, 1.0f, false);
		DataOwner::GetInst()->number->RenderCC(r.second, 1175 + 40, offsety + 50, 0.2f, 1.0f, false);
		offsety += 72;
	}
	goldBack->Render(1160, 530, 64, 64, 0, 0, 64, 64);
	DataOwner::GetInst()->number->RenderCC(haveGold, 1250, 560, 0.3f, 1.0f, false);
}

void FlagMgr::SetSpeedList(const std::map<int, int>& list)
{
	speedList = list;
}

void FlagMgr::AppendFlag(TimeObj* obj, bool next)
{
	if(obj->state == TimeObj::SUCCESS)return;
	int speed = 0;
	for(auto it = blackFlag.begin(); it != blackFlag.end();)
	{
		if((*it)->GetObj() == obj)
		{
			speed = (*it)->GetNum();
			(*it)->SetState(FlagGmk::State::FADE_OUT);
			releaseFlag.push_back(*it);
			it = blackFlag.erase(it);
		}
		else
			it++;
	}
	if(next)
		speed = NextSpeed(speed);
	else
		speed = BeforeSpeed(speed);
	if(speed == 0)return;

	FlagGmk* flg = new FlagGmk(obj);
	flg->Init(DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::FRAG_BLACK),
			  DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::FRAG_BLACK),
			  DataOwner::GetInst()->number);
	flg->SetNum(speed);
	flg->SetType(FlagGmk::BLACK);
	blackFlag.push_back(flg);
}

void FlagMgr::AppendGoldFlag(TimeObj* obj)
{
	if(obj->state != TimeObj::SUCCESS || obj->GetSuccessCnt() > 0)return;
	if(obj->GetGold_Effect())
	{
		obj->SetGold_Effect(false);
		//解除
		for(auto it = goldFlag.begin(); it != goldFlag.end(); it++)
		{
			if((*it)->GetObj() == obj)
			{
				(*it)->SetState(FlagGmk::State::FADE_OUT);
				releaseFlag.push_back(*it);
				it = goldFlag.erase(it);
				break;
			}
		}
		obj->SetState(TimeObj::SUCCESS);
	}
	else
	{
		//金フラ装備
		if(haveGold < 1)return;
		obj->SetGold_Effect(true);

		FlagGmk* flg = new FlagGmk(obj);
		flg->Init(DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::FRAG_GOLD),
				  DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::FRAG_GOLD),
				  DataOwner::GetInst()->number);

		int speed = obj->GetOrginSpeed();

		flg->SetNum(speed);
		flg->SetType(FlagGmk::GOLD);
		goldFlag.push_back(flg);

	}
}

void FlagMgr::StartCheck()
{
	state = State::MOVE_NEXT;
	// チェック対象の演出をスタート
	for(auto& r : blackFlag)
	{
		r->GetObj()->SetState(TimeObj::State::CHECK);
		r->SetChecked(false);
	}
	// リストの先頭を確保
	nowFlag = blackFlag.begin();
	if(nowFlag == blackFlag.end())
	{
		state = State::MAIN;
		return;
	}
	Vector2 p = (*nowFlag)->GetObj()->GetPos();
	Campus::GetInst()->SetNextPos(GetPoint(p.x, p.y));
	Campus::GetInst()->TimeReset();
}

bool FlagMgr::CheckNext()
{
	for(auto it = blackFlag.begin(); it != blackFlag.end(); it++)
	{
		if((*it)->IsChecked())continue;

		nowFlag = it;
		return true;
	}
	return false;
}

void FlagMgr::CheckFlag()
{
	(*nowFlag)->GetObj()->SetState(TimeObj::State::MOVE);
	(*nowFlag)->SetChecked(true);
	if(abs((*nowFlag)->GetObj()->GetOrginSpeed() - (*nowFlag)->GetNum()) < 0.1f)
	{
		speedList[(*nowFlag)->GetNum()]--;
		(*nowFlag)->SetType(FlagGmk::TYPE::FREE);

		(*nowFlag)->GetObj()->SetGold_Effect(false);
		TimeObj::AddChain();
		(*nowFlag)->GetObj()->SetMine_SChain(TimeObj::GetChain());
		(*nowFlag)->GetObj()->SetState(TimeObj::State::SUCCESS);
		(*nowFlag)->SetState(FlagGmk::State::FADE_OUT);
		releaseFlag.push_back(*nowFlag);

		//(*nowFlag)->SetBack(DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::FRAG_GOLD),
		//					DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::FRAG_GOLD));
		//(*nowFlag)->SetType(FlagGmk::TYPE::GOLD);
		//(*nowFlag)->SetState(FlagGmk::State::STAY);
		//(*nowFlag)->GetObj()->SetState(TimeObj::State::STOP);
		//goldFlag.push_back(*nowFlag);
		//if(speedList[(*nowFlag)->GetNum()] <= 0)// 使い切ったフラグを解放
		//{
		//	for(auto it = blackFlag.begin(); it != blackFlag.end();)
		//	{
		//		if((*it) == (*nowFlag))
		//		{
		//			it++;
		//			continue;
		//		}
		//		if((*it)->GetNum() == (*nowFlag)->GetNum())
		//		{
		//			(*it)->GetObj()->SetState(TimeObj::MOVE);
		//			(*it)->SetState(FlagGmk::State::FADE_OUT);
		//			releaseFlag.push_back(*it);
		//			it = blackFlag.erase(it);
		//		}
		//		else
		//			it++;
		//	}
		//}
		nowFlag = blackFlag.erase(nowFlag);
	}
	else
	{
		(*nowFlag)->SetState(FlagGmk::State::FADE_OUT);
		releaseFlag.push_back(*nowFlag);
		nowFlag = blackFlag.erase(nowFlag);
	}
}

bool FlagMgr::IsFinishEffect()
{
	return effectTimer-- <= 0;
}

POINT FlagMgr::GetNowObjPos()
{
	POINT ret;
	ret.x = (*nowFlag)->GetObj()->GetPos().x;
	ret.y = (*nowFlag)->GetObj()->GetPos().y;
	return ret;
}

bool FlagMgr::IsCheckEnd()
{
	return state == MAIN;
}

bool FlagMgr::IsClear()
{
	for(auto& r : speedList)
	{
		if(r.second > 0)
			return false;
	}
	return true;
}

void FlagMgr::SetHaveGoldFlag(int num)
{
	haveGold = num;
}


inline int FlagMgr::NextSpeed(int nowSpeed)
{
	for(auto r = speedList.begin(); r != speedList.end(); r++)
	{
		if(r->first == nowSpeed || nowSpeed == 0)
		{
			while(r != speedList.end() && (r->first == nowSpeed || r->second == 0))
				r++;
			return r != speedList.end() ? r->first : 0;
		}
	}
	return 0;
}

inline int FlagMgr::BeforeSpeed(int nowSpeed)
{
	for(auto r = speedList.rbegin(); r != speedList.rend(); r++)
	{
		if(r->first == nowSpeed || nowSpeed == 0)
		{
			while(r != speedList.rend() && (r->first == nowSpeed || r->second == 0))
			{
				r++;
			}
			return r != speedList.rend() ? r->first : 0;
		}
	}
	return 0;
}

inline void FlagMgr::ReleaseImage()
{
	SafeDelete(listBack);
	SafeDelete(blockBack);
	SafeDelete(goldBack);
}

inline void FlagMgr::ReleaseList()
{
	for(auto& r : blackFlag)
		SafeDelete(r);
	for(auto& r : goldFlag)
		SafeDelete(r);
	for(auto& r : releaseFlag)
		SafeDelete(r);

	blackFlag.clear();
	goldFlag.clear();
	releaseFlag.clear();
	speedList.clear();

	nowFlag = blackFlag.end();
}