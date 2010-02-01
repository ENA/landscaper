#ifndef _GAMENETWORK_H_
#define _GAMENETWORK_H_

#include "systemstate.h"
#include "network.h"

#include "gamemain.h"
#include "input.h"

namespace space{
	//�ڑ��ݒ�
	class GameNetwork : public SystemState
	{
	protected:

	public:
		//�N���C�A���g�Ȃ�1,�T�[�o�[�Ȃ�0
		bool isClient;

		//==========
		//���͊֘A
		//==========
		std::vector<Input*> inputs;
		//����������
		struct Pushtime{
			unsigned short pushtime_left;
		        unsigned short pull_left;
			unsigned short pushtime_middle;
		        unsigned short pull_middle;
			unsigned short pushtime_right;
		        unsigned short pull_right;
			//key=�{�^���ԍ� , value=�����ꂽ�t���[����
			std::map<unsigned short,unsigned short> pushtime_key;
			//�����ꂽ�L�[
			std::list<unsigned short> pull_key;
			//�}�E�X�̈ʒu���ς����
			Matrix21<int> bpos;
			bool mouseIsMove;
			
			void clear(){
				pushtime_left = 0;
				pull_left = 0;
				pushtime_middle = 0;
				pull_middle = 0;
				pushtime_right = 0;
				pull_right = 0;
				pushtime_key.clear();
				pull_key.clear();
				bpos=Matrix21<int>(2000,2000);
				mouseIsMove = false;
			}
			Pushtime(){clear();}
		};
		std::vector<Pushtime> pushtimes;
		//�����̔ԍ�
		unsigned int my_number;

		//===================
		//�l�b�g���[�N�֘A
		//===================
		TCPsocket server_sock;
		std::vector<TCPsocket> client_sock;
		SDLNet_SocketSet sock_set;
		
		//========
		//���̂�
		//========
		//�I���t���O���J�E���^
		unsigned int FinishBegin_cnt;

	public:
		GameNetwork(
			ProgramSystemMain *_sys,
			std::vector<Input*> &_in, unsigned int _mynum,
			bool _iscl, std::vector<TCPsocket> &_cl, TCPsocket &_sv);
		virtual ~GameNetwork();

		void draw();
		void update();
		void after_all();
		
		//���͎��Ԃ̂���
		void setpushtime(unsigned short num);
		
		//�R�}���h�󂯎��
		bool recvCommandC(unsigned int i,unsigned short pnum,unsigned int size);
		bool sendCommandC(unsigned int i,unsigned short pnum);
		bool genCommandC(RawData &data,unsigned short pnum);
		bool decompCommandC(RawData &rdata,unsigned short pnum);

		//�ڑ��̏I��
		void ending();

	};
}

#endif
