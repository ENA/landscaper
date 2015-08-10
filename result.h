#ifndef _RESULT_H_
#define _RESULT_H_

#include "systemstate.h"
#include "input.h"

#include "gamesetting.h"
#include "gamemain.h"

namespace space{

class Result : public SystemState
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
		std::unordered_map<unsigned short,unsigned short> pushtime_key;

		//����{�^���̂��ƁB
		bool isReady;

		//========
		//����
		//========
		std::string name;
		int rank;
		int spendmoney;
		int killed;
		int bekilled;
		int typeofBase;
	};
	std::vector<Player> player;
	unsigned int player_ctrl;
	std::vector<int> rank;
		
	WinRect readybutton_rect;

public:
	Result(ProgramSystemMain* sys,GameMain* gm);
	virtual ~Result(void);

	virtual void draw();
	virtual void update();
};

}// end of namespace space

#endif
