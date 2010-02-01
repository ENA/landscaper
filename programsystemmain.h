#ifndef _PROGRAMSYSTEMMAIN_H_
#define _PROGRAMSYSTEMMAIN_H_

#include "input.h"
#include "sound.h"

namespace space
{
	//�S�V�X�e�����C��
	class SystemState;
	class GameMain;
	class ProgramSystemMain : public DrawAndUpdate
	{
	public:
		//�o�[�W����
		unsigned int version;

		//���
		std::list<SystemState*> nowstates;
		//���͂̎���
		Input input[16];
		//��ʂ̃T�[�t�F�C�X
		SDL_Surface* screen;
		//FPS�ݒ�p
		FPScontrol fpscontrol;
		//�r�b�g�}�b�v�t�H���g
		BMPFONT bmpfont;
		BMPFONT bmpfontred;
		//��
		SDLMIXERCHUNK sound_move;//�ړ�
		SDLMIXERCHUNK sound_enter;//����
		SDLMIXERCHUNK sound_cancel;//�L�����Z��
		SDLMIXERCHUNK sound_setting;//�Z�b�e�B���O
		SDLMIXERCHUNK sound_bgm;//bgm�ݒ�̂悤�Ȃ���
		SDLMIXERCHUNK sound_warning;//�x����

	public:

		ProgramSystemMain();
		~ProgramSystemMain();

		void setTitle();
		void setGameSetting();
		void setBaseSetting();
		void setFieldSetting();
		void setGameMain();
		void setResult(GameMain *gm);
		void setFadeOut(unsigned int _count);
		void setFadeIn(unsigned int _count);
		void setGameDemo();
		void setTextInput(SystemState* ret, std::string exp, std::string* strret);

		void draw();
		void update();
		void after_all();
		void DisplayReload();

		int  initialize();
		void mainloop();
		void finalize();

		void ending();
		void taskclear();

	};

}

#endif
