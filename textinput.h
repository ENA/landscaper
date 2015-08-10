#ifndef _TEXTINPUT_H_
#define _TEXTINPUT_H_

#include "systemstate.h"


namespace space
{
	//�ڑ��ݒ�
	class TextInput : public SystemState
	{
	public:
		//���͂��镶����̐���
		std::string explanation;
		//�߂��̃X�e�[�g
		SystemState *state_return;
		//�߂�������̃A�h���X
		std::string *string_return;

		//
		int sc1,sc2,sc3,sc4;

		//�w�i�̈ʒu
		WinRect winpos;

		//�X�y�[�X�A�o�b�N�X�y�[�X�A����
		WinRect sprect,bsrect,retrect;

		//==========
		//���͊֘A
		//==========
		const Input *input;
		unsigned short pushtime_left;
		unsigned short pushtime_middle;
		unsigned short pushtime_right;
		//key=�{�^���ԍ� , value=�����ꂽ�t���[����
		std::unordered_map<unsigned short,unsigned short> pushtime_key;

		//������
		std::vector<char> dispchar;

	public:
		TextInput(ProgramSystemMain *_sys, SystemState* ret, std::string exp, std::string* strret);
		virtual ~TextInput(){}

		void draw();
		void update();
		void after_all();
	};
	
}


#endif