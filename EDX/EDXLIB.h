#pragma once

///////////////////////////////////////////
//										 //
//				EDX_Input				 //
//										 //
///////////////////////////////////////////
int MouseGet(int butontype);
enum{
	EDX_CLICK_L, EDX_CLICK_R
};

class EDX_Input{
	//マウス入力状態の保存
	static int Click[2];
public:
	static BOOL GetState();
	static unsigned int GetClickModule(int BUTTON_TYPE);
};