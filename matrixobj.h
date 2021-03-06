#ifndef _MATRIXOBJ_H_
#define _MATRIXOBJ_H_

#include <cmath>
#include <cfloat>

namespace space
{

//きわめて普通な3行一列の行列ともいえなくはない何か
//点の構造体といえなくもない
template <class T=double>
struct Matrix31
{
	T x;
	T y;
	T z;

	Matrix31();
	Matrix31(const Matrix31<T> &_t);
	Matrix31(T _x,T _y,T _z);

	//足し算
	inline Matrix31<T> operator+(const Matrix31<T> &_t);
	//引き算
	inline Matrix31<T> operator-(const Matrix31<T> &_t);
	//代入
	inline Matrix31<T>& operator=(const Matrix31<T> &_t);
	//等しい
	inline bool operator==(const Matrix31<T> &_t) const;

	//定数との足し算
	inline Matrix31<T> operator+(T _c);
	//定数との引き算
	inline Matrix31<T> operator-(T _c);
	//定数との掛け算
	inline Matrix31<T> operator*(T _c);
	//定数との割り算
	inline Matrix31<T> operator/(T _c);

	//絶対値
	inline T Abs2();
	inline T Abs();

	//内積
	inline T DotMul(const Matrix31<T> &_t);
	//外積
	inline Matrix31<T> CrsMul(const Matrix31<T> &_t);

};


//前方宣言
template <class T> struct Matrix21;
template <class T> struct Line2;
template <class T> struct Triangle2;
template <class T> struct Circle2;

//きわめて普通な2行一列の行列ともいえなくはない何か
//点の構造体といえなくもない
template <class T=double>
struct Matrix21
{
	T x;
	T y;

	Matrix21();
	Matrix21(const Matrix21<T> &_t);
	Matrix21(T _x,T _y);

	//足し算
	inline Matrix21<T> operator+(const Matrix21<T> &_t) const;
	//引き算
	inline Matrix21<T> operator-(const Matrix21<T> &_t) const;

	//代入
	inline Matrix21<T>& operator=(const Matrix21<T> &_t);
	//足して代入
	inline Matrix21<T>& operator+=(const Matrix21<T> &_t);
	//引いて代入
	inline Matrix21<T>& operator-=(const Matrix21<T> &_t);
	
	//等しい
	inline bool operator==(const Matrix21<T> &_t) const;
	//等しくない
	inline bool operator!=(const Matrix21<T> &_t) const;

	//定数との足し算
	inline Matrix21<T> operator+(T _c) const;
	//定数との引き算
	inline Matrix21<T> operator-(T _c) const;
	//定数との掛け算
	inline Matrix21<T> operator*(T _c) const;
	//定数との割り算
	inline Matrix21<T> operator/(T _c) const;
	//足して代入
	inline Matrix21<T>& operator+=(T _c) ;
	//引いて代入
	inline Matrix21<T>& operator-=(T _c) ;
	//掛けて代入
	inline Matrix21<T>& operator*=(T _c) ;
	//割って代入
	inline Matrix21<T>& operator/=(T _c) ;

	//絶対値
	inline T Abs2() const;
	inline T Abs() const;

	//2次元の内積
	inline T DotMul(const Matrix21<T> &_t) const;
	//2次元の外積
	inline T CrsMul(const Matrix21<T> &_t) const;

	//キャスト
	template <class S> inline operator Matrix21<S>() const{ return Matrix21<S>((S)x,(S)y); }

	//おなじだったら。
	inline bool Col_Dot (const Matrix21 &_mt) const;
	//その他
	inline bool Col_Line (const Line2<T> &_mt) const;
	inline bool Col_Tri (const Triangle2<T> &_mt) const;
	inline bool Col_Circle (const Circle2<T> &_mt) const;
};


//==============
//二次元な人々
//==============
//直線
template <class T=double>
struct Line2
{
	//点2個
	Matrix21<T> p1,p2;

	Line2();
	Line2(const Line2 &_l);
	Line2(const Matrix21<T> &_p1, const Matrix21<T> &_p2);
	Line2(T _x1,T _y1,T _x2,T _y2);

	//代入
	inline Line2<T>& operator=(const Line2<T> &_l);
	//等しい
	inline bool operator==(const Line2<T> &_l) const;

	//長さの２乗
	inline T length2();
	inline T length();

	//2直線の当たり判定
	//{p1(x,y)→p2(x,y)}と{_l.p1(x,y)→_l.p2(x,y)}
	//当たればtrue
	inline bool Col_Line (Line2 _l) const;
	//doubleでは問題のある定義。
	inline bool Col_Dot(const Matrix21<T> &_mt) const;
	//その他
	inline bool Col_Tri (const Triangle2<T> &_mt) const;
	inline bool Col_Circle (const Circle2<T> &_mt) const;
	
	
	// 二つの線分の交点を求める。戻り値trueで交わっている.
	inline bool CrossPoint(const Line2<T> &_l, Matrix21<T> &_p) const;
	//ある点の垂線と直交する地点を求める。直行地点が線上ならtrue.
	inline bool OrthoPoint(const Matrix21<T> &_p, Matrix21<T> &_h) const;
	//ある点の、線対称な位置を求める．その位置から垂線を引いて対象線上ならばtrue.
	inline bool LineSymmetryPoint(const Matrix21<T> &_p, Matrix21<T> &_q) const;

};
	
//三角形
template <class T=double>
struct Triangle2
{
	//点３こ
	Matrix21<T> p1,p2,p3;

	Triangle2();
	Triangle2(const Triangle2<T> &_t);
	Triangle2(const Line2<T> &_l1, const Line2<T> &_l2, const Line2<T> &_l3);
	Triangle2(const Matrix21<T> &_p1, const Matrix21<T> &_p2, const Matrix21<T> &_p3);
	
	Triangle2(T _x1,T _y1,T _x2,T _y2,T _x3,T _y3);
	
	//等しい
	inline bool operator==(const Triangle2<T> &_t) const;
	//代入
	inline Triangle2<T>& operator=(const Triangle2<T> &_t);

	//三角形と点の判定
	//{0(x,y)→1(x,y)→2(x,y)},およびp(x,y)
	//当たればtrue
	inline bool Col_Dot(const Matrix21<T> &_p) const;
	//三角形と線の判定
	inline bool Col_Line(const Line2<T> &_l) const ;
	//三角形と三角形の判定
	inline bool Col_Tri(const Triangle2<T> &_t) const ;
	//その他
	inline bool Col_Circle (const Circle2<T> &_mt) const;

};


//円
template <class T=double>
struct Circle2
{
	Matrix21<T> p;	//中心点
	T d;	//直径

	Circle2();
	Circle2(const Circle2<T> &_c);
	Circle2(const Matrix21<T> &_p,T &_d);
	Circle2(T _x, T _y, T _d);
	//代入
	inline Circle2<T>& operator=(const Circle2<T> &_c);

	//円と点の当たり判定
	//dは円の直径
	//当たったらtrue
	inline bool Col_Dot(const Matrix21<T> &_p) const;
	//円と円
	//当たったらtrue
	inline bool Col_Circle (const Circle2<T> &_c) const;
	//線と円の当たり判定
	//これはつまり,点と直線の距離を求めてそれを判定することに等しい
	//直線の座標を2つ、円の中心座標を一つ、最後には円の直径を入れよう
	//当たったらtrue
	inline bool Col_Line (const Line2<T> &_l) const;
	//三角形と円の当たり判定
	//あたればtrue
	inline bool Col_Tri(const Triangle2<T> &_t) const;

};



//===========================
//きわめて普通な3行一列の行列
//===========================
template <class T> Matrix31<T>::Matrix31(){}
template <class T> Matrix31<T>::Matrix31(const Matrix31<T> &_t){ x=_t.x; y=_t.y; z=_t.z;}
template <class T> Matrix31<T>::Matrix31(T _x,T _y,T _z){ x=_x;y=_y; z=_z;}

//足し算
template <class T> inline Matrix31<T> Matrix31<T>::operator+(const Matrix31<T> &_t)
{ return Matrix31<T>(x+_t.x,y+_t.y,z+_t.z); }
//引き算
template <class T> inline Matrix31<T> Matrix31<T>::operator-(const Matrix31<T> &_t)
{ return Matrix31<T>(x-_t.x,y-_t.y,z-_t.z); }
//代入
template <class T> inline Matrix31<T>& Matrix31<T>::operator=(const Matrix31<T> &_t)
{
	x=_t.x; y=_t.y; z=_t.z;
	return *this;
}
//等しい
template <class T> inline bool Matrix31<T>::operator==(const Matrix31<T> &_t) const
{ return (x==_t.x&&y==_t.y&&z==_t.z);}

//定数との足し算
template <class T> inline Matrix31<T> Matrix31<T>::operator+(T _c)
{ return Matrix31(x+_c,y+_c,z+_c); }
//定数との引き算
template <class T> inline Matrix31<T> Matrix31<T>::operator-(T _c)
{ return Matrix31(x-_c,y-_c,z-_c); }
//定数との掛け算
template <class T> inline Matrix31<T> Matrix31<T>::operator*(T _c)
{ return Matrix31(x*_c,y*_c,z*_c); }
//定数との割り算
template <class T> inline Matrix31<T> Matrix31<T>::operator/(T _c)
{ return Matrix31(x/_c,y/_c,z/_c); }

//絶対値
template <class T> inline T Matrix31<T>::Abs2() {return x*x+y*y+z*z;}
template <class T> inline T Matrix31<T>::Abs() {return sqrt(Abs2());}

//内積
template <class T> inline T Matrix31<T>::DotMul(const Matrix31<T> &_t)
{ return (_t.x*x + _t.y*y + _t.z*z); }
//外積
template <class T> inline Matrix31<T> Matrix31<T>::CrsMul(const Matrix31<T> &_t)
{ return Matrix31(y*_t.z-z*_t.y ,z*_t.x-x*_t.z, x*_t.y-y*_t.x); }




//===========================
//きわめて普通な2行一列の行列
//===========================
template <class T> Matrix21<T>::Matrix21(){}
template <class T> Matrix21<T>::Matrix21(const Matrix21<T> &_t){ x=_t.x; y=_t.y; }
template <class T> Matrix21<T>::Matrix21(T _x,T _y){ x=_x;y=_y; }

//足し算
template <class T> inline Matrix21<T> Matrix21<T>::operator+(const Matrix21<T> &_t) const
{ return Matrix21<T>(x+_t.x,y+_t.y); }
//引き算
template <class T> inline Matrix21<T> Matrix21<T>::operator-(const Matrix21<T> &_t) const 
{ return Matrix21<T>(x-_t.x,y-_t.y); }

//代入
template <class T> inline Matrix21<T>& Matrix21<T>::operator=(const Matrix21<T> &_t)
{ x=_t.x; y=_t.y; return *this; }
//足して代入
template <class T> inline Matrix21<T>& Matrix21<T>::operator+=(const Matrix21<T> &_t)
{ x+=_t.x; y+=_t.y; return *this; }
//引いて代入
template <class T> inline Matrix21<T>& Matrix21<T>::operator-=(const Matrix21<T> &_t)
{ x-=_t.x; y-=_t.y; return *this; }

//等しい
template <class T> inline bool Matrix21<T>::operator==(const Matrix21<T> &_t) const { return (x==_t.x&&y==_t.y); }
//等しくない
template <class T> inline bool Matrix21<T>::operator!=(const Matrix21<T> &_t) const { return (x!=_t.x||y!=_t.y); }

//定数との足し算
template <class T> inline Matrix21<T> Matrix21<T>::operator+(T _c) const { return Matrix21(x+_c,y+_c); }
//定数との引き算
template <class T> inline Matrix21<T> Matrix21<T>::operator-(T _c) const { return Matrix21(x-_c,y-_c); }
//定数との掛け算
template <class T> inline Matrix21<T> Matrix21<T>::operator*(T _c) const { return Matrix21(x*_c,y*_c); }
//定数との割り算
template <class T> inline Matrix21<T> Matrix21<T>::operator/(T _c) const { return Matrix21(x/_c,y/_c); }
//足して代入
template <class T> inline Matrix21<T>& Matrix21<T>::operator+=(T _c) { x+=_c; y+=_c; return *this; }
//引いて代入
template <class T> inline Matrix21<T>& Matrix21<T>::operator-=(T _c) { x-=_c; y-=_c; return *this; }
//掛けて代入
template <class T> inline Matrix21<T>& Matrix21<T>::operator*=(T _c) { x*=_c; y*=_c; return *this; }
//割って代入
template <class T> inline Matrix21<T>& Matrix21<T>::operator/=(T _c) { x/=_c; y/=_c; return *this; }

//絶対値
template <class T> inline T Matrix21<T>::Abs2() const {return x*x+y*y;}
template <class T> inline T Matrix21<T>::Abs() const {return sqrt(Abs2());}

//2次元の内積
template <class T> inline T Matrix21<T>::DotMul(const Matrix21<T> &_t) const { return (x*_t.x + y*_t.y); }
//2次元の外積
template <class T> inline T Matrix21<T>::CrsMul(const Matrix21<T> &_t) const { return (x*_t.y - y*_t.x); }

//おなじだったら。
template <class T> inline bool Matrix21<T>::Col_Dot (const Matrix21<T> &_mt) const { return _mt==*this; }
//その他
template <class T> inline bool Matrix21<T>::Col_Line (const Line2<T> &_mt) const { return _mt.Col_Dot(*this); }
template <class T> inline bool Matrix21<T>::Col_Tri (const Triangle2<T> &_mt) const { return _mt.Col_Dot(*this); }
template <class T> inline bool Matrix21<T>::Col_Circle (const Circle2<T> &_mt) const { return _mt.Col_Dot(*this); }



//==============
//直線
//=============
template <class T> inline Line2<T>::Line2(){}
template <class T> inline Line2<T>::Line2(const Line2 &_l) {p1=_l.p1; p2=_l.p2;}
template <class T> inline Line2<T>::Line2(const Matrix21<T> &_p1, const Matrix21<T> &_p2){ p1=_p1; p2=_p2; }
template <class T> inline Line2<T>::Line2(T _x1,T _y1,T _x2,T _y2)
{
	p1=Matrix21<T>(_x1,_y1);
	p2=Matrix21<T>(_x2,_y2);
}

//代入
template <class T> inline Line2<T>& Line2<T>::operator=(const Line2<T> &_l)
{
	p1=_l.p1; p2=_l.p2;
	return *this;
}
//等しい
template <class T> inline bool Line2<T>::operator==(const Line2<T> &_l) const { return (p1==_l.p1&&p2==_l.p2);}

//長さの２乗
template <class T> inline T Line2<T>::length2(){ return (p1-p2).Abs2(); }
template <class T> inline T Line2<T>::length() { return sqrt( length2() ); }

//2直線の当たり判定
//{p1(x,y)→p2(x,y)}と{_l.p1(x,y)→_l.p2(x,y)}
//当たればtrue
template <class T> inline bool Line2<T>::Col_Line (Line2<T> _l) const {
  double mulres[2];
  mulres[0] = (p1-p2).CrsMul(p1-_l.p1) * (p1-p2).CrsMul(p1-_l.p2);
  mulres[1] = (_l.p1-_l.p2).CrsMul(_l.p1-p1) * (_l.p1-_l.p2).CrsMul(_l.p1-p2);

  if ( mulres[0] < 0.0 && mulres[1] < 0.0 ) return true;
  return false;
}

//doubleでは問題のある定義。
template <class T> inline bool Line2<T>::Col_Dot(const Matrix21<T> &_mt) const
{
	return (_mt.x-p1.x/p2.x-p1.x==_mt.y-p1.y/p2.y-p1.y);
}

//その他
template <class T> inline bool Line2<T>::Col_Tri (const Triangle2<T> &_mt) const { return _mt.Col_Line(*this); }
template <class T> inline bool Line2<T>::Col_Circle (const Circle2<T> &_mt) const { return _mt.Col_Line(*this); }

// 二つの線分の交点を求める。戻り値trueで交わっている.
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

//ある点の垂線と直交する地点を求める。直行地点が線上ならtrue.
template <class T> inline bool Line2<T>::OrthoPoint(const Matrix21<T> &_p, Matrix21<T> &_h) const
{
	T k;
	Matrix21<T> s = p2-p1;
	k = s.DotMul(_p)/s.Abs2();
	_h = s*k + p1;
	return (0.0<=k&&k<=1.0);
}

//ある点の、線対称な位置を求める．その位置から垂線を引いて対象線上ならばtrue.
template <class T> inline bool Line2<T>::LineSymmetryPoint(const Matrix21<T> &_p, Matrix21<T> &_q) const
{
	Matrix21<T> _h;
	bool ret = OrthoPoint(_p,_h);
	_q = _h*2 - _p;
	return ret;
}



//==========
//三角形
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

//等しい
template<class T> inline bool Triangle2<T>::operator==(const Triangle2<T> &_t) const { return (p1==_t.p1&&p2==_t.p2&&p3==_t.p3);}
//代入
template<class T> inline Triangle2<T>& Triangle2<T>::operator=(const Triangle2<T> &_t)
{
	p1=_t.p1; p2=_t.p2; p3=_t.p3;
	return *this;
}

//三角形と点の判定
//{0(x,y)→1(x,y)→2(x,y)},およびp(x,y)
//当たればtrue
template<class T> inline bool Triangle2<T>::Col_Dot(const Matrix21<T> &_p) const {
  double mulres[3];
  mulres[0] = (p1-p2).CrsMul(Matrix21<T>(p1-_p));
  mulres[1] = (p2-p3).CrsMul(Matrix21<T>(p2-_p));
  mulres[2] = (p3-p1).CrsMul(Matrix21<T>(p3-_p));

  if(mulres[0]>0.0 && mulres[1]>0.0 && mulres[2]>0.0) return true;
  if(mulres[0]<0.0 && mulres[1]<0.0 && mulres[2]<0.0) return true;
  return false;
}

//三角形と線の判定
template<class T> inline bool Triangle2<T>::Col_Line(const Line2<T> &_l) const 
{
	if( (Line2<T>(p1,p2)).Col_Line(_l) ) return true;
	if( (Line2<T>(p2,p3)).Col_Line(_l) ) return true;
	if( (Line2<T>(p3,p1)).Col_Line(_l) ) return true;
	//最後に内部判定
	return Col_Dot(_l.p1);
}

//三角形と三角形の判定
template<class T> inline bool Triangle2<T>::Col_Tri(const Triangle2<T> &_t) const 
{
	if( Col_Line(Line2<T>(_t.p1,_t.p2)) ) return true;
	if( Col_Line(Line2<T>(_t.p2,_t.p3)) ) return true;
	if( Col_Line(Line2<T>(_t.p3,_t.p1)) ) return true;
	//最後に内部判定
	//三角形は互いに内側にいる可能性を考える
	if( Col_Dot(_t.p1) ) return true;
	return _t.Col_Dot(p1);
}

//その他
template <class T> inline bool Triangle2<T>::Col_Circle (const Circle2<T> &_mt) const { return _mt.Col_Tri(*this); }

//====
//円
//====
template<class T> Circle2<T>::Circle2(){}
template<class T> Circle2<T>::Circle2(const Circle2<T> &_c){ p=_c.p; d=_c.d;}
template<class T> Circle2<T>::Circle2(const Matrix21<T> &_p,T &_d){ p=_p; d=_d; }
template<class T> Circle2<T>::Circle2(T _x, T _y, T _d){ p=Matrix21<T>(_x,_y); d=_d; }
//代入
template<class T> Circle2<T>& Circle2<T>::operator=(const Circle2<T> &_c)
{
	p=_c.p; d=_c.d;
	return *this;
}

//円と点の当たり判定
//dは円の直径
//当たったらtrue
template<class T> inline bool Circle2<T>::Col_Dot(const Matrix21<T> &_p) const
{
	if( d*d/4.0 > (Line2<T>(p,_p)).length2() ) return true;
  return false;
}

//円と円
//当たったらtrue
template<class T> inline bool Circle2<T>::Col_Circle (const Circle2<T> &_c) const
{
	if( (d+_c.d)*(d+_c.d)/4.0 > (Line2<T>(p,_c.p)).length2() ) return true;
	return false;
}

//線と円の当たり判定
//これはつまり,点と直線の距離を求めてそれを判定することに等しい
//直線の座標を2つ、円の中心座標を一つ、最後には円の直径を入れよう
//当たったらtrue
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

  //端の点と円の当たり判定を行う
  //これがないと正確じゃない
  if(Col_Dot(_l.p1))return true;
  if(Col_Dot(_l.p2))return true;

  return false;
}

//三角形と円の当たり判定
//あたればtrue
template<class T> inline bool Circle2<T>::Col_Tri(const Triangle2<T> &_t) const
{
	// 円と3つの直線との当たり判定
	if( Col_Line(Line2<T>(_t.p1,_t.p2)) ) return true;
	if( Col_Line(Line2<T>(_t.p2,_t.p3)) ) return true;
	if( Col_Line(Line2<T>(_t.p3,_t.p1)) ) return true;

	//直線と交わらなくても、三角形の内部に円がある場合はあたったことになる
	return _t.Col_Dot(p);
}



}

#endif
