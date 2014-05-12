#include <iostream>
#include <random>
#include <fstream>
#include <ctime>
#include <cmath>
#include <sstream>
using namespace std;
const int M = 1000;


//string name = "small";
//int SC_old_num = 1;
//int SC_new_num = 2;
//int BS_old_num = 3;
//int BS_new_num = 10;
//int point_num = 20;
//
//int SC_capacity = 30;
//int BS_capacity = 10;

//string name = "medium";
//int SC_old_num = 1;
//int SC_new_num = 3;
//int BS_old_num = 5;
//int BS_new_num = 15;
//int point_num = 70;
//
//int SC_capacity = 60;
//int BS_capacity = 20;


//string name = "large";
//int SC_old_num = 2;
//int SC_new_num = 6;
//int BS_old_num = 8;
//int BS_new_num = 18;
//int point_num = 160;
//
//int SC_capacity = 100;
//int BS_capacity = 40;

//string name = "extraLarge";
//int SC_old_num = 2;
//int SC_new_num = 6;
//int BS_old_num = 8;
//int BS_new_num = 18;
//int point_num = 200;
//
//int SC_capacity = 100;
//int BS_capacity = 40;

string name = "extraExtraLarge";
int SC_old_num = 2;
int SC_new_num = 6;
int BS_old_num = 8;
int BS_new_num = 18;
int point_num = 230;

int SC_capacity = 100;
int BS_capacity = 40;
int SC_num;
int BS_num;

int SC_cost = 100000;
int BS_cost = 20000;
int BS_SC_cost = 40;


int SC_min_radius = 150;
int BS_max_radius = 300;
int BS_min_radius = 40;

struct base_station;
struct switching_center;

struct point
{
public:
	int x, y;
	point()
	{};
	point(int _x, int _y) :
		x(_x), y(_y)
	{};
	int distance(base_station &);
	int distance(switching_center &);
	bool inRadius(base_station*);
};

struct base_station : point
{
public:
	base_station()
	{};
	base_station(int _x, int _y) :
		point(_x, _y)
	{};
	bool correctPlacement(base_station *bs, int t1)
	{
		for (int i = 0; i < t1; i++)
		{
			if (distance(bs[i]) < 2 * BS_min_radius)
				return false;
		}
		return true;
	}
};

struct switching_center : point
{};

int point::distance(base_station &b)
{
	return static_cast<int>(sqrt(pow(x - b.x, 2) + pow(y - b.y, 2)));
};

int point::distance(switching_center &b)
{
	return static_cast<int>(sqrt(pow(x - b.x, 2) + pow(y - b.y, 2)));
};

bool point::inRadius(base_station *bs)
{
	for (int i = 0; i < BS_num; i++)
	{
		if (distance(bs[i]) < BS_max_radius)
			return true;
	}

	return false;
};

int main()
{
	string dir = "instance/";
	
	int instCount = 20;
	for (int no = 1; no <= instCount; no++)
	{

		//cout << "in" << no << endl;

		point demand_points[400];
		switching_center switching_centers[20];
		base_station base_stations[50];



		default_random_engine generator;
		generator.seed(no+31);
		uniform_int_distribution<int> dist(0, M);


		int sc_old_add = 0.5 * (dist(generator) % SC_old_num);
		int sc_new_add = 0.5 * (dist(generator) % SC_new_num);
		int bs_old_add = 0.5 * (dist(generator) % BS_old_num);;
		int bs_new_add = 0.5 * (dist(generator) % BS_new_num);
		int point_add  = 0.5 * (dist(generator) % point_num);

		SC_old_num += sc_old_add;
		SC_new_num += sc_new_add;
		BS_old_num += bs_old_add;
		BS_new_num += bs_new_add;
		point_num += point_add;

		SC_num = SC_old_num + SC_new_num;
		BS_num = BS_old_num + BS_new_num;

		ostringstream ostr;
		ostr << dir << name << no << "_" << SC_old_num << "_" << SC_new_num << "_" << BS_old_num << "_" << BS_new_num << "_" << point_num << ".txt";

		ofstream out(ostr.str());

		out << SC_old_num << endl;
		out << SC_new_num << endl;
		out << BS_old_num << endl;
		out << BS_new_num << endl;
		out << point_num << endl;
		out << SC_cost << endl;
		out << BS_cost << endl;
		out << SC_capacity << endl;
		out << BS_capacity << endl;
		out << BS_max_radius << endl;
		//switching centers generation
		for (int i = 0; i < SC_num; i++)
		{
			switching_centers[i].x = dist(generator);
			switching_centers[i].y = dist(generator);
		}



		//base stations generation
		
		long min;
		long temp;
		int k_min;
		vector<vector<int>> Yjk;
		Yjk.resize(BS_old_num);
		for (int j = 0; j < BS_old_num; j++)
		{
			Yjk[j].resize(SC_old_num);
		}
		for (int j = 0; j < BS_old_num;)
		{
			base_stations[j].x = dist(generator);
			base_stations[j].y = dist(generator);
			if (!base_stations[j].correctPlacement(base_stations, j))
			{
				continue;
			}
			k_min = static_cast<int>(1.0*j * SC_old_num / BS_old_num);
			Yjk[j][k_min] = 1;
			j++;
		}
		for (int j = 0; j < BS_old_num; j++)
		{
			for (int k = 0; k < SC_old_num; k++)
				out << Yjk[j][k] << " ";
			out << endl;
		}

		for (int j = 0; j < BS_num;)
		{
			base_stations[j].x = dist(generator);
			base_stations[j].y = dist(generator);
			if (!base_stations[j].correctPlacement(base_stations, j))
			{
				continue;
			}
			if (j >= BS_old_num)
			{
				for (int k = 0; k < SC_num; k++)
				{
					out << base_stations[j].distance(switching_centers[k]) * BS_SC_cost << " ";
				}

				out << endl;
			}
			j++;
		}


		//demand points generation
		int i = 0;

		for (int i = 0; i < point_num; )
		{
			demand_points[i].x = dist(generator);
			demand_points[i].y = dist(generator);
			if (!demand_points[i].inRadius(base_stations))
			{
				continue;
			}
			for (int j = 0; j < BS_num; j++)
			{
				out << demand_points[i].distance(base_stations[j]) << " ";
			}
			out << endl;
			i++;
		}


		SC_old_num -= sc_old_add;
		SC_new_num -= sc_new_add;
		BS_old_num -= bs_old_add;
		BS_new_num -= bs_new_add;
		point_num -= point_add;
		out.close();

		//cout << "out" << no << endl;
	}
}