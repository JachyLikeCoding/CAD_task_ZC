#pragma once
#ifndef HALF_EDGE_DS_H
#define HALF_EDGE_DS_H

#include<iostream>
#include<vector>
using namespace std;
typedef double Vector[4];
struct Point;	//	几何点
struct Vertex;
struct Edge;
struct HalfEdge;
struct Loop;
struct Face;
struct Solid;


struct Point {
	double coord[3];
	double *getPoint() {
		return coord;
	}
	void setPoint(double x, double y, double z);
	void setPoint(Point point);
	friend inline istream & operator >> (istream & is, Point & point)
	{
		is >> point.coord[0] >> point.coord[1] >> point.coord[2];
		return is;
	}
	friend inline ostream& operator << (ostream & os, Point & point)
	{
		os << "( " << point.coord[0] << ", " << point.coord[1] << ", " << point.coord[2] << " )";
		return os;
	}
};


class Vertex {
public:
	int vertex_id;	//顶点序号
	Vector vcoor;	//	顶点坐标
	Point point;	//几何点
	HalfEdge *vhe;	//指向以该顶点为起点的半边
	Vertex *prevv;	//指向前一个顶点
	Vertex *nextv;	//指向后一个顶点

	Vertex(double x, double y, double z, int _id = 0) :  vertex_id(_id), nextv(nullptr), prevv(nullptr), vhe(nullptr) {
		vcoor[0] = x;
		vcoor[1] = y;
		vcoor[2] = z;
		vcoor[3] = 1;
	}

	Vertex(Point _point, int _id = 0) : vertex_id(_id), nextv(nullptr), prevv(nullptr), vhe(nullptr) {
		vcoor[0] = _point.coord[0];
		vcoor[1] = _point.coord[1];
		vcoor[2] = _point.coord[2];
		vcoor[3] = 1;
		point.setPoint(_point);
	}
};

class  HalfEdge {
public:
	Edge *edge;	//指向半边对应的边
	Vertex *startv;	//指向半边的起点
	Vertex *endv;	//指向半边的终点
	Loop *wloop;	//指向半边所在的环
	HalfEdge *prv;	//指向上一条半边
	HalfEdge *nxt;	//指向下一条半边
	HalfEdge *brotherhe; //兄弟半边
	HalfEdge(Vertex *_startv, Vertex *_endv, Loop *_wloop = nullptr, Edge *_edge = nullptr) : 
		startv(_startv), endv(_endv), wloop(_wloop) ,edge(_edge), brotherhe(nullptr), prv(nullptr), nxt(nullptr) {}
};

class Edge {
public:
	int edge_id;	//边的序号
	HalfEdge *he1;	//指向左半边
	HalfEdge *he2;	//指向右半边
	Edge *preve;	//指向上一条边
	Edge *nexte;	//指向下一条边
	Edge(HalfEdge *_he1, HalfEdge *_he2, int _id) : edge_id(_id), he1(_he1), he2(_he2), preve(nullptr), nexte(nullptr) {}
};

class Loop {
public:
	int loop_id;	//环的序号
	HalfEdge *ledge;	//指向构成环的半边
	Face *lface;	//指向环所在的面
	Loop *prevl;	//指向前一个环
	Loop *nextl;	//指向后一个环
	Loop()  : loop_id(0), ledge(nullptr), lface(nullptr), prevl(nullptr), nextl(nullptr){}
	Loop(Face *_face = nullptr, int _id = 0) : loop_id(_id), ledge(nullptr), lface(_face), prevl(nullptr), nextl(nullptr) {}
	void updateHalfEdgeOfLoop(Loop *&loop);
};

class Face {
public:
	int face_id;		//面序号
	Solid *fsolid;	//指向面所在的体
	Loop *floops;	//指向构成该面的环
	vector<Loop *> inloops;
	Face *prevf;	//指向前一个面
	Face *nextf;	//指向后一个面
	Face() : face_id(0), fsolid(nullptr), floops(nullptr), prevf(nullptr), nextf(nullptr){}
	Face(Solid *_solid = nullptr, int _id = 0) : face_id(_id), fsolid(nullptr), floops(nullptr), prevf(nullptr), nextf(nullptr) {}
	void addLoopToFace(Loop *lp);
};

class Solid {
public:
	int solid_id;	//体序号
	Face *sfaces;	//指向构成该体的面
	Edge *sedges;	//指向构成该体的边
	Vertex *svertexs;	//指向构成该体的顶点
	Solid *prevs;	//指向前一个体
	Solid *nexts;	//指向后一个体
	vector<Vertex *> vList;
	vector<Loop *> lpList;
	vector<Face *> fList;
	vector<Face *> sweepList;
	int edgeCount;
	int faceCount;
	int loopCount;

	Solid(int _id = 0) : solid_id(_id), sfaces(nullptr), sedges(nullptr), svertexs(nullptr), prevs(nullptr), nexts(nullptr){}
	void addEdgeToSolid(Edge *edge, Solid *&solid);
	void addFaceToSolid(Face *face, Solid *&solid);
	void deleteFaceFromSolid(Face * face, Solid *&solid);
	void deleteEdgeFromSolid(Edge *edge, Solid *&solid);
};

#endif // !HALF_EDGE_DS_H
