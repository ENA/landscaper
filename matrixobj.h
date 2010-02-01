#ifndef _MATRIXOBJ_H_
#define _MATRIXOBJ_H_

#include <math.h>
#include <float.h>

namespace space
{

//����߂ĕ��ʂ�3�s���̍s��Ƃ������Ȃ��͂Ȃ�����
//�_�̍\���̂Ƃ����Ȃ����Ȃ�
template <class T=double>
struct Matrix31
{
	T x;
	T y;
	T z;

	Matrix31();
	Matrix31(const Matrix31<T> &_t);
	Matrix31(T _x,T _y,T _z);

	//�����Z
	inline Matrix31<T> operator+(const Matrix31<T> &_t);
	//�����Z
	inline Matrix31<T> operator-(const Matrix31<T> &_t);
	//���
	inline Matrix31<T>& operator=(const Matrix31<T> &_t);
	//������
	inline bool operator==(const Matrix31<T> &_t) const;

	//�萔�Ƃ̑����Z
	inline Matrix31<T> operator+(T _c);
	//�萔�Ƃ̈����Z
	inline Matrix31<T> operator-(T _c);
	//�萔�Ƃ̊|���Z
	inline Matrix31<T> operator*(T _c);
	//�萔�Ƃ̊���Z
	inline Matrix31<T> operator/(T _c);

	//��Βl
	inline T Abs2();
	inline T Abs();

	//����
	inline T DotMul(const Matrix31<T> &_t);
	//�O��
	inline Matrix31<T> CrsMul(const Matrix31<T> &_t);

};


//�O���錾
template <class T> struct Matrix21;
template <class T> struct Line2;
template <class T> struct Triangle2;
template <class T> struct Circle2;

//����߂ĕ��ʂ�2�s���̍s��Ƃ������Ȃ��͂Ȃ�����
//�_�̍\���̂Ƃ����Ȃ����Ȃ�
template <class T=double>
struct Matrix21
{
	T x;
	T y;

	Matrix21();
	Matrix21(const Matrix21<T> &_t);
	Matrix21(T _x,T _y);

	//�����Z
	inline Matrix21<T> operator+(const Matrix21<T> &_t) const;
	//�����Z
	inline Matrix21<T> operator-(const Matrix21<T> &_t) const;

	//���
	inline Matrix21<T>& operator=(const Matrix21<T> &_t);
	//�����đ��
	inline Matrix21<T>& operator+=(const Matrix21<T> &_t);
	//�����đ��
	inline Matrix21<T>& operator-=(const Matrix21<T> &_t);
	
	//������
	inline bool operator==(const Matrix21<T> &_t) const;
	//�������Ȃ�
	inline bool operator!=(const Matrix21<T> &_t) const;

	//�萔�Ƃ̑����Z
	inline Matrix21<T> operator+(T _c) const;
	//�萔�Ƃ̈����Z
	inline Matrix21<T> operator-(T _c) const;
	//�萔�Ƃ̊|���Z
	inline Matrix21<T> operator*(T _c) const;
	//�萔�Ƃ̊���Z
	inline Matrix21<T> operator/(T _c) const;
	//�����đ��
	inline Matrix21<T>& operator+=(T _c) ;
	//�����đ��
	inline Matrix21<T>& operator-=(T _c) ;
	//�|���đ��
	inline Matrix21<T>& operator*=(T _c) ;
	//�����đ��
	inline Matrix21<T>& operator/=(T _c) ;

	//��Βl
	inline T Abs2() const;
	inline T Abs() const;

	//2�����̓���
	inline T DotMul(const Matrix21<T> &_t) const;
	//2�����̊O��
	inline T CrsMul(const Matrix21<T> &_t) const;

	//�L���X�g
	template <class S> inline operator Matrix21<S>() const{ return Matrix21<S>((S)x,(S)y); }

	//���Ȃ���������B
	inline bool Col_Dot (const Matrix21 &_mt) const;
	//���̑�
	inline bool Col_Line (const Line2<T> &_mt) const;
	inline bool Col_Tri (const Triangle2<T> &_mt) const;
	inline bool Col_Circle (const Circle2<T> &_mt) const;
};


//==============
//�񎟌��Ȑl�X
//==============
//����
template <class T=double>
struct Line2
{
	//�_2��
	Matrix21<T> p1,p2;

	Line2();
	Line2(const Line2 &_l);
	Line2(const Matrix21<T> &_p1, const Matrix21<T> &_p2);
	Line2(T _x1,T _y1,T _x2,T _y2);

	//���
	inline Line2<T>& operator=(const Line2<T> &_l);
	//������
	inline bool operator==(const Line2<T> &_l) const;

	//�����̂Q��
	inline T length2();
	inline T length();

	//2�����̓����蔻��
	//{p1(x,y)��p2(x,y)}��{_l.p1(x,y)��_l.p2(x,y)}
	//�������true
	inline bool Col_Line (Line2 _l) const;
	//double�ł͖��̂����`�B
	inline bool Col_Dot(const Matrix21<T> &_mt) const;
	//���̑�
	inline bool Col_Tri (const Triangle2<T> &_mt) const;
	inline bool Col_Circle (const Circle2<T> &_mt) const;
	
	
	// ��̐����̌�_�����߂�B�߂�ltrue�Ō�����Ă���.
	inline bool CrossPoint(const Line2<T> &_l, Matrix21<T> &_p) const;
	//����_�̐����ƒ�������n�_�����߂�B���s�n�_������Ȃ�true.
	inline bool OrthoPoint(const Matrix21<T> &_p, Matrix21<T> &_h) const;
	//����_�́A���Ώ̂Ȉʒu�����߂�D���̈ʒu���琂���������đΏې���Ȃ��true.
	inline bool LineSymmetryPoint(const Matrix21<T> &_p, Matrix21<T> &_q) const;

};
	
//�O�p�`
template <class T=double>
struct Triangle2
{
	//�_�R��
	Matrix21<T> p1,p2,p3;

	Triangle2();
	Triangle2(const Triangle2<T> &_t);
	Triangle2(const Line2<T> &_l1, const Line2<T> &_l2, const Line2<T> &_l3);
	Triangle2(const Matrix21<T> &_p1, const Matrix21<T> &_p2, const Matrix21<T> &_p3);
	
	Triangle2(T _x1,T _y1,T _x2,T _y2,T _x3,T _y3);
	
	//������
	inline bool operator==(const Triangle2<T> &_t) const;
	//���
	inline Triangle2<T>& operator=(const Triangle2<T> &_t);

	//�O�p�`�Ɠ_�̔���
	//{0(x,y)��1(x,y)��2(x,y)},�����p(x,y)
	//�������true
	inline bool Col_Dot(const Matrix21<T> &_p) const;
	//�O�p�`�Ɛ��̔���
	inline bool Col_Line(const Line2<T> &_l) const ;
	//�O�p�`�ƎO�p�`�̔���
	inline bool Col_Tri(const Triangle2<T> &_t) const ;
	//���̑�
	inline bool Col_Circle (const Circle2<T> &_mt) const;

};


//�~
template <class T=double>
struct Circle2
{
	Matrix21<T> p;	//���S�_
	T d;	//���a

	Circle2();
	Circle2(const Circle2<T> &_c);
	Circle2(const Matrix21<T> &_p,T &_d);
	Circle2(T _x, T _y, T _d);
	//���
	inline Circle2<T>& operator=(const Circle2<T> &_c);

	//�~�Ɠ_�̓����蔻��
	//d�͉~�̒��a
	//����������true
	inline bool Col_Dot(const Matrix21<T> &_p) const;
	//�~�Ɖ~
	//����������true
	inline bool Col_Circle (const Circle2<T> &_c) const;
	//���Ɖ~�̓����蔻��
	//����͂܂�,�_�ƒ����̋��������߂Ă���𔻒肷�邱�Ƃɓ�����
	//�����̍��W��2�A�~�̒��S���W����A�Ō�ɂ͉~�̒��a�����悤
	//����������true
	inline bool Col_Line (const Line2<T> &_l) const;
	//�O�p�`�Ɖ~�̓����蔻��
	//�������true
	inline bool Col_Tri(const Triangle2<T> &_t) const;

};



//===========================
//����߂ĕ��ʂ�3�s���̍s��
//===========================
template <class T> Matrix31<T>::Matrix31(){}
template <class T> Matrix31<T>::Matrix31(const Matrix31<T> &_t){ x=_t.x; y=_t.y; z=_t.z;}
template <class T> Matrix31<T>::Matrix31(T _x,T _y,T _z){ x=_x;y=_y; z=_z;}

//�����Z
template <class T> inline Matrix31<T> Matrix31<T>::operator+(const Matrix31<T> &_t)
{ return Matrix31<T>(x+_t.x,y+_t.y,z+_t.z); }
//�����Z
template <class T> inline Matrix31<T> Matrix31<T>::operator-(const Matrix31<T> &_t)
{ return Matrix31<T>(x-_t.x,y-_t.y,z-_t.z); }
//���
template <class T> inline Matrix31<T>& Matrix31<T>::operator=(const Matrix31<T> &_t)
{
	x=_t.x; y=_t.y; z=_t.z;
	return *this;
}
//������
template <class T> inline bool Matrix31<T>::operator==(const Matrix31<T> &_t) const
{ return (x==_t.x&&y==_t.y&&z==_t.z);}

//�萔�Ƃ̑����Z
template <class T> inline Matrix31<T> Matrix31<T>::operator+(T _c)
{ return Matrix31(x+_c,y+_c,z+_c); }
//�萔�Ƃ̈����Z
template <class T> inline Matrix31<T> Matrix31<T>::operator-(T _c)
{ return Matrix31(x-_c,y-_c,z-_c); }
//�萔�Ƃ̊|���Z
template <class T> inline Matrix31<T> Matrix31<T>::operator*(T _c)
{ return Matrix31(x*_c,y*_c,z*_c); }
//�萔�Ƃ̊���Z
template <class T> inline Matrix31<T> Matrix31<T>::operator/(T _c)
{ return Matrix31(x/_c,y/_c,z/_c); }

//��Βl
template <class T> inline T Matrix31<T>::Abs2() {return x*x+y*y+z*z;}
template <class T> inline T Matrix31<T>::Abs() {return sqrt(Abs2());}

//����
template <class T> inline T Matrix31<T>::DotMul(const Matrix31<T> &_t)
{ return (_t.x*x + _t.y*y + _t.z*z); }
//�O��
template <class T> inline Matrix31<T> Matrix31<T>::CrsMul(const Matrix31<T> &_t)
{ return Matrix31(y*_t.z-z*_t.y ,z*_t.x-x*_t.z, x*_t.y-y*_t.x); }




//===========================
//����߂ĕ��ʂ�2�s���̍s��
//===========================
template <class T> Matrix21<T>::Matrix21(){}
template <class T> Matrix21<T>::Matrix21(const Matrix21<T> &_t){ x=_t.x; y=_t.y; }
template <class T> Matrix21<T>::Matrix21(T _x,T _y){ x=_x;y=_y; }

//�����Z
template <class T> inline Matrix21<T> Matrix21<T>::operator+(const Matrix21<T> &_t) const
{ return Matrix21<T>(x+_t.x,y+_t.y); }
//�����Z
template <class T> inline Matrix21<T> Matrix21<T>::operator-(const Matrix21<T> &_t) const 
{ return Matrix21<T>(x-_t.x,y-_t.y); }

//���
template <class T> inline Matrix21<T>& Matrix21<T>::operator=(const Matrix21<T> &_t)
{ x=_t.x; y=_t.y; return *this; }
//�����đ��
template <class T> inline Matrix21<T>& Matrix21<T>::operator+=(const Matrix21<T> &_t)
{ x+=_t.x; y+=_t.y; return *this; }
//�����đ��
template <class T> inline Matrix21<T>& Matrix21<T>::operator-=(const Matrix21<T> &_t)
{ x-=_t.x; y-=_t.y; return *this; }

//������
template <class T> inline bool Matrix21<T>::operator==(const Matrix21<T> &_t) const { return (x==_t.x&&y==_t.y); }
//�������Ȃ�
template <class T> inline bool Matrix21<T>::operator!=(const Matrix21<T> &_t) const { return (x!=_t.x||y!=_t.y); }

//�萔�Ƃ̑����Z
template <class T> inline Matrix21<T> Matrix21<T>::operator+(T _c) const { return Matrix21(x+_c,y+_c); }
//�萔�Ƃ̈����Z
template <class T> inline Matrix21<T> Matrix21<T>::operator-(T _c) const { return Matrix21(x-_c,y-_c); }
//�萔�Ƃ̊|���Z
template <class T> inline Matrix21<T> Matrix21<T>::operator*(T _c) const { return Matrix21(x*_c,y*_c); }
//�萔�Ƃ̊���Z
template <class T> inline Matrix21<T> Matrix21<T>::operator/(T _c) const { return Matrix21(x/_c,y/_c); }
//�����đ��
template <class T> inline Matrix21<T>& Matrix21<T>::operator+=(T _c) { x+=_c; y+=_c; return *this; }
//�����đ��
template <class T> inline Matrix21<T>& Matrix21<T>::operator-=(T _c) { x-=_c; y-=_c; return *this; }
//�|���đ��
template <class T> inline Matrix21<T>& Matrix21<T>::operator*=(T _c) { x*=_c; y*=_c; return *this; }
//�����đ��
template <class T> inline Matrix21<T>& Matrix21<T>::operator/=(T _c) { x/=_c; y/=_c; return *this; }

//��Βl
template <class T> inline T Matrix21<T>::Abs2() const {return x*x+y*y;}
template <class T> inline T Matrix21<T>::Abs() const {return sqrt(Abs2());}

//2�����̓���
template <class T> inline T Matrix21<T>::DotMul(const Matrix21<T> &_t) const { return (x*_t.x + y*_t.y); }
//2�����̊O��
template <class T> inline T Matrix21<T>::CrsMul(const Matrix21<T> &_t) const { return (x*_t.y - y*_t.x); }

//���Ȃ���������B
template <class T> inline bool Matrix21<T>::Col_Dot (const Matrix21<T> &_mt) const { return _mt==*this; }
//���̑�
template <class T> inline bool Matrix21<T>::Col_Line (const Line2<T> &_mt) const { return _mt.Col_Dot(*this); }
template <class T> inline bool Matrix21<T>::Col_Tri (const Triangle2<T> &_mt) const { return _mt.Col_Dot(*this); }
template <class T> inline bool Matrix21<T>::Col_Circle (const Circle2<T> &_mt) const { return _mt.Col_Dot(*this); }



//==============
//����
//=============
template <class T> inline Line2<T>::Line2(){}
template <class T> inline Line2<T>::Line2(const Line2 &_l) {p1=_l.p1; p2=_l.p2;}
template <class T> inline Line2<T>::Line2(const Matrix21<T> &_p1, const Matrix21<T> &_p2){ p1=_p1; p2=_p2; }
template <class T> inline Line2<T>::Line2(T _x1,T _y1,T _x2,T _y2)
{
	p1=Matrix21<T>(_x1,_y1);
	p2=Matrix21<T>(_x2,_y2);
}

//���
template <class T> inline Line2<T>& Line2<T>::operator=(const Line2<T> &_l)
{
	p1=_l.p1; p2=_l.p2;
	return *this;
}
//������
template <class T> inline bool Line2<T>::operator==(const Line2<T> &_l) const { return (p1==_l.p1&&p2==_l.p2);}

//�����̂Q��
template <class T> inline T Line2<T>::length2(){ return (p1-p2).Abs2(); }
template <class T> inline T Line2<T>::length() { return sqrt( length2() ); }

//2�����̓����蔻��
//{p1(x,y)��p2(x,y)}��{_l.p1(x,y)��_l.p2(x,y)}
//�������true
template <class T> inline bool Line2<T>::Col_Line (Line2<T> _l) const {
  double mulres[2];
  mulres[0] = (p1-p2).CrsMul(p1-_l.p1) * (p1-p2).CrsMul(p1-_l.p2);
  mulres[1] = (_l.p1-_l.p2).CrsMul(_l.p1-p1) * (_l.p1-_l.p2).CrsMul(_l.p1-p2);

  if ( mulres[0] < 0.0 && mulres[1] < 0.0 ) return true;
  return false;
}

//double�ł͖��̂����`�B
template <class T> inline bool Line2<T>::Col_Dot(const Matrix21<T> &_mt) const
{
	return (_mt.x-p1.x/p2.x-p1.x==_mt.y-p1.y/p2.y-p1.y);
}

//���̑�
template <class T> inline bool Line2<T>::Col_Tri (const Triangle2<T> &_mt) const { return _mt.Col_Line(*this); }
template <class T> inline bool Line2<T>::Col_Circle (const Circle2<T> &_mt) const { return _mt.Col_Line(*this); }

// ��̐����̌�_�����߂�B�߂�ltrue�Ō�����Ă���.
template <class T> inline bool Line2<T>::CrossPoint(const Line2<T> &_l, Matrix21<T> &_p) const
{
  double A,B,C,D,E,F,det;

  A = p2.x - p1.x;
  B = p2.y - p1.y;

  C = _l.p2.x - _l.p1.x;
  D = _l.p2.y - _l.p1.y;

  det = D*A-B*C;

  if( det*det < DBL_EPSILON){
	_p = Matrix21<T>(0.0,0.0);
    return false;
  }

  E = B*   p1.x - A*   p1.y;
  F = D*_l.p1.x - C*_l.p1.y;

  _p.x = (A*F-E*C)/det;
  _p.y = (B*F-D*E)/det;
  return Col_Line(_l);
}

//����_�̐����ƒ�������n�_�����߂�B���s�n�_������Ȃ�true.
template <class T> inline bool Line2<T>::OrthoPoint(const Matrix21<T> &_p, Matrix21<T> &_h) const
{
	T k;
	Matrix21<T> s = p2-p1;
	k = s.DotMul(_p)/s.Abs2();
	_h = s*k + p1;
	return (0.0<=k&&k<=1.0);
}

//����_�́A���Ώ̂Ȉʒu�����߂�D���̈ʒu���琂���������đΏې���Ȃ��true.
template <class T> inline bool Line2<T>::LineSymmetryPoint(const Matrix21<T> &_p, Matrix21<T> &_q) const
{
	Matrix21<T> _h;
	bool ret = OrthoPoint(_p,_h);
	_q = _h*2 - _p;
	return ret;
}



//==========
//�O�p�`
//============
template<class T> Triangle2<T>::Triangle2(){}
template<class T> Triangle2<T>::Triangle2(const Triangle2<T> &_t){ p1 = _t.p1; p2 = _t.p2; p3 = _t.p3; }
template<class T> Triangle2<T>::Triangle2(const Line2<T> &_l1, const Line2<T> &_l2, const Line2<T> &_l3)
{
	p1=_l1.p1; p2=_l2.p1; p3=_l3.p1;
}
template<class T> Triangle2<T>::Triangle2(const Matrix21<T> &_p1, const Matrix21<T> &_p2, const Matrix21<T> &_p3)
{
	p1=_p1; p2=_p2; p3=_p3;
}

template<class T> Triangle2<T>::Triangle2(T _x1,T _y1,T _x2,T _y2,T _x3,T _y3)
{
	p1 = Matrix21<T>(_x1,_y1);
	p2 = Matrix21<T>(_x2,_y2);
	p3 = Matrix21<T>(_x3,_y3);
}

//������
template<class T> inline bool Triangle2<T>::operator==(const Triangle2<T> &_t) const { return (p1==_t.p1&&p2==_t.p2&&p3==_t.p3);}
//���
template<class T> inline Triangle2<T>& Triangle2<T>::operator=(const Triangle2<T> &_t)
{
	p1=_t.p1; p2=_t.p2; p3=_t.p3;
	return *this;
}

//�O�p�`�Ɠ_�̔���
//{0(x,y)��1(x,y)��2(x,y)},�����p(x,y)
//�������true
template<class T> inline bool Triangle2<T>::Col_Dot(const Matrix21<T> &_p) const {
  double mulres[3];
  mulres[0] = (p1-p2).CrsMul(Matrix21<T>(p1-_p));
  mulres[1] = (p2-p3).CrsMul(Matrix21<T>(p2-_p));
  mulres[2] = (p3-p1).CrsMul(Matrix21<T>(p3-_p));

  if(mulres[0]>0.0 && mulres[1]>0.0 && mulres[2]>0.0) return true;
  if(mulres[0]<0.0 && mulres[1]<0.0 && mulres[2]<0.0) return true;
  return false;
}

//�O�p�`�Ɛ��̔���
template<class T> inline bool Triangle2<T>::Col_Line(const Line2<T> &_l) const 
{
	if( (Line2<T>(p1,p2)).Col_Line(_l) ) return true;
	if( (Line2<T>(p2,p3)).Col_Line(_l) ) return true;
	if( (Line2<T>(p3,p1)).Col_Line(_l) ) return true;
	//�Ō�ɓ�������
	return Col_Dot(_l.p1);
}

//�O�p�`�ƎO�p�`�̔���
template<class T> inline bool Triangle2<T>::Col_Tri(const Triangle2<T> &_t) const 
{
	if( Col_Line(Line2<T>(_t.p1,_t.p2)) ) return true;
	if( Col_Line(Line2<T>(_t.p2,_t.p3)) ) return true;
	if( Col_Line(Line2<T>(_t.p3,_t.p1)) ) return true;
	//�Ō�ɓ�������
	//�O�p�`�݂͌��ɓ����ɂ���\�����l����
	if( Col_Dot(_t.p1) ) return true;
	return _t.Col_Dot(p1);
}

//���̑�
template <class T> inline bool Triangle2<T>::Col_Circle (const Circle2<T> &_mt) const { return _mt.Col_Tri(*this); }

//====
//�~
//====
template<class T> Circle2<T>::Circle2(){}
template<class T> Circle2<T>::Circle2(const Circle2<T> &_c){ p=_c.p; d=_c.d;}
template<class T> Circle2<T>::Circle2(const Matrix21<T> &_p,T &_d){ p=_p; d=_d; }
template<class T> Circle2<T>::Circle2(T _x, T _y, T _d){ p=Matrix21<T>(_x,_y); d=_d; }
//���
template<class T> Circle2<T>& Circle2<T>::operator=(const Circle2<T> &_c)
{
	p=_c.p; d=_c.d;
	return *this;
}

//�~�Ɠ_�̓����蔻��
//d�͉~�̒��a
//����������true
template<class T> inline bool Circle2<T>::Col_Dot(const Matrix21<T> &_p) const
{
	if( d*d/4.0 > (Line2<T>(p,_p)).length2() ) return true;
  return false;
}

//�~�Ɖ~
//����������true
template<class T> inline bool Circle2<T>::Col_Circle (const Circle2<T> &_c) const
{
	if( (d+_c.d)*(d+_c.d)/4.0 > (Line2<T>(p,_c.p)).length2() ) return true;
	return false;
}

//���Ɖ~�̓����蔻��
//����͂܂�,�_�ƒ����̋��������߂Ă���𔻒肷�邱�Ƃɓ�����
//�����̍��W��2�A�~�̒��S���W����A�Ō�ɂ͉~�̒��a�����悤
//����������true
template<class T> inline bool Circle2<T>::Col_Line (const Line2<T> &_l) const
{

  double A,B,C,D,E,F,G,dist;
  A = _l.p2.x - _l.p1.x;
  B = _l.p2.y - _l.p1.y;
  C = B*_l.p1.x - A*_l.p1.y;
  F = A*A+B*B;
  dist = fabs(B*p.x-A*p.y-C)/sqrt(F);

  D = (Line2<T>(p,_l.p1)).length2() ;
  E = (Line2<T>(p,_l.p2)).length2() ;
  G = (D>E) ? D : E;

  if(dist <= d/2 && G*G-dist*dist <= F*F)return true;

  //�[�̓_�Ɖ~�̓����蔻����s��
  //���ꂪ�Ȃ��Ɛ��m����Ȃ�
  if(Col_Dot(_l.p1))return true;
  if(Col_Dot(_l.p2))return true;

  return false;
}

//�O�p�`�Ɖ~�̓����蔻��
//�������true
template<class T> inline bool Circle2<T>::Col_Tri(const Triangle2<T> &_t) const
{
	// �~��3�̒����Ƃ̓����蔻��
	if( Col_Line(Line2<T>(_t.p1,_t.p2)) ) return true;
	if( Col_Line(Line2<T>(_t.p2,_t.p3)) ) return true;
	if( Col_Line(Line2<T>(_t.p3,_t.p1)) ) return true;

	//�����ƌ����Ȃ��Ă��A�O�p�`�̓����ɉ~������ꍇ�͂����������ƂɂȂ�
	return _t.Col_Dot(p);
}



}

#endif
