#include <iostream>
#include "Shape.h"
#include "Edge.h"
using namespace std;

//����������֮��ľ���ĺ���
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


//����ָ��DirectEgdge��ArcEdge��vector����������Polygon
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

//����DirectEdge�����Polygon�����������������ArcEdge����ģ��򷵻�UnknowValue
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

//��Ҫ�ж�DirectEdge�Ƿ����Χ��һ�����ͼ�Σ������ܣ��򷵻�0�����ܣ��򷵻�1. ����ArcEdge����ģ�Ĭ�Ϸ���1
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

//�˴�ֻ����Բ�Ľ�����������εĽ��������ڸ��ӣ���û��д
shared_ptr<Shape> operator&(shared_ptr<Shape>s1, shared_ptr<Shape> s2)
{
	//�ж��Ƿ���Բ
	if (dynamic_pointer_cast<Circle>(s1) && dynamic_pointer_cast<Circle>(s2))
	{
		//Բ�Ľ�����
		vector<shared_ptr<Edge>> edges;
		Point p[] = { Point(0,0),Point(0,0) };
		Circle c[] = { *dynamic_pointer_cast<Circle>(s1),*dynamic_pointer_cast<Circle>(s2) };
		//�������
		int i = insect(p, c[0], c[1]);
		//�޽��㣬�غϣ�ֻ��һ������ʱ�������ؿ�
		if (i == 0 || i == -1 || i == 1)
			return shared_ptr<Shape>(new Polygon);
		//����������
		else if (i == 2)
		{
			//�����������radian1��radian2����Բ�ľ���d
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
			//����Բ�ĽϿ������������Ļ��ȴ���Pi,����Ҫ�Ի��ȵĴ�С���в��������ڹ���ArcEdge
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
			//������ˣ�����������ArcEdge
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

//�������뽻����������ͬ
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