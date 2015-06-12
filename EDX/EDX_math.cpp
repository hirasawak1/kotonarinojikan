#include "../IEX/iextreme.h"
#include "EDXLIB.h"

//********************************************
//�@�\�T�v�FFloat�^�̌덷���l������r����
//�����Ffloat a , b ��r�Ώ�
//�߂�l�F�����ꍇtrue
//********************************************
bool EDX::FloatCmp(float a, float b){
	if (a > b - FLT_EPSILON || a<b + FLT_EPSILON)
	{
		return true;
	}
	return false;
}

//********************************************
//�@�\�T�v�F�x�N�g�����擾����
//�����Ffloat vx,vy,vz �x�N�g���̊e����
//�߂�l�F�x�N�g��
//********************************************
EDX::Vector EDX::GetV(float vx, float vy, float vz){
	Vector V = { vx, vy, vz };
	return V;
}
EDX::Vector EDX::GetV(D3DXVECTOR3 D3Vec){
	Vector V = { D3Vec.x, D3Vec.y, D3Vec.z };
	return V;
}

//********************************************
//�@�\�T�v�F�_�Ɠ_�̋������擾����
//�@�@�@�@�@�܂��̓x�N�g���̑傫�����擾����
//�����FVector Vec1,Vec2 �_
//�߂�l�F�_�Ɠ_�̒�������
//********************************************
float EDX::Length(Vector Vec1, Vector Vec2){

	Vector sub = Vec2 - Vec1;

	return sqrtf(sub.x*sub.x + sub.y*sub.y + sub.z*sub.z);
}

float EDX::Length(Vector Vec){

	Vector sub = Vec;

	return sqrtf(sub.x*sub.x + sub.y*sub.y + sub.z*sub.z);
}

//********************************************
//�@�\�T�v�F�x�N�g���̐��K�����s��
//�����FD3DVECTOR Vector ���K�����s���x�N�g��
//�߂�l�F���K�������x�N�g��
//********************************************
EDX::Vector EDX::Normalize(Vector Vec){
	float r = sqrtf(Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z);
	Vec.x /= r;
	Vec.y /= r;
	Vec.z /= r;
	return Vec;
}

D3DXVECTOR3 EDX::Normalize(D3DXVECTOR3 Vec){
	float r = sqrtf(Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z);
	Vec.x /= r;
	Vec.y /= r;
	Vec.z /= r;
	return Vec;
}

//********************************************
//�@�\�T�v�F�x�N�g�����m�̊O�ς��擾����
//�����FVector v1 �x�N�g��1
//Vector v2 �x�N�g��2
//�߂�l�F�O�ϒl
//********************************************
EDX::Vector EDX::CrossVec(Vector v1, Vector v2){
	Vector Out;
	Out.x = v1.y*v2.z - v1.z*v2.y;
	Out.y = v1.z*v2.x - v1.x*v2.z;
	Out.z = v1.x*v2.y - v1.y*v2.x;
	return Out;
}

//********************************************
//�@�\�T�v�F�x�N�g�����m�̓��ς��擾����
//�����FVector v1 �x�N�g��1
//Vector v2 �x�N�g��2
//�߂�l�F���ϒl
//********************************************
float EDX::DotVec(Vector v1, Vector v2){
	float Out = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	return Out;
}

//********************************************
//�@�\�T�v�F�N�H�[�^�j�I�����m�̏�Z���s��
//�����FQuaternion Q1 �N�H�[�^�j�I��1
//Quaternion Q2 �N�H�[�^�j�I��2
//�߂�l�F��Z�̌���
//********************************************
EDX::Quaternion EDX::MulQuater(Quaternion Q1, Quaternion Q2){
	Quaternion Out;
	Out.w = Q1.w*Q2.w - DotVec(Q1.xyz, Q2.xyz);
	Out.xyz = Q2.xyz*Q1.w + Q1.xyz*Q2.w + CrossVec(Q1.xyz, Q2.xyz);
	
	return Out;
}

//********************************************
//�@�\�T�v�F�N�H�[�^�j�I����p�����C�ӎ���]���s��
//�����FD3DVECTOR Pos ��]���������W
//D3DVECTOR Axis �C�ӎ��x�N�g��
//float RotateAngle ��]����p�x(���W�A���p)
//�߂�l�F��]��̍��W
//********************************************
EDX::Vector EDX::FreeAxisRotate(Vector Pos, Vector Axis, float RotateAngle){
	Quaternion Out;
	Out.xyz = Pos;
	Out.w = 0;

	if (RotateAngle > EDX_PI_F)RotateAngle -= EDX_PI_F*2.0f;
	if (RotateAngle < EDX_PI_F)RotateAngle += EDX_PI_F*2.0f;

	Axis = Normalize(Axis);

	Quaternion Q;
	Q.w = cosf(RotateAngle*0.5f);
	Q.x = Axis.x*sinf(RotateAngle * 0.5f);
	Q.y = Axis.y*sinf(RotateAngle * 0.5f);
	Q.z = Axis.z*sinf(RotateAngle * 0.5f);

	Quaternion R;
	R.w = cosf(RotateAngle*0.5f);
	R.x = -Axis.x*sinf(RotateAngle * 0.5f);
	R.y = -Axis.y*sinf(RotateAngle * 0.5f);
	R.z = -Axis.z*sinf(RotateAngle * 0.5f);

	Out = MulQuater(MulQuater(R, Out), Q);
	return Out.xyz;
}

EDX::Vector EDX::FreeAxisRotate(Vector Pos, Vector Offset, Vector Axis, float RotateAngle){
	Quaternion Out;
	Out.xyz = Pos - Offset;
	Out.w = 0;

	if (RotateAngle > EDX_PI_F)RotateAngle -= EDX_PI_F*2.0f;
	if (RotateAngle < EDX_PI_F)RotateAngle += EDX_PI_F*2.0f;

	Axis = Normalize(Axis);

	Quaternion Q;
	Q.w = cosf(RotateAngle*0.5f);
	Q.x = Axis.x*sinf(RotateAngle * 0.5f);
	Q.y = Axis.y*sinf(RotateAngle * 0.5f);
	Q.z = Axis.z*sinf(RotateAngle * 0.5f);

	Quaternion R;
	R.w = cosf(RotateAngle*0.5f);
	R.x = -Axis.x*sinf(RotateAngle * 0.5f);
	R.y = -Axis.y*sinf(RotateAngle * 0.5f);
	R.z = -Axis.z*sinf(RotateAngle * 0.5f);

	Out = MulQuater(MulQuater(R, Out), Q);
	return Out.xyz + Offset;
}


//********************************************
//�@�\�T�v�FD3DX�w���p�[�֐���p�����C�ӎ���]���s��
//�����FD3DVECTOR Pos ��]���������W
//D3DVECTOR Axis �C�ӎ��x�N�g��
//float RotateAngle ��]����p�x(���W�A���p)
//�߂�l�F��]��̍��W
//********************************************
D3DXMATRIX EDX::FreeAxisRotateDX(Vector Pos, Vector Axis, float RotateAngle){
	D3DXMATRIX mat;
	D3DXVECTOR3 buff = { Axis.x, Axis.y, Axis.z};
	D3DXMatrixRotationAxis(&mat, &buff, RotateAngle);
	mat = mat;
	return mat;
}


//********************************************
//�@�\�T�v�F�x�N�g���ƍs��̊|���Z���s��
//�����FVector v �x�N�g��
//D3DXMATRIX mat �s��
//�߂�l�F�v�Z��̃x�N�g��
//********************************************
EDX::Vector EDX::MulVM(Vector v, D3DXMATRIX mat) {
	Vector Out;
	Out.x = mat._11*v.x + mat._12*v.y + mat._13*v.z + mat._41*1.0f;
	Out.y = mat._21*v.x + mat._22*v.y + mat._23*v.z + mat._42*1.0f;
	Out.z = mat._31*v.x + mat._32*v.y + mat._33*v.z + mat._43*1.0f;
	return Out;
}
EDX::Vector EDX::MulVM(D3DXVECTOR3 v, D3DXMATRIX mat) {
	Vector Out;
	Out.x = mat._11*v.x + mat._12*v.y + mat._13*v.z + mat._14*1.0f;
	Out.y = mat._21*v.x + mat._22*v.y + mat._23*v.z + mat._24*1.0f;
	Out.z = mat._31*v.x + mat._32*v.y + mat._33*v.z + mat._34*1.0f;
	return Out;
}
//
////********************************************
////�@�\�T�v�F�ϊ��s���p����3D��ԏ�̍��W��
////			�X�N���[����łǂ̓_�ɑ��݂��邩���ׂ�
////�����FVector WPos ���[���h��ԏ�ł̍��W
////�߂�l�F�X�N���[����ł̍��W
////********************************************
//Vector GetScreenPos(Vector WPos){
//	D3DXMATRIX View, Persp;
//	D3DXVECTOR4 Pos = { WPos.x, WPos.y, WPos.z, 1 };
//
//	float Width = (float)edxSystem::GetRect().right;
//	float Height = (float)edxSystem::GetRect().bottom;
//
//	D3DXVec4Transform(&Pos, &Pos, &EDX_Camera::GetVPMat());
//
//
//	Vector Out = { 0, 0, 0 };
//	Out.x = 0.5f*Pos.x / Pos.w + 0.5f;
//	Out.y = 0.5f*Pos.y / Pos.w + 0.5f;
//	Out.y = 1.0f - Out.y;
//	Out.x *= Width;
//	Out.y *= Height;
//	Out.z = Pos.z / Pos.w;
//	if (Out.z >= 1.0f){
//		Out.x *= (float)edxSystem::GetRect().right;
//		Out.y *= (float)edxSystem::GetRect().bottom;
//	}
//	return Out;
//}
