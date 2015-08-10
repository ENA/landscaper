#include "title.h"

namespace space{

Title::Title(ProgramSystemMain *_sys) : SystemState(_sys,TITLE)
{
	pushtime_left = 1;
	pushtime_middle = 0;
	pushtime_right = 0;
	pushtime_key.clear();
	pushtime_key[SDLK_RETURN] = 1;
	input = &systemmain->input[0];
}

Title::~Title(void)
{
}

void Title::draw(){
	char _str[256];
	

	//�w�i�̔w�i
	SDL_FillRect(systemmain->screen,NULL,0x003366);
	//�w�i
	{
		SDL_Rect yr = {
			systemmain->screen->w/2,systemmain->screen->h/2,
			systemmain->screen->w  ,systemmain->screen->h};
		SDLFILESRFS::inst().get("image/title.gif").Draw(systemmain->screen,&yr,NULL,systemmain->screen->h);
	}	
	//���S�\��
	{
		SDL_Rect yr = {
			systemmain->screen->w/2,systemmain->screen->h/2,
			systemmain->screen->w  ,systemmain->screen->h};
		SDLFILESRFS::inst().get("image/title_logo.gif").Draw(systemmain->screen,&yr,NULL,systemmain->screen->h);
	}

	//�_�łŎw������
	if(counter%52 < 40)
	{
		sprintf(_str,"PRESS ENTER OR CLICK TO GAME START.");
		systemmain->bmpfont.draw(
			systemmain->screen,
			systemmain->screen->w/2-(strlen(_str)*11)/2,
			systemmain->screen->h/2-100,
			_str,
			systemmain->screen->h);
	}
}

void Title::update(){

	//bgm���Đ�����ĂȂ�������Đ��B
	if(!systemmain->sound_bgm.isPlaying()){
		systemmain->sound_bgm.load("sound/waittime.wav");
		systemmain->sound_bgm.fadein(480);
	}

	//===========
	//���͂̂���
	//==========
	{
		Input ctrinp = *input;
		//mouse
		if( ctrinp.mouse.getleft().isPush() ) pushtime_left++;
		else pushtime_left = 0;
		if( ctrinp.mouse.getmiddle().isPush() ) pushtime_middle++;
		else pushtime_middle = 0;
		if( ctrinp.mouse.getright().isPush() ) pushtime_right++;
		else pushtime_right = 0;
		//key
		const Keyboard::KeyMap &km = ctrinp.keyboard.getkeys();
		std::unordered_map<unsigned short,unsigned short> &now = pushtime_key;
		//�؂�
		std::unordered_map<unsigned short,unsigned short>::iterator nowit = now.begin();
		for(;nowit!=now.end();nowit++)
		{
			if( km.find(nowit->first) == km.end() ){
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

	//====================
	//�Ƃ��ɂȂ����ǂ܂��B
	//====================
	//Enter�܂��̓N���b�N��
	if(	(pushtime_key.count(SDLK_RETURN) && pushtime_key[SDLK_RETURN]==1) ||
		(pushtime_left == 1) ){
		//���ցB
		isNeed = false;
		systemmain->setGameSetting();
		systemmain->sound_enter.play();
		return;
	}

	//�J�E���^����蒙�܂�����ʂ̉�ʂɈڍs�����肷��B
	if(counter > 600){
		isNeed = false;
		systemmain->setGameDemo();
		systemmain->setFadeIn(30);
	}
	else if(counter > 570){
		systemmain->setFadeOut(30);
	}

	//�J�E���^
	counter++;
}

}//end of namespace space

