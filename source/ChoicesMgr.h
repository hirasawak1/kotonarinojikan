#ifndef _CHOICESMGR_H_
#define _CHOICESMGR_H_

#include <vector>

class SuperHoge;// 時計やろうそくの基底クラス

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