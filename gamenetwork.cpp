#include "gamenetwork.h"

namespace space{

	//�ڑ��ݒ�
	GameNetwork::GameNetwork(
		ProgramSystemMain *_sys,
		std::vector<Input*> &_in, unsigned int _mynum,
		bool _iscl, std::vector<TCPsocket> &_cl, TCPsocket &_sv)
		: SystemState(_sys,GAMENETWORK)
	{
		//���͐ݒ�
		inputs = _in;
		my_number = _mynum;
		for(unsigned int i=0;i<inputs.size();i++){
			inputs[i]->clear();
			pushtimes.push_back(Pushtime());
		}
		//�\�P�b�g�ݒ�
		isClient = _iscl;
		server_sock = _sv;
		client_sock = _cl;
		//�\�P�b�g�Z�b�g�����
		sock_set = SDLNet_AllocSocketSet(client_sock.size()+1);
		SDLNet_TCP_AddSocket(sock_set,server_sock);
		for(unsigned int i=0;i<client_sock.size();i++){
			SDLNet_TCP_AddSocket(sock_set,client_sock[i]);
		}

		//�J�E���^
		FinishBegin_cnt = UINT_MAX;
	}

	//�ڑ��I������
	void GameNetwork::ending(){
		//�\�P�b�g�Z�b�g�J��
		if(sock_set!=NULL){
			SDLNet_FreeSocketSet(sock_set);
			sock_set=NULL;
		}
		//�N���C�A���g�\�P�b�g�S���J��
		for(unsigned int i=0;i<client_sock.size();i++){
			SDLNet_TCP_Close(client_sock[i]);
		}
		client_sock.clear();
		//�T�[�o�[�\�P�b�g�J��
		if(server_sock!=NULL){
			SDLNet_TCP_Close(server_sock);
			server_sock = NULL;
		}
	}

	GameNetwork::~GameNetwork(){
		//�����ŉ����c���Ă���ꍇ�A�������N�����Ă��܂����Ƃ������Ƃł���B
		if(client_sock.size()>0)
		{
			//���x��������B
			ending();
		}
	}

	//draw�Ȃ�
	void GameNetwork::draw()
	{
		SDL_Rect _rect= {systemmain->screen->w-80,systemmain->screen->h-20,0,0};
		//draw_str("It Works! (Network)",systemmain->screen,&_rect,systemmain->screen->h);
	}

	//�R�}���h���M
	bool GameNetwork::genCommandC(RawData &data, unsigned short pnum){
		//�L�[�{�[�h(���܂�0�ŁB)
		unsigned short keys = 0;
		//data.write(&keys,2);
		
		//�L�[�{�[�h�̉����ꂽ������ꂽ�肵���������B
		for(unsigned int j=0;j<keys;j++){
		  //�܂������ɂ͂Ȃɂ������������
		}
		//�}�E�X�̕�
		unsigned char mouses = 0;
		if(pushtimes[pnum].pushtime_left==1  || pushtimes[pnum].pull_left  !=0)mouses++;
		if(pushtimes[pnum].pushtime_middle==1|| pushtimes[pnum].pull_middle!=0)mouses++;
		if(pushtimes[pnum].pushtime_right==1 || pushtimes[pnum].pull_right !=0)mouses++;
		if(pushtimes[pnum].mouseIsMove) mouses+=128;
		data.write(&mouses,1);

		//�͂��߂Ƀ}�E�X�̍��W
		if(pushtimes[pnum].mouseIsMove){
			Matrix21<short> _mousepos(inputs[pnum]->mouse.getpos().x, inputs[pnum]->mouse.getpos().y);
			int zippos = _mousepos.x*4096 + _mousepos.y;
			data.write(&zippos,3);
		}

		//�}�E�X�ŕω��̋N���������𑗂�
		unsigned char mousestate;
		//left
		if(pushtimes[pnum].pushtime_left==1){
		  mousestate = 0 + 1*4;
		  data.write(&mousestate,1);
		  Matrix21<short> _pos(inputs[pnum]->mouse.getleft().getPushpos().x,inputs[pnum]->mouse.getleft().getPushpos().y);
		  data.write(&_pos.x,2);
		  data.write(&_pos.y,2);
		}
		else if(pushtimes[pnum].pull_left !=0){
		  mousestate = 0 + 0*4;
		  data.write(&mousestate,1);
		  Matrix21<short> _pos(inputs[pnum]->mouse.getleft().getPullpos().x,inputs[pnum]->mouse.getleft().getPullpos().y);
		  data.write(&_pos.x,2);
		  data.write(&_pos.y,2);
		}
		//middle
		if(pushtimes[pnum].pushtime_middle==1){
		  mousestate = 1 + 1*4;
		  data.write(&mousestate,1);
		  Matrix21<short> _pos(inputs[pnum]->mouse.getmiddle().getPushpos().x,inputs[pnum]->mouse.getmiddle().getPushpos().y);
		  data.write(&_pos.x,2);
		  data.write(&_pos.y,2);
		}
		else if(pushtimes[pnum].pull_middle !=0){
		  mousestate = 1 + 0*4;
		  data.write(&mousestate,1);
		  Matrix21<short> _pos(inputs[pnum]->mouse.getmiddle().getPullpos().x,inputs[pnum]->mouse.getmiddle().getPullpos().y);
		  data.write(&_pos.x,2);
		  data.write(&_pos.y,2);
		}
		//left
		if(pushtimes[pnum].pushtime_right==1){
		  mousestate = 2 + 1*4;
		  data.write(&mousestate,1);
		  Matrix21<short> _pos(inputs[pnum]->mouse.getright().getPushpos().x,inputs[pnum]->mouse.getright().getPushpos().y);
		  data.write(&_pos.x,2);
		  data.write(&_pos.y,2);
		}
		else if(pushtimes[pnum].pull_right !=0){
		  mousestate = 2 + 0*4;
		  data.write(&mousestate,1);
		  Matrix21<short> _pos(inputs[pnum]->mouse.getright().getPullpos().x,inputs[pnum]->mouse.getright().getPullpos().y);
		  data.write(&_pos.x,2);
		  data.write(&_pos.y,2);
		}


		return true;
	}

	//�R�}���h���M
	bool GameNetwork::sendCommandC(unsigned int i,unsigned short pnum){
		
		RawData rdata;
		if(!genCommandC(rdata,pnum))return false;
		unsigned int com = 'c' + (256)*rdata.size() + (256*256*256)*(pnum);
		
		RawData senddata;
		senddata.write(&com,4);
		senddata.write(&rdata[0],rdata.size());
		int sendsize = SDLNet_TCP_Send(client_sock[i],&senddata[0],senddata.size());
		if(sendsize != senddata.size()) return false;

		return true;
	}

	//�R�}���h�󂯎��
	bool GameNetwork::decompCommandC(RawData &rdata,unsigned short pnum){
		//�L�[�{�[�h
		unsigned short keys=0;
		//rdata.read((void*)&keys,2);

		//�L�[�{�[�h�̉����ꂽ������ꂽ�肵���������B
		for(unsigned int j=0;j<keys;j++){
			//�L�[�{�[�h�����󂯎��
			unsigned short keystate;
			rdata.read((void*)&keystate,2);
			unsigned short keynum = keystate % 512;
			unsigned short isPush = keystate / 512;
			//�����ꂽ
			if(isPush!=0){
				inputs[pnum]->keyboard.pushkey(keynum);
			}
			//�����ꂽ
			else{
				inputs[pnum]->keyboard.pullkey(keynum);
			}
		}
		//�}�E�X�̕�
		unsigned char mouses;
		rdata.read((void*)&mouses,1);

		//�ŏ��Ƀ}�E�X�̍��W
		if(mouses/128==1){
			int zippos = 0;
			rdata.read((void*)&zippos,3);
			Matrix21<int> mousepos(zippos/4096,zippos%4096);
			inputs[pnum]->mouse.setpos(mousepos);
			mouses %= 128;
		}

		//�}�E�X�̉����ꂽ������ꂽ�肵���������B
		for(unsigned int j=0;j<mouses;j++){
			//�}�E�X�����󂯎��
			unsigned char mousestate;
			rdata.read((void*)&mousestate,1);
			unsigned char keynum = mousestate % 4;
			unsigned char isPush = mousestate / 4;
			Matrix21<short> _pos;
			rdata.read((void*)&_pos.x,2);
			rdata.read((void*)&_pos.y,2);
			Matrix21<int> pos;
			pos.x=_pos.x; pos.y=_pos.y;
			//�����ꂽ
			if(isPush!=0){
				switch(keynum)
				{
				case 0:
					inputs[pnum]->mouse.setleft().push(pos);
					break;
				case 1:
					inputs[pnum]->mouse.setmiddle().push(pos);
					break;
				case 2:
					inputs[pnum]->mouse.setright().push(pos);
					break;
				}
			}
			//�����ꂽ
			else{
				switch(keynum)
				{
				case 0:
					inputs[pnum]->mouse.setleft().pull(pos);
					break;
				case 1:
					inputs[pnum]->mouse.setmiddle().pull(pos);
					break;
				case 2:
					inputs[pnum]->mouse.setright().pull(pos);
					break;
				}
			}
		}

		return true;
	}

	//�R�}���h�󂯎��
	bool GameNetwork::recvCommandC(unsigned int i,unsigned short pnum,unsigned int size){
		RawData rdata;
		rdata.assign(size);
		int recvsize = SDLNet_TCP_Recv(client_sock[i],(void*)&rdata[0],size);
		if(recvsize<0 || recvsize!=size) return false;
		if(!decompCommandC(rdata,pnum))return false;
		return true;
	}

    //���͎���
    void GameNetwork::setpushtime(unsigned short num){
		//���͂̂���
		Input ctrinp = *inputs[num];
		Pushtime *ctrpush = &pushtimes[num];
		//�����ꂽ�{�^�����X�g���N���A
		ctrpush->pull_key.clear();
		
		//mouse
		//left
		ctrpush->pull_left=0;
		if( ctrinp.mouse.getleft().isPush() ) ctrpush->pushtime_left++;
		else{
		  if(ctrpush->pushtime_left!=0)ctrpush->pull_left = 1;
		  ctrpush->pushtime_left = 0;
		}
		//middle
		ctrpush->pull_middle=0;
		if( ctrinp.mouse.getmiddle().isPush() ) ctrpush->pushtime_middle++;
		else{
		    if(ctrpush->pushtime_middle!=0)ctrpush->pull_middle = 1;
		    ctrpush->pushtime_middle = 0;
		}
		//right
		ctrpush->pull_right=0;
		if( ctrinp.mouse.getright().isPush() ) ctrpush->pushtime_right++;
		else{
		  if(ctrpush->pushtime_right!=0)ctrpush->pull_right = 1;
		  ctrpush->pushtime_right = 0;
		}
		//�}�E�X�̈ʒu���ς������
		if( ctrpush->bpos != ctrinp.mouse.getpos() ){
			ctrpush->bpos = ctrinp.mouse.getpos();
			ctrpush->mouseIsMove = true;
		}
		else ctrpush->mouseIsMove = false;
		
		//key
		const Keyboard::KeyMap &km = ctrinp.keyboard.getkeys();
		std::unordered_map<unsigned short,unsigned short> &now = ctrpush->pushtime_key;
		//�؂�
		std::unordered_map<unsigned short,unsigned short>::iterator nowit = now.begin();
		for(;nowit!=now.end();nowit++)
		{
			if( km.find(nowit->first) == km.end() ){
			  ctrpush->pull_key.push_back(nowit->first);
			  nowit = now.erase(nowit);
			  if(nowit==now.end())break;
			}
			else { nowit->second++; }
		}
		//�ǉ�����
		Keyboard::KeyMap::const_iterator kmit = km.begin();
		for(;kmit!=km.end();kmit++)
		{
			if( now.find(kmit->first) == now.end() ) now[kmit->first]=1;
		}	 
    }

	//update
	void GameNetwork::update()
	{
		//�I���Ƃ��Ȃ�J�E���g��������B
		if(counter > FinishBegin_cnt){
			if(counter-FinishBegin_cnt>=60){
				ProgramSystemMain *psm=systemmain;
				std::list<SystemState*>::iterator sl=psm->nowstates.begin();
				for(;sl!=psm->nowstates.end();sl++)(*sl)->isNeed=false;
				psm->setGameSetting();
				psm->setFadeIn(60);
			}
		}
		else{

			//====================
			//���[�J���̓��͂ɂ���
			//====================
			setpushtime(my_number);
	        
			//===========
			//�l�b�g���[�N���͂̂���
			//==========

			//�ʐM�G���[���o�p
			try{

			if(isClient){
				//����
				if(!sendCommandC(0,my_number))throw "Send Error.";

				//�N�̕����������ė����̂��t���O
				std::bitset<16> whoreturns(0x00);
				//�����̓Z�b�g���Ă���
				whoreturns.set(my_number);

				
				//�S���A���Ă���܂ő҂�
				while(whoreturns.count() < inputs.size()){

					//�\�P�b�g�̉������������̐�����Ԃ�
					if(SDLNet_CheckSockets(sock_set,7000) > 0)
					{
						//�ǂ����������痈��������������
						if(SDLNet_SocketReady(client_sock[0]))
						{
							//�R�}���h�Ɋւ���
							char command;
							unsigned int pnum;
							unsigned int datasize;

							//�󂯎��B�~�X�����狭���I���B
							unsigned int block;
							if( SDLNet_TCP_Recv(client_sock[0],(void*)(&block),4) < 4 ){
								throw "Command Recv Error.";
							}
							//���𒊏o����B
							else{
								command = (char)block;
								datasize = (block/256)%(256*256);
								pnum = block / (256*256*256);
							}
		
							//�e�R�}���h�̏���
							//�����񂪗���
							if(command == 'c'){
								if(!recvCommandC(0,pnum,datasize))throw "Data 'c' Recv Error";
								setpushtime(pnum);
								//�������ė�����
								whoreturns.set(pnum);
							}
							//�����I���R�}���h(�Ȃ��Ȃ�������痎����)
							//else if(command == 'k'){
							else{
								throw "Unknown Command.";
							}
						}
					}
					//�^�C���A�E�g��炩������I������B
					else{
						throw "Timeout.";
					}
				}
				

			}
			else{

				//�N�̕��������ė����̂��t���O
				std::bitset<16> whoreturns(0x00);
				//�����̓Z�b�g���Ă���
				whoreturns.set(my_number);
				
				//��������܂ő҂�
				while(whoreturns.count() < inputs.size()){

					//�\�P�b�g�̉������������̐�����Ԃ�			
					if(SDLNet_CheckSockets(sock_set,7000) > 0)
					{
						//�S�N���C�A���g�Ɋւ���
						for(unsigned int i=0;i<client_sock.size();i++){
							//�ǂ����������痈��������������
							if(SDLNet_SocketReady(client_sock[i]))
							{
								//�R�}���h�Ɋւ���
								char command;
								unsigned int pnum;
								unsigned int datasize;

								//�󂯎��B�~�X�����狭���I���B
								unsigned int block;
								if( SDLNet_TCP_Recv(client_sock[i],(void*)(&block),4) < 4 ){
									throw "Command Recv Error.";
								}
								//���𒊏o����B
								else{
									command = (char)block;
									datasize = (block/256)%(256*256);
									pnum = block / (256*256*256);
								}

								//�e�R�}���h�̏���
								//�����񂪗���
								if(command == 'c'){
									//�R�}���h���󂯎��
									if(!recvCommandC(i,pnum,datasize)) throw "Data 'c' Recv Error.";
									//�������Ԑݒ�
									setpushtime(pnum);
									//�������ė�����
									whoreturns.set(pnum);
								}
								//�����I���R�}���h(��̂�������痎����)
								//else if(command == 'k'){
								else{
									throw "Unknown Command.";}
							}
						}
					}
					//�^�C���A�E�g��炩������I������B
					else{throw "Timeout.";}

				}
				

				//����
				for(unsigned int i=0;i<client_sock.size();i++){
					RawData rdata;
					for(unsigned int j=0;j<client_sock.size();j++){
						//����Ɠ����z�͑���Ȃ�
						if(i!=j){
							if(!sendCommandC(i,j+1)) throw "Send Error.";
						}
						//�����z�ɂȂ����炻��Ƃ��ɃT�[�o�[�̓z�𑗂�
						else{
							if(!sendCommandC(i,0)) throw "Send Error.";
						}
					}
				}
				
			}

			}
			catch(char* msg){
				//�l�b�g���[�N�G���[�L�^
				std::ofstream srm("networkerror.log",std::ios::out|std::ios::app);
				srm << "(" << my_number << ")" << msg << "::" << time(NULL) << std::endl;
				//��̏���
				FinishBegin_cnt = counter;
				systemmain->setFadeOut(60);
			}
		}
		counter++;
		return;
	}

	//�S���I��������Ƃɉ������������̏���
	void GameNetwork::after_all()
	{
		return;
	}

}
