#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "common.h"


//���ߐF
//RGB��G255,���̑�0

namespace space
{

	//SDL�̃T�[�t�F�C�X������Ƀ��b�v���ăN���X�ɂ��������B
	class SDLSURFACE
	{
	public:
		SDL_Surface *surface;
		bool mine;

	public:

		void Free(){
			if(mine)SDL_FreeSurface(surface);
			mine=false;
		}
		void LoadSurface(SDL_Surface *image)
		{
		
			//�A���t�@������
			if( image->flags&SDL_SRCALPHA ){
				SDL_SetAlpha(image, 0, 0);
			}

			//�J���[�L�[�������ꍇ�ɂ͉��΂ŃJ���[�L�[�����B
			if( ( image->flags & SDL_SRCCOLORKEY ) == 0){
				//SDL_SetColorKey(
				//	image,SDL_SRCCOLORKEY | SDL_RLEACCEL | SDL_HWACCEL,
				//	SDL_MapRGB(image->format, 0, 255, 0));
			}
			
			//FREE
			Free();
			//surface
			surface = SDL_DisplayFormat(image);	
			//�������
			mine = true;

		}
		void LoadIMG(const std::string &fname)
		{
			//�摜�ǂ݂���
			SDL_Surface *image = IMG_Load(fname.c_str());
			if(image==NULL){
				char s[256];
				sprintf(s,"Can't open image file !!!(%s,%s)",fname.c_str(),SDL_GetError());
				throw s;
			}
			LoadSurface(image);
			SDL_FreeSurface(image);	
		}



		void LoadText(const std::string &text)
		{
			//�t�H���g�ݒ�
			static TTF_Font *font = TTF_OpenFont("tt0246m_.ttf",14);
			static SDL_Color white = {255,255,255,255};
			if(font==NULL){
				char s[256];
				sprintf(s,"Can't open font file !!!(%s)",TTF_GetError()); 
				throw s;
			}
			//�T�[�t�F�C�X�𐻑����܂�
			SDL_Surface *fontsf = TTF_RenderText_Solid(font,text.c_str(),white);
			if(fontsf==NULL){
				char s[256];
				sprintf(s,"Can't load font surface !!!(%s)",TTF_GetError()); 
				throw s;
			}
			LoadSurface( fontsf );
			SDL_FreeSurface( fontsf );
		}

		void Draw(SDL_Surface *dst,SDL_Rect *dstrect,
			SDL_Rect *srcrect, unsigned int w_height) const
		{
			SDL_Rect drect = *dstrect;
			if(w_height!=0) drect.y = w_height - drect.y;
			if(srcrect==NULL)
			{
				drect.x -= surface->w/2;
				drect.y -= surface->h/2;
			}
			else
			{
				drect.x -= srcrect->w/2;
				drect.y -= srcrect->h/2;
			}
			SDL_BlitSurface(surface,srcrect,dst,&drect);
		}

		void DrawWH(SDL_Surface *dst,SDL_Rect *dstrect,
			SDL_Rect *srcrect, unsigned int w_height) const
		{
			SDL_Rect drect = *dstrect;
			//��؂Ȃ���
			if(w_height!=0) drect.y = w_height - drect.y;

			//�g��k��
			double scalex,scaley;
			if(srcrect==NULL)
			{
				scalex = (double)drect.w/(double)surface->w;
				scaley = (double)drect.h/(double)surface->h;
			}
			else
			{
				scalex = (double)drect.w/(double)srcrect->w;
				scaley = (double)drect.h/(double)srcrect->h;
			}
			SDL_Surface *zoom = zoomSurface(surface,scalex,scaley,0);
			zoom->flags = surface->flags;
			zoom->format->colorkey = surface->format->colorkey;
			SDL_Surface *zooms = SDL_DisplayFormat(zoom);
			SDL_FreeSurface(zoom);

			//���G�ؔ���
			if(srcrect!=NULL)
			{
				SDL_Rect _rect = *srcrect;
				_rect.x = (Sint16)(_rect.x * scalex);
				_rect.y = (Sint16)(_rect.y * scaley);
				_rect.w = (Uint16)(_rect.w * scalex);
				_rect.h = (Uint16)(_rect.h * scaley);
				drect.x -= _rect.w/2;
				drect.y -= _rect.h/2;
				SDL_BlitSurface(zooms,&_rect,dst,&drect);
			}
			else{
				drect.x -= zooms->w/2;
				drect.y -= zooms->h/2;
				SDL_BlitSurface(zooms,NULL,dst,&drect);
			}
			SDL_FreeSurface(zooms);			
		}

		SDLSURFACE(){mine=false;}
		SDLSURFACE(const std::string &fname)
		{
			mine = false;
			LoadIMG(fname);
		}
		virtual ~SDLSURFACE(){Free();}
	};

	//�摜�t�@�C����ǂݍ����GL�̃e�N�X�`���ɂ��܂�
	//Factory�I��class�ł��B
	class GLTEXTURE
	{
	protected:
		unsigned int texid;

		unsigned int width,height;
		unsigned int n_width,n_height;

		void SetToGLTexture(unsigned int* _im,unsigned short _fl=GL_NEAREST)
		{
			//�o�C���h����B
			BindToGLTexture();

			//�e�N�X�`���C���[�W�̐ݒ�
			glPixelStorei(GL_UNPACK_ALIGNMENT,4);

			//�e�N�X�`���o�^
			glTexImage2D
				(GL_TEXTURE_2D,0,GL_RGBA,n_width,n_height,
				0,GL_RGBA,GL_UNSIGNED_BYTE,_im);

			//�e�N�X�`���̌J��Ԃ����@
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

			//�g��k���̕⊮���@
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,_fl);
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,_fl);

		}
	public:		

		unsigned int GetW() const { return width;  }
		unsigned int GetH() const { return height; }
		unsigned int GetNW()const { return n_width;  }
		unsigned int GetNH()const { return n_height; }
		unsigned int GetID()const { return texid; }

		GLTEXTURE(){texid = 0;}
		GLTEXTURE(const GLTEXTURE &_t)
		{
			texid = _t.GetID();
			width = GetW(); height = GetH();
			n_width = GetNW(); n_height = GetNH();
		}
		GLTEXTURE(std::string _fn)
		{
			texid = 0;
			LoadFile(_fn);
		}
		virtual ~GLTEXTURE()
		{
			glDeleteTextures(1,&texid);
		}


		void BindToGLTexture()
		{
			if(texid==0) glGenTextures(1,&texid);
			glBindTexture(GL_TEXTURE_2D,texid);
		}

		unsigned int upper2base(unsigned int src) const
		{
			if(src<=1)return 1;
			unsigned int n=1; src-=1;
			do{n*=2;src/=2;} while(src!=0);
			return n;
		}

		int LoadArray(unsigned int* pixels, unsigned int _w, unsigned int _h)
		{
			width=_w; height=_h;
			
			n_width = upper2base(_w); n_height= upper2base(_h);
			unsigned int *p = new unsigned int [n_width*n_height];

			for(int i=0;i<(int)n_height;i++)
			{
				for(int j=0;j<(int)n_width;j++)
				{
					p[i*n_width+j] = pixels[(i*height/n_height)*width + (j*width/n_width)];
				}
			}
			SetToGLTexture(p);

			delete [] p;
			return 0;
		}
		int LoadSurface(SDL_Surface *surface)
		{
			
			//�`��̈�w��	
			SDL_Rect area;
			area.x=0;  area.y=0;
			area.w=surface->w; area.h=surface->h;

			//�F�͂������RGBA
			SDL_Surface *rgba = ::SDL_CreateRGBSurface(SDL_HWSURFACE,
													   area.w,area.h,32,
													   0x000000ff,0x0000ff00,
													   0x00ff0000,0xff000000);
			//�A���t�@�擾
			if( surface->flags&SDL_SRCALPHA )SDL_SetAlpha(surface, 0, 0);

	
			//�F���ƂƂ��ɂǂ���
			if(!rgba){ SDL_FreeSurface(rgba); throw "Can't Load Surface!!!";}
			SDL_BlitSurface(surface,&area,rgba,&area);

			//�o�^
			LoadArray((unsigned int*)rgba->pixels,area.w,area.h);

			//�J��
			SDL_FreeSurface(rgba);

			return 0;
		}

		int LoadFile(std::string filename)
		{
			//�ǂ݂���
			SDL_Surface *surface = IMG_Load(filename.c_str());
			if(! surface) { throw "Can't IMG_Load Surface!!!"; }

			int ret = LoadSurface(surface);

			SDL_FreeSurface(surface);
			
			return ret;

		}


		void draw(double _w,double _h)
		{
			//�e�N�X�`���L����
			glEnable(GL_TEXTURE_2D);
			//�e�N�X�`�����o�C���h
			BindToGLTexture();

			//���_��ݒ�
			GLfloat w_2=static_cast<GLfloat>(_w/2),h_2=static_cast<GLfloat>(_h/2);
			glBegin(GL_QUADS);
			glTexCoord2d( 0.0, 1.0);
			glVertex3f(-w_2, -h_2 ,0.0);
			glTexCoord2d( 1.0, 1.0);
			glVertex3f( w_2, -h_2 ,0.0);
			glTexCoord2d( 1.0, 0.0);
			glVertex3f( w_2,  h_2 ,0.0);
			glTexCoord2d( 0.0, 0.0);
			glVertex3f(-w_2,  h_2 ,0.0);
			glEnd();

			//�e�N�X�`��������
			glDisable(GL_TEXTURE_2D);
		}
		inline void draw(double _scl){ draw((double)width*_scl,(double)height*_scl); }
		inline void draw_w(double _w){ draw(_w,(double)height/(double)width*_w); }
		inline void draw_h(double _h){ draw((double)width/(double)height*_h,_h); }
		inline void draw(){ draw(width,height); }
	};

	//===============================
	//���e�N�X�`���Ǘ��N���X
	//�t�@�C������ǂݍ��ރe�N�X�`����S�Ĉꌳ�Ǘ�����
	//================================
	class GLFILETEXS
	{
	private:
		GLFILETEXS(const GLFILETEXS&); //��`�֎~

		typedef std::unordered_map<std::string,GLTEXTURE*> STRTEXMAP;
		STRTEXMAP graphicrc;
		GLFILETEXS(){}
		virtual ~GLFILETEXS(){clear();}
	public:
		//�C���X�^���X�擾
		static GLFILETEXS& inst(){
			static GLFILETEXS instance;
			return instance;
		}
		//�摜��Ⴄ
		GLTEXTURE& get(std::string _name)
		{
			STRTEXMAP::iterator it = graphicrc.find(_name);
			if(it==graphicrc.end())
			{
				return *(graphicrc[_name] = new GLTEXTURE(_name));
			}
			return *(it->second);
		}
		//�I�[���f���[�g
		void clear()
		{
			STRTEXMAP::iterator it = graphicrc.begin();
			for(;it!=graphicrc.end();it++) delete it->second;
			graphicrc.clear();
		}
	};

	//===============================
	//�T�[�t�F�C�X�Ǘ��N���X
	//�t�@�C������ǂݍ��ރT�[�t�F�C�X��S�Ĉꌳ�Ǘ�����
	//================================
	class SDLFILESRFS
	{
	private:
		SDLFILESRFS(const SDLFILESRFS&); //��`�֎~

		typedef std::unordered_map<std::string,SDLSURFACE*> STRSRFMAP;
		STRSRFMAP graphicrc;
		SDLFILESRFS(){}
		virtual ~SDLFILESRFS(){clear();}
	public:
		//�C���X�^���X�擾
		static SDLFILESRFS& inst(){
			static SDLFILESRFS instance;
			return instance;
		}
		//�摜��Ⴄ
		const SDLSURFACE& get(std::string _name)
		{
			STRSRFMAP::iterator it = graphicrc.find(_name);
			if(it==graphicrc.end())
			{
				return *(graphicrc[_name] = new SDLSURFACE(_name));
			}
			return *(it->second);
		}
		//�I�[���f���[�g
		void clear()
		{
			STRSRFMAP::iterator it = graphicrc.begin();
			for(;it!=graphicrc.end();it++) delete it->second;
			graphicrc.clear();
		}
	};

	//=================
	//�r�b�g�}�b�v����
	//=================
	class BMPFONT
	{
	private:
		SDLSURFACE srfs;
		char ascii_ofs;
		unsigned int font_width;
		unsigned int font_height;

	public:
		char getASCIIofs() const {return ascii_ofs;}
		unsigned int getW() const {return font_width;}
		unsigned int getH() const {return font_height;}
		void open(std::string fname){
			srfs.LoadIMG(fname);
		}
		void fontsetting(unsigned int fw,unsigned int fh,char ofs)
		{
			font_width=fw; font_height=fh;
			ascii_ofs = ofs;
		}
		void draw(SDL_Surface *dst, short x, short y, std::string str,unsigned int w_height)
		{
			unsigned int num_width = srfs.surface->w / font_width;
			for(unsigned int i=0;i<str.length();i++){
				char pos = ascii_ofs + str[i];
				SDL_Rect src;
				src.x = (pos % num_width)*font_width;
				src.y = (pos / num_width)*font_height;
				src.w=font_width; src.h=font_height;
				SDL_Rect dstr = {x+i*font_width+font_width/2,y+font_height/2,0,0};
				srfs.Draw(dst,&dstr,&src,w_height);
			}
		}
		void draw(SDL_Surface *dst, short x, short y, char c, unsigned int w_height){char str[2]; str[0]=c;str[1]='\0'; draw(dst,x,y,str,w_height);}

		void draw(SDL_Surface *dst, short x, short y, double scale, std::string str,unsigned int w_height)
		{
			unsigned int num_width = srfs.surface->w / font_width;
			for(unsigned int i=0;i<str.length();i++){
				char pos = ascii_ofs + str[i];
				SDL_Rect src;
				src.x = (pos % num_width)*font_width;
				src.y = (pos / num_width)*font_height;
				src.w=font_width; src.h=font_height;
				SDL_Rect dstr = {(Sint16)(x+i*src.w*scale+src.w*scale/2.0),(Sint16)(y+src.h*scale/2.0),(Uint16)((double)src.w*scale),(Uint16)((double)src.h*scale)};
				srfs.DrawWH(dst,&dstr,&src,w_height);
			}
		}

		BMPFONT(){}
		BMPFONT(std::string fname){
			open(fname);
		}
		virtual ~BMPFONT(){}

	};

}

#endif
