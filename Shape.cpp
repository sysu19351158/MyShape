#include <iostream>
#include "Shape.h"
#include "Edge.h"
using namespace std;

//用于求两点之间的距离的函数
double distance(Point* p1, Point* p2)
{
	return sqrt(pow(p1->x_ - p2->x_, 2) + pow(p1->y_ - p2->y_, 2));
}




bool Circle::IsValid() const
{
	if (radius_ <= 0)
		return false;
	else
		return true;
}


//用由指向DirectEgdge或ArcEdge的vector数组来构造Polygon
Polygon::Polygon(vector<shared_ptr<Edge>> edges)
{
	if (dynamic_pointer_cast<DirectEdge>(edges[0]))
		for (unsigned int i = 0; i < edges.size(); i++)
			edges_.push_back(shared_ptr<Edge>(new DirectEdge(*dynamic_pointer_cast<DirectEdge>(edges[i]))));
	else if (dynamic_pointer_cast<ArcEdge>(edges[0]))
		for (unsigned int i = 0; i < edges.size(); i++)
			edges_.push_back(shared_ptr<Edge>(new ArcEdge(*dynamic_pointer_cast<ArcEdge>(edges[i]))));
}


double Polygon::Circumference()const
{
	double c = 0;
	auto size = edges_.size();
	for (unsigned int i = 0; i < size; i++)
		c += edges_[i]->Length();
	return c;
}

//若是DirectEdge构造的Polygon，则可求出面积，若是ArcEdge构造的，则返回UnknowValue
double Polygon::Area()const
{
	if (dynamic_pointer_cast<DirectEdge>(edges_[0]))
	{
		double area = 0;
		unsigned int i_count = edges_.size();
		for (unsigned int i = 0; i < i_count; i++)
			area += (dynamic_pointer_cast<DirectEdge>(edges_[i])->p1.x_ * dynamic_pointer_cast<DirectEdge>(edges_[(i + 1) % i_count])->p1.y_ - dynamic_pointer_cast<DirectEdge>(edges_[(i + 1) % i_count])->p1.x_ * dynamic_pointer_cast<DirectEdge>(edges_[i])->p1.y_);
		return abs(0.5 * area);
	}
	else
		return UnknownValue;
}



void Polygon::Print()
{
	cout << "Edges of Polygon:" << endl;
	for (unsigned int i = 0; i < edges_.size(); i++)
	{
		edges_[i]->Print();
	}
	cout << "Area:" << Area() << " Circumference:" << Circumference() << endl;
}

//主要判断DirectEdge是否可以围成一个封闭图形，若不能，则返回0，若能，则返回1. 若是ArcEdge构造的，默认返回1
bool Polygon::IsValid()const
{
	int judge = 1;
	unsigned int i_count = edges_.size();
	if (i_count == 0)
		return 0;
	else if (dynamic_pointer_cast<DirectEdge>(edges_[0]))
	{
		for (unsigned int i = 0; i < i_count; i++)
			if (!(dynamic_pointer_cast<DirectEdge>(edges_[i])->p2.x_ == dynamic_pointer_cast<DirectEdge>(edges_[(i + 1) % i_count])->p1.x_ && dynamic_pointer_cast<DirectEdge>(edges_[i])->p2.y_ == dynamic_pointer_cast<DirectEdge>(edges_[(i + 1) % i_count])->p1.y_))
			{
				judge = 0;  break;
			}
		if (judge)
			return 1;
		else
			return 0;
	}
	else
		return 1;
}

//此处只进行圆的交操作，多边形的交操作过于复杂，故没有写
shared_ptr<Shape> operator&(shared_ptr<Shape>s1, shared_ptr<Shape> s2)
{
	//判断是否都是圆
	if (dynamic_pointer_cast<Circle>(s1) && dynamic_pointer_cast<Circle>(s2))
	{
		//圆的交操作
		vector<shared_ptr<Edge>> edges;
		Point p[] = { Point(0,0),Point(0,0) };
		Circle c[] = { *dynamic_pointer_cast<Circle>(s1),*dynamic_pointer_cast<Circle>(s2) };
		//求出交点
		int i = insect(p, c[0], c[1]);
		//无交点，重合，只有一个交点时，均返回空
		if (i == 0 || i == -1 || i == 1)
			return shared_ptr<Shape>(new Polygon);
		//有两个交点
		else if (i == 2)
		{
			//求出两个弧度radian1，radian2和两圆心距离d
			double a1, b1, a2, b2, c, d;
			double radian1, radian2;
			a1 = distance(&dynamic_pointer_cast<Circle>(s1)->center_, &p[0]);
			b1 = distance(&dynamic_pointer_cast<Circle>(s1)->center_, &p[1]);
			c = distance(&p[0], &p[1]);
			a2 = distance(&dynamic_pointer_cast<Circle>(s2)->center_, &p[0]);
			b2 = distance(&dynamic_pointer_cast<Circle>(s2)->center_, &p[1]);
			radian1 = acos((a1 * a1 + b1 * b1 - c * c) / (2 * a1 * b1));
			radian2 = acos((a2 * a2 + b2 * b2 - c * c) / (2 * a2 * b2));
			d = distance(&dynamic_pointer_cast<Circle>(s1)->center_, &dynamic_pointer_cast<Circle>(s2)->center_);
			//若两圆心较靠近，交出来的弧度大于Pi,则需要对弧度的大小进行操作以用于构造ArcEdge
			if (d < sqrt(abs(pow(dynamic_pointer_cast<Circle>(s1)->radius_, 2) - pow(dynamic_pointer_cast<Circle>(s2)->radius_, 2))))
			{
				if (dynamic_pointer_cast<Circle>(s1)->radius_ < dynamic_pointer_cast<Circle>(s2)->radius_)
				{
					edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s1)->center_, p[0], p[1], 2 * Pi - radian1)));
					edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s2)->center_, p[1], p[0], radian2)));
					return shared_ptr<Shape>(new Polygon(edges));
				}
				else
				{
					edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s1)->center_, p[0], p[1], radian1)));
					edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s2)->center_, p[1], p[0], 2 * Pi - radian2)));
					return shared_ptr<Shape>(new Polygon(edges));
				}
			}
			//若非如此，则正常构造ArcEdge
			else
			{
				edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s1)->center_, p[0], p[1], radian1)));
				edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s2)->center_, p[1], p[0], radian2)));
				return shared_ptr<Shape>(new Polygon(edges));
			}
		}
	}
	else
		return shared_ptr<Shape>(new Polygon);
}

//并操作与交操作大体相同
shared_ptr<Shape> operator|(shared_ptr<Shape> s1, shared_ptr<Shape> s2)
{
	if (dynamic_pointer_cast<Circle>(s1) && dynamic_pointer_cast<Circle>(s2))
	{
		vector<shared_ptr<Edge>> edges;
		Point p[] = { Point(0,0),Point(0,0) };
		Circle c[] = { *dynamic_pointer_cast<Circle>(s1),*dynamic_pointer_cast<Circle>(s2) };
		int i = insect(p, c[0], c[1]);
		if (i == 0 || i == -1)
			return shared_ptr<Shape>(new Polygon);
		else if (i == 1)
		{
			edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s1)->center_, p[0], p[0], 2 * Pi)));
			edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s2)->center_, p[0], p[0], 2 * Pi)));
			return shared_ptr<Shape>(new Polygon(edges));
		}
		else if (i == 2)
		{
			double a1, b1, a2, b2, c, d;
			double radian1, radian2;
			a1 = distance(&dynamic_pointer_cast<Circle>(s1)->center_, &p[0]);
			b1 = distance(&dynamic_pointer_cast<Circle>(s1)->center_, &p[1]);
			c = distance(&p[0], &p[1]);
			a2 = distance(&dynamic_pointer_cast<Circle>(s2)->center_, &p[0]);
			b2 = distance(&dynamic_pointer_cast<Circle>(s2)->center_, &p[1]);
			radian1 = acos((a1 * a1 + b1 * b1 - c * c) / (2 * a1 * b1));
			radian2 = acos((a2 * a2 + b2 * b2 - c * c) / (2 * a2 * b2));
			d = distance(&dynamic_pointer_cast<Circle>(s1)->center_, &dynamic_pointer_cast<Circle>(s2)->center_);
			if (d < sqrt(abs(pow(dynamic_pointer_cast<Circle>(s1)->radius_, 2) - pow(dynamic_pointer_cast<Circle>(s2)->radius_, 2))))
			{
				if (dynamic_pointer_cast<Circle>(s1)->radius_ < dynamic_pointer_cast<Circle>(s2)->radius_)
				{
					edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s1)->center_, p[0], p[1], radian1)));
					edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s2)->center_, p[1], p[0], 2 * Pi - radian2)));
					return shared_ptr<Shape>(new Polygon(edges));
				}
				else
				{
					edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s1)->center_, p[0], p[1], 2 * Pi - radian1)));
					edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s2)->center_, p[1], p[0], radian2)));
					return shared_ptr<Shape>(new Polygon(edges));
				}
			}
			else
			{
				edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s1)->center_, p[0], p[1], 2 * Pi - radian1)));
				edges.push_back(shared_ptr<Edge>(new ArcEdge(dynamic_pointer_cast<Circle>(s2)->center_, p[1], p[0], 2 * Pi - radian2)));
				return shared_ptr<Shape>(new Polygon(edges));
			}
		}
	}
	else
		return shared_ptr<Shape>(new Polygon);
}