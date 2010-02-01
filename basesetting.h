#ifndef _BASESETTING_H_
#define _BASESETTING_H_

#include "systemstate.h"
#include "input.h"

#include "gamesetting.h"
#include "gamemain.h"

namespace space{

class BaseSetting : public SystemState
{
public:

	//�I���t���O���J�E���^
	unsigned int FinishBegin_cnt;

	struct Player{
		//==========
		//���͊֘A
		//==========
		const Input *input;
		unsigned short pushtime_left;
		unsigned short pushtime_middle;
		unsigned short pushtime_right;
		//key=�{�^���ԍ� , value=�����ꂽ�t���[����
		std::map<unsigned short,unsigned short> pushtime_key;

		//========
		//�ݒ荀��
		//========
		//���O
		std::string name;
		//���_�^�C�v(�}�C�i�X�Ŗ��I��)
		int typeofBase;

	};
	std::vector<Player> player;
	unsigned int player_ctrl;
		
	std::vector< Circle2<double> > basebutton_circle;
	std::vector<const SDLSURFACE*> basebutton_surface;
	std::vector<std::string> basebutton_name;
	
	unsigned int randomseed;

public:
	BaseSetting(ProgramSystemMain *_sys,std::vector<BaseSetting::Player> pl,unsigned int m,unsigned int _rds);
	virtual ~BaseSetting(void);

	virtual void draw();
	virtual void update();
};

}// end of namespace space

#endif
