#ifndef EDGE_H
#define EDGE_H
#include <iostream>
using namespace std;

//��Ҫ��ǰ������
class Circle;  

class Point
{
//��Ҫ�õ�Point��˽�г�Ա����Ԫ��������Ԫ��
	friend double distance(Point* p1, Point* p2);
	friend int insect(Point p[], Circle c1, Circle c2);
	friend class Polygon;
	friend class DirectEdge;
	friend class ArcEdge;
public:
	Point(double x, double y) :x_{ x }, y_{ y }{}
	void print() { cout << "(" << x_ << "," << y_ << ")"; }
private:
	double x_, y_;
};

//��ΪҪ������ArcEdge�࣬�ʰ�Edge��Ϊ�����࣬������DirectEdge��ArcEdge������
class Edge
{
public:
	virtual double Length() = 0;
	virtual void Print() = 0;
};

//DirectEdge����ԭ���ļ���Ҫ���Edge��
class DirectEdge : public Edge
{
	friend class Polygon;
public:
	DirectEdge(Point p1_, Point p2_) :p1{ p1_ }, p2{ p2_ }{}
	DirectEdge(DirectEdge& e) :p1{ e.p1 }, p2{ e.p2 }{}
	double Length() { return sqrt(pow(p1.x_ - p2.x_, 2) + pow(p1.y_ - p2.y_, 2)); }
	void Print() { cout << "DirectEdge:"; p1.print(); cout << "����"; p2.print(); cout << endl; }
private:
	Point p1, p2;

};

//ArcEdge���ڹ�������ϵ�Polygon�����ĸ�˽�г�Ա��Բ�ģ������˵㼰����
class ArcEdge :public Edge
{
public:
	ArcEdge(Point center, Point p1, Point p2, double radian) :center_{ center }, p1_{ p1 }, p2_{ p2 }, radian_{ radian }{}
	ArcEdge(ArcEdge& a) :center_{ a.center_ }, p1_{ a.p1_ }, p2_{ a.p2_ }, radian_{ a.radian_ }{}
	double Length() { return radian_ * (sqrt(pow(p1_.x_ - center_.x_, 2) + pow(p1_.y_ - center_.y_, 2))); }
	void Print() { cout << "ArcEdge:" << " center:"; center_.print(); cout << "Point:"; p1_.print(); cout << "  "; p2_.print(); cout << "radian:" << radian_ << endl; }
private:
	Point center_, p1_, p2_;
	double radian_;
};
#endif