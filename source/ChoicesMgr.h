#ifndef _CHOICESMGR_H_
#define _CHOICESMGR_H_

#include <vector>

class SuperHoge;// ���v��낤�����̊��N���X

class ChoicesMgr
{
	//------------ field ---------------
private:
	std::vector<SuperHoge*> choicesList;

	//------------ method ----------------
public:
	ChoicesMgr();
	~ChoicesMgr();
	void Update();
	void Render();
};

#endif