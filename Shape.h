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
	//insect函数是求两圆交点的函数，如有交点则返回交点个数，若无则返回0，若圆重合则返回-1
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


//广义上的Polygon，私有成员edges_中的智能指针可指向ArcEdge，也可指向DirectEdge
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