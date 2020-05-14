#include <iostream>
#include <string>
#include "ShapeFile.h"
#include <fstream>
#include "Shape.h"
#include "Edge.h"

//求两圆交点所需的宏
#define square(val) ((val) * (val))
#define equal(c1, c2) (((c1.center_.x_) == (c2.center_.x_)) && ((c1.center_.y_) == (c2.center_.y_)) && ((c1.radius_) == (c2.radius_)))
#define yequal(c1, c2) ((c1.center_.y_) == (c2.center_.y_))
#define xequal(c1, c2) ((c1.center_.x_) == (c2.center_.x_))

using namespace std;

//求两圆交点的函数
int insect(Point p[], Circle c1, Circle c2)
{
	double error, A, B, C, K, S, x1, y1, x2, y2, x, y;
	if (equal(c1, c2) || p == NULL)
		return 0;
	else
	{
		if (yequal(c1, c2))
		{
			x = (square((c1.center_.x_)) + square((c1.center_.y_)) + square((c2.radius_)) - square((c2.center_.x_)) - square((c2.center_.y_)) - square((c1.radius_))) / (2 * (c1.center_.x_) - 2 * (c2.center_.x_));
			A = 1;
			B = (-2) * (c1.center_.y_);
			C = square(x) + square((c1.center_.x_)) + square((c1.center_.y_)) - square((c1.radius_)) - 2 * (c1.center_.x_) * x;
			error = square(B) - 4 * A * C;

			if (error < 0)
				return -1;
			else
			{
				x1 = x2 = x;
				y1 = ((-B) + sqrt(error)) / (2 * A);
				y2 = ((-B) - sqrt(error)) / (2 * A);
			}
		}
		else
			if (xequal(c1, c2))
			{
				y = (square((c1.center_.x_)) + square((c1.center_.y_)) + square((c2.radius_)) - square((c2.center_.x_)) - square((c2.center_.y_)) - square((c1.radius_))) / (2 * (c1.center_.y_) - 2 * (c2.center_.y_));
				A = 1;
				B = (-2) * (c1.center_.x_);
				C = square((c1.center_.x_)) + square((c1.center_.y_)) + square(y) - square((c1.radius_)) - 2 * y * (c1.center_.y_);
				error = square(B) - 4 * A * C;

				if (error < 0)
					return -1;
				else
				{
					y1 = y2 = y;
					x1 = ((-B) + sqrt(error)) / (2 * A);
					x2 = ((-B) - sqrt(error)) / (2 * A);
				}
			}
			else
			{
				K = (2 * (c2.center_.x_) - 2 * (c1.center_.x_)) / (2 * (c1.center_.y_) - 2 * (c2.center_.y_));
				S = (square((c1.center_.x_)) + square((c1.center_.y_)) + square((c2.radius_)) - square((c2.center_.x_)) - square((c2.center_.y_)) - square((c1.radius_))) / (2 * (c1.center_.y_) - 2 * (c2.center_.y_));

				//解二次方程
				A = (1 + square(K));
				B = 2 * K * S - 2 * (c1.center_.x_) - 2 * (c1.center_.y_) * K;
				C = square(S) + square((c1.center_.x_)) + square((c1.center_.y_)) - square((c1.radius_)) - 2 * (c1.center_.y_) * S;
				error = square(B) - 4 * A * C;

				if (error < 0)
					return -1;
				else
				{
					x1 = ((-B) + sqrt(error)) / (2 * A);
					x2 = ((-B) - sqrt(error)) / (2 * A);
					y1 = K * x1 + S;
					y2 = K * x2 + S;
				}
			}
		p[0].x_ = x1;
		p[0].y_ = y1;

		p[1].x_ = x2;
		p[1].y_ = y2;
		if (p[0].x_ == p[1].x_ && p[0].y_ == p[1].y_)
			return 1;
		else
			return 2;
	}
}




//从字符串中提取数字返回int的vector数组的函数
vector<int> GetNum(const string str, vector<int> num)
{
	int len = str.size();
	int index = 0;
	int t;
	for (int i = 0; i < len; i++)
	{
		num.push_back(0);
		while (!(str[i] > '0' && str[i] < '9'))
		{
			i++;
		}
		while (str[i] >= '0' && str[i] < '9')
		{
			t = int(str[i] - '0');
			num[index] = num[index] * 10 + t;
			i++;
		}
		index++;

	}
	return num;
}

//正常构造shapefile
ShapeFile::ShapeFile(ifstream& f)
{
	string text;
	vector<shared_ptr<Point>> points_;
	while (getline(f, text))
	{
		if (text[0] == '#')
			continue;
		else if (text[0] == 'P' && text[text.size() - 1] == ')')
		{
			vector<int> num;
			num = GetNum(text, num);
			points_.push_back(shared_ptr<Point>(new Point(double(num[1]), double(num[num.size() - 1]))));
		}
		else if (text[0] == 'C')
		{
			vector<int> num;
			num = GetNum(text, num);
			shapes_.push_back(shared_ptr<Shape>(new Circle(*(points_[num[0] - 1]), double(num[1]))));

		}
		else if (text[0] == 'P')
		{
			vector<int> num;
			vector<shared_ptr<Edge>> edges;
			num = GetNum(text, num);
			for (unsigned int i = 0; i < num.size(); i++)
				edges.push_back(shared_ptr<Edge>(new DirectEdge(*points_[num[i] - 1], *points_[num[(i + 1) % num.size()] - 1])));
			shapes_.push_back(shared_ptr<Shape>(new Polygon(edges)));
		}
	}
}

//先进行交操作，再进行并操作
void ShapeFile::Generate()
{
	for (unsigned int i = 0; i < shapes_.size(); i++)
	{
		for (unsigned int j = i + 1; j < shapes_.size(); j++)
		{
			if (j >= shapes_.size())
				break;
			if ((shapes_[i] & shapes_[j])->IsValid())
				composieShapes_.push_back(shapes_[i] & shapes_[j]);
		}
	}
	for (unsigned int i = 0; i < shapes_.size(); i++)
	{
		for (unsigned int j = i + 1; j < shapes_.size(); j++)
		{
			if (j >= shapes_.size())
				break;
			if ((shapes_[i] | shapes_[j])->IsValid())
				composieShapes_.push_back(shapes_[i] | shapes_[j]);
		}
	}
}

void ShapeFile::Print()
{
	cout << endl;
	cout << "shapes:" << endl;
	cout << endl;
	for (unsigned int i = 0; i < shapes_.size(); i++)
		shapes_[i]->Print();
	cout << endl;
	cout << "composieShapes_:" << endl;
	cout << endl;
	for (unsigned int i = 0; i < composieShapes_.size(); i++)
		composieShapes_[i]->Print();
}