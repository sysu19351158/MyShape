#ifndef SHAPE_H
#define SHAPE_H


#include <iostream>
#include "Edge.h"
#include <vector>
using namespace std;
static const double Pi = 3.1415926;

class Shape
{
	friend shared_ptr<Shape> operator&(shared_ptr<Shape>, shared_ptr<Shape>);
	friend shared_ptr<Shape> operator|(shared_ptr<Shape>, shared_ptr<Shape>);
public:
	virtual double Circumference()const = 0;
	virtual double Area()const = 0;
	virtual void Print() = 0;
	virtual bool IsValid()const = 0;
protected:
	static const int UnknownValue = -1;
};



class Circle :public Shape
{
	friend shared_ptr<Shape> operator&(shared_ptr<Shape>, shared_ptr<Shape>);
	friend shared_ptr<Shape> operator|(shared_ptr<Shape>, shared_ptr<Shape>);
	//insect����������Բ����ĺ��������н����򷵻ؽ�������������򷵻�0����Բ�غ��򷵻�-1
	friend int insect(Point p[], Circle c1, Circle c2);
public:
	Circle(Point center, double radius) :center_{ center }, radius_{ radius }{}
	Circle(double x, double y, double radius) :center_(x, y), radius_{ radius }{}
	virtual double Circumference() const override { return 2 * Pi * radius_; }
	virtual double Area()const override { return Pi * radius_ * radius_; }
	virtual void Print() override { cout << "Center:"; center_.print(); cout << "radius:" << radius_ << " Area:" << Area() << " Circumference:" << Circumference() << endl; }
	virtual bool IsValid()const override;
private:
	Point center_;
	double radius_;
};


//�����ϵ�Polygon��˽�г�Աedges_�е�����ָ���ָ��ArcEdge��Ҳ��ָ��DirectEdge
class Polygon : public Shape
{
	friend shared_ptr<Shape> operator&(shared_ptr<Shape>, shared_ptr<Shape>);
	friend shared_ptr<Shape> operator|(shared_ptr<Shape>, shared_ptr<Shape>);
public:
	Polygon() {}
	Polygon(vector<shared_ptr<Edge>> edges);
	virtual double Circumference()const override;
	virtual double Area()const override;
	virtual void Print() override;
	virtual bool IsValid()const override;

private:
	vector<shared_ptr<Edge>> edges_;

};
#endif