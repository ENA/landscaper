#ifndef _GAMESETTING_H_
#define _GAMESETTING_H_

#include "systemstate.h"
#include "input.h"

#include "gamemain.h"
#include "gamenetwork.h"
#include "basesetting.h"

namespace space
{
	//�ڑ��ݒ�
	class GameSetting : public SystemState
	{
	protected:

	public:
		//�N���C�A���g�Ȃ�true,�T�[�o�[�Ȃ�false
		bool isClient;
		//�����_���V�[�h
		unsigned int randomseed;
		//�����̃v���C���[�i���o�[
		unsigned int my_number;
		std::string my_name;

		//==========
		//���͊֘A
		//==========
		const Input *input;
		bool nexttrig_disabled_flg;
		unsigned short pushtime_left;
		unsigned short pushtime_middle;
		unsigned short pushtime_right;
		bool pushtrig_left;
		bool pushtrig_middle;
		bool pushtrig_right;
		//key=�{�^���ԍ� , value=�����ꂽ�t���[����
		std::unordered_map<unsigned short,unsigned short> pushtime_key;
		std::unordered_set<unsigned short> pushtrig_key;
		//�O�̃}�E�X�ʒu
		Matrix21<int> bmousepos;
		bool mousemove;

		//�l��Ⴄ���
		std::string return_string;
		bool afterreturn;

		//�l�b�g���[�N�֘A
		TCPsocket server_sock;
		std::vector<TCPsocket> client_sock;
		SDLNet_SocketSet sock_set;
		IPaddress ip;
		std::vector<std::string> names;

		//���̑����
		unsigned int player_num;
		std::string hostname;
		unsigned short port;

		//��ʑJ��
		int state;
		int selectitem;

	public:
		GameSetting(ProgramSystemMain *_sys);
		virtual ~GameSetting();

		void draw();
		void update();
		void after_all();

		void setnextstate(int);
	};
	
}

#endif
