#include "textinput.h"

namespace space
{

	//�ڑ��ݒ�
	TextInput::TextInput(ProgramSystemMain *_sys, SystemState* ret, std::string exp, std::string* strret) : SystemState(_sys,TEXTINPUT)
	{
		//��������
		pushtime_left = 1;
		pushtime_middle = 0;
		pushtime_right = 0;
		pushtime_key.clear();
		pushtime_key[SDLK_RETURN] = 1;

		//���͐ݒ�
		input = &_sys->input[0];
		
		//�Ԃ����
		state_return = ret;
		//����
		explanation = exp;
		//�Ԃ�������
		string_return = strret;


		//�\�����镶����
		{
			dispchar.clear();
			sc1 = 'z'-'a'+1;
			sc2 = '.'-','+1 +sc1;
			sc3 = '9'-'0'+1 +sc2;
			sc4 = ' '-' '+1 +sc3;

			for(int i=0;i<sc3;i++){
				char c;	
				if     (i>=sc2) c=i+'0'-sc2;
				else if(i>=sc1) c=i+','-sc1;
				else            c=i+'a';
				dispchar.push_back(c);
			}
		}

		//�f�t�H���g������
		//*string_return = "test";

		//SPACE
		sprect.x=240; sprect.y=200;
		sprect.w=5*systemmain->bmpfont.getW(); sprect.h=systemmain->bmpfont.getH();
		//BACK
		bsrect.x=sprect.x+sprect.w+20; bsrect.y=sprect.y;
		bsrect.w=4*systemmain->bmpfont.getW(); bsrect.h=systemmain->bmpfont.getH();
		//ENTER
		retrect.x=bsrect.x+bsrect.w+20; retrect.y=bsrect.y;
		retrect.w=5*systemmain->bmpfont.getW(); retrect.h=systemmain->bmpfont.getH();
		
		//�^�񒆂悹
		sprect.x = (systemmain->screen->w-((retrect.x+retrect.w)-sprect.x))/2;
		bsrect.x=sprect.x+sprect.w+20;
		retrect.x=bsrect.x+bsrect.w+20;

		//�w�i
		winpos.w=260;winpos.h=160;
		winpos.x=(systemmain->screen->w-winpos.w)/2;winpos.y=192;

	}

	//draw
	void TextInput::draw()
	{

		//�z�X�g�A�N���C�A���g�̕\��
		//char _str[256];

		//�w�i�̔w�i
		SDL_FillRect(systemmain->screen,NULL,0x003366);
		//�w�i
		{
			//�摜�w�i�͂�����
			//SDL_Rect yr = {
			//	systemmain->screen->w/2,systemmain->screen->h/2,
			//	systemmain->screen->w  ,systemmain->screen->h};
			//SDLFILESRFS::inst().get("image/title.gif").Draw(systemmain->screen,&yr,NULL,systemmain->screen->h);

			//�߂��ʂ̂�Ȃ炱����
			state_return->draw();

			//�Â�����Ȃ炱����
			SDL_Rect r = {0,0,systemmain->screen->w,systemmain->screen->h};
			Sint16 dx[4] = {0,r.w,r.w,0};
			Sint16 dy[4] = {0,0,r.h,r.h};
			filledPolygonRGBA(systemmain->screen,dx,dy,4,0,0,0,128);

			//�~�j�w�i�͂�����
			Sint16 x[4] = {winpos.x,winpos.x+winpos.w,winpos.x+winpos.w,winpos.x};
			Sint16 y[4] = {winpos.y,winpos.y,winpos.y+winpos.h,winpos.y+winpos.h};
			for(int i=0;i<4;i++){y[i]=systemmain->screen->h-y[i];}
			filledPolygonRGBA(systemmain->screen,x,y,4,16,16,64,255);
			rectangleRGBA(systemmain->screen,winpos.x,systemmain->screen->h-winpos.y,winpos.x+winpos.w,systemmain->screen->h-winpos.y-winpos.h,32,32,255,255);
		}

		//�^�C�g��
		{
			SDL_Rect r;
			r.y = 140;
			r.w = systemmain->bmpfont.getW() * explanation.length();
			r.x = (systemmain->screen->w-r.w)/2;
			r.h = 15;
			
			Sint16 x[4] = {r.x,r.x+r.w,r.x+r.w,r.x};
			Sint16 y[4] = {r.y,r.y,r.y+r.h,r.y+r.h};
			
			//�w�i
			//filledPolygonRGBA(systemmain->screen,x,y,4,192,192,255,(unsigned char)(63.0+96.0+96.0*sin((double)(counter*1.75)*M_PI/180.0)));
			
			//����
			SDL_Rect _rect= {r.x,480-r.y-r.h,0,0};
	
			//�����\��
			systemmain->bmpfont.draw(systemmain->screen,_rect.x,_rect.y,explanation.c_str(),systemmain->screen->h);
		}

		//�ł����񂾕����\��
		{
			SDL_Rect r;
			r.y = 293;
			r.w =  systemmain->bmpfont.getW();
			r.x = (systemmain->screen->w - r.w*string_return->length())/2;
			r.h = 15;
			
			Sint16 x[4] = {r.x+r.w*string_return->length(),r.x+r.w+r.w*string_return->length(),r.x+r.w+r.w*string_return->length(),r.x+r.w*string_return->length()};
			Sint16 y[4] = {systemmain->screen->h-r.y,systemmain->screen->h-r.y,systemmain->screen->h-(r.y+r.h),systemmain->screen->h-(r.y+r.h)};
			//�w�i
			filledPolygonRGBA(systemmain->screen,x,y,4,0,196,(unsigned char)(127.0+64.0+64.0*sin((double)(counter*3.0)*M_PI/180.0)),196);
			//����
			systemmain->bmpfont.draw(systemmain->screen,r.x,r.y,*string_return,systemmain->screen->h);
		}

		//�I�𕶎��\��
		{
			int bw = systemmain->bmpfont.getW();
			int bh = systemmain->bmpfont.getH();
			int w =   bw+8;
			int h =  (bh+4)*(-1);
			int ofx=200,ofy=260;
			Matrix21<int> np=input->mouse.getpos();

			//���ʂ̕���
			for(unsigned int i=0;i<dispchar.size();i++){
				int nowx = ofx+w*(i%13); int nowy = ofy+h*(i/13);
				//�o�b�N
				if(np.x>=nowx && np.x<=nowx+bw && np.y>=nowy && np.y<=nowy+bh){
					Sint16 x[4] = {nowx-1,nowx+bw,nowx+bw,nowx-1};
					Sint16 y[4] = {systemmain->screen->h-(nowy),systemmain->screen->h-(nowy),systemmain->screen->h-(nowy+bh),systemmain->screen->h-(nowy+bh)};			
					filledPolygonRGBA(systemmain->screen,x,y,4,0,196,(unsigned char)(127.0+64.0+64.0*sin((double)(counter*3.0)*M_PI/180.0)),196);
				}
				//����
				systemmain->bmpfont.draw(systemmain->screen,nowx,nowy,dispchar[i],systemmain->screen->h);
			}

			//�X�y�[�X�L�[
			{
				//�w�i
				if(sprect.Col_Dot(np)){
					Sint16 x[4] = {sprect.x,sprect.x+sprect.w,sprect.x+sprect.w,sprect.x};
					Sint16 y[4] = {sprect.y,sprect.y,sprect.y+sprect.h,sprect.y+sprect.h};
					for(int i=0;i<4;i++){y[i]=systemmain->screen->h-y[i];}
					filledPolygonRGBA(systemmain->screen,x,y,4,0,196,(unsigned char)(127.0+64.0+64.0*sin((double)(counter*3.0)*M_PI/180.0)),196);
				}
				//����
				systemmain->bmpfont.draw(systemmain->screen,sprect.x,sprect.y,"SPACE",systemmain->screen->h);
			}
			//�o�b�N�X�y�[�X
			{
				if(bsrect.Col_Dot(np)){
					Sint16 x[4] = {bsrect.x,bsrect.x+bsrect.w,bsrect.x+bsrect.w,bsrect.x};
					Sint16 y[4] = {bsrect.y,bsrect.y,bsrect.y+bsrect.h,bsrect.y+bsrect.h};
					for(int i=0;i<4;i++){y[i]=systemmain->screen->h-y[i];}
					filledPolygonRGBA(systemmain->screen,x,y,4,0,196,(unsigned char)(127.0+64.0+64.0*sin((double)(counter*3.0)*M_PI/180.0)),196);
				}
				//����
				systemmain->bmpfont.draw(systemmain->screen,bsrect.x,bsrect.y,"BACK",systemmain->screen->h);
			}
			//����
			{
				if(retrect.Col_Dot(np)){
					Sint16 x[4] = {retrect.x,retrect.x+retrect.w,retrect.x+retrect.w,retrect.x};
					Sint16 y[4] = {retrect.y,retrect.y,retrect.y+retrect.h,retrect.y+retrect.h};
					for(int i=0;i<4;i++){y[i]=systemmain->screen->h-y[i];}
					filledPolygonRGBA(systemmain->screen,x,y,4,0,196,(unsigned char)(127.0+64.0+64.0*sin((double)(counter*3.0)*M_PI/180.0)),196);
				}
				//����
				systemmain->bmpfont.draw(systemmain->screen,retrect.x,retrect.y,"ENTER",systemmain->screen->h);
			}

		}

		//�J�E���^
		counter++;
	}

	//update
	void TextInput::update()
	{
		//===========
		//���͂̂���
		//==========
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
		std::map<unsigned short,unsigned short> &now = pushtime_key;
		//�؂�
		std::map<unsigned short,unsigned short>::iterator nowit = now.begin();
		for(;nowit!=now.end();nowit++)
		{
			if( km.find(nowit->first) == km.end() ){
			  std::map<unsigned short,unsigned short>::iterator tit = nowit;
			  nowit--;now.erase(tit);
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
		//
			
		//===================
		//���͂̂���
		//===================
		//�㉺���E
		//if( pushtime_key.count(SDLK_DOWN)  && pushtime_key[SDLK_DOWN]==1 ){
		//	systemmain->sound_move.play();
		//}
		//if( pushtime_key.count(SDLK_UP)  && pushtime_key[SDLK_UP]==1 ){
		//	systemmain->sound_move.play();
		//}
		//if( pushtime_key.count(SDLK_LEFT)  && pushtime_key[SDLK_LEFT]==1 ){
		//	systemmain->sound_setting.play();
		//}
		//if( pushtime_key.count(SDLK_RIGHT) && pushtime_key[SDLK_RIGHT]==1){
		//	systemmain->sound_setting.play();
		//}
	
		//�L�[�{�[�h���當������
		{

			//���ʂ̃L�[
			{
				//sec1
				for(int i=0;i<sc1;i++){
					if(pushtime_key.count(SDLK_a+i) && pushtime_key[SDLK_a+i]==1){
						*string_return += 'a'+i;
					}
				}

				//sec2
				for(int i=0;i<sc2-sc1;i++){
					if(pushtime_key.count(SDLK_COMMA+i) && pushtime_key[SDLK_COMMA+i]==1){
						*string_return += ','+i;
					}
				}
				
				//sec3
				for(int i=0;i<sc3-sc2;i++){
					if(pushtime_key.count(SDLK_0+i) && pushtime_key[SDLK_0+i]==1){
						*string_return += '0'+i;
					}
				}
			}

			//�X�y�[�X�ƌ���ƃo�b�N�X�y�[�X
			{
				//SPACE
				if(pushtime_key.count(SDLK_SPACE) && pushtime_key[SDLK_SPACE]==1){
						*string_return += ' ';
				}
				//ENTER
				if(pushtime_key.count(SDLK_RETURN) && pushtime_key[SDLK_RETURN]==1 )
				{
					isNeed = false;
					systemmain->nowstates.push_back(state_return);
					systemmain->sound_enter.play();
				}
				//BACKSPACE
				if(pushtime_key.count(SDLK_BACKSPACE) && (pushtime_key[SDLK_BACKSPACE]==1 || pushtime_key[SDLK_BACKSPACE]>15)){
					*string_return = string_return->substr(0,string_return->size()-1);
				}
			}
		}

		//�N���b�N���A�I�𕶎��ł�����
		if(input->mouse.getleft().isPush() && pushtime_left==1){
			int bw = systemmain->bmpfont.getW();
			int bh = systemmain->bmpfont.getH();
			int w =   bw+8;
			int h =  (bh+4)*(-1);
			int ofx=200,ofy=260;
			Matrix21<int> np=input->mouse.getleft().getPushpos();

			//���ʂ̃L�[
			for(unsigned int i=0;i<dispchar.size();i++){
				int nowx = ofx+w*(i%13); int nowy = ofy+h*(i/13);
				//����
				if(np.x>=nowx && np.x<=nowx+bw && np.y>=nowy && np.y<=nowy+bh){
					*string_return += dispchar[i];
				}
			}
			//�X�y�[�X�L�[
			if(sprect.Col_Dot(np)) *string_return += ' ';
			//�o�b�N�X�y�[�X
			if(bsrect.Col_Dot(np)) *string_return = string_return->substr(0,string_return->size()-1);
			//����
			if(retrect.Col_Dot(np)){
				isNeed = false;
				systemmain->nowstates.push_back(state_return);
				systemmain->sound_enter.play();
			}
		}

		//��ʊO�ŉE�N���b�N������A�L�����Z���I���B
		if(input->mouse.getright().isPush() && pushtime_right==1){
			Matrix21<int> np=input->mouse.getright().getPushpos();
			if(winpos.Col_Dot(np)==false){
				string_return->resize(0);
				isNeed = false;
				systemmain->nowstates.push_back(state_return);
				systemmain->sound_cancel.play();
			}
		}

	}

	//�S���I��������Ƃɉ������������̏���
	void TextInput::after_all()
	{
	}
	
}
