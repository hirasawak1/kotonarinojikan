#ifndef _CHOICESMGR_H_
#define _CHOICESMGR_H_

#include <vector>

class TimeObject;// ���v��낤�����̊��N���X

class ChoicesMgr
{
	//------------ field ---------------
private:
	std::vector<TimeObject*> choicesList;

	//------------ method ----------------
public:
	ChoicesMgr();
	~ChoicesMgr();
	void Init();
	void Update();
	void Render();
};

#endif