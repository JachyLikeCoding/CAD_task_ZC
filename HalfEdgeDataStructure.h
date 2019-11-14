#pragma once
#ifndef HALF_EDGE_DS_H
#define HALF_EDGE_DS_H

#include<iostream>
#include<vector>
using namespace std;
typedef double Vector[4];
struct Point;	//	���ε�
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
	int vertex_id;	//�������
	Vector vcoor;	//	��������
	Point point;	//���ε�
	HalfEdge *vhe;	//ָ���Ըö���Ϊ���İ��
	Vertex *prevv;	//ָ��ǰһ������
	Vertex *nextv;	//ָ���һ������

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
	Edge *edge;	//ָ���߶�Ӧ�ı�
	Vertex *startv;	//ָ���ߵ����
	Vertex *endv;	//ָ���ߵ��յ�
	Loop *wloop;	//ָ�������ڵĻ�
	HalfEdge *prv;	//ָ����һ�����
	HalfEdge *nxt;	//ָ����һ�����
	HalfEdge *brotherhe; //�ֵܰ��
	HalfEdge(Vertex *_startv, Vertex *_endv, Loop *_wloop = nullptr, Edge *_edge = nullptr) : 
		startv(_startv), endv(_endv), wloop(_wloop) ,edge(_edge), brotherhe(nullptr), prv(nullptr), nxt(nullptr) {}
};

class Edge {
public:
	int edge_id;	//�ߵ����
	HalfEdge *he1;	//ָ������
	HalfEdge *he2;	//ָ���Ұ��
	Edge *preve;	//ָ����һ����
	Edge *nexte;	//ָ����һ����
	Edge(HalfEdge *_he1, HalfEdge *_he2, int _id) : edge_id(_id), he1(_he1), he2(_he2), preve(nullptr), nexte(nullptr) {}
};

class Loop {
public:
	int loop_id;	//�������
	HalfEdge *ledge;	//ָ�򹹳ɻ��İ��
	Face *lface;	//ָ�����ڵ���
	Loop *prevl;	//ָ��ǰһ����
	Loop *nextl;	//ָ���һ����
	Loop()  : loop_id(0), ledge(nullptr), lface(nullptr), prevl(nullptr), nextl(nullptr){}
	Loop(Face *_face = nullptr, int _id = 0) : loop_id(_id), ledge(nullptr), lface(_face), prevl(nullptr), nextl(nullptr) {}
	void updateHalfEdgeOfLoop(Loop *&loop);
};

class Face {
public:
	int face_id;		//�����
	Solid *fsolid;	//ָ�������ڵ���
	Loop *floops;	//ָ�򹹳ɸ���Ļ�
	vector<Loop *> inloops;
	Face *prevf;	//ָ��ǰһ����
	Face *nextf;	//ָ���һ����
	Face() : face_id(0), fsolid(nullptr), floops(nullptr), prevf(nullptr), nextf(nullptr){}
	Face(Solid *_solid = nullptr, int _id = 0) : face_id(_id), fsolid(nullptr), floops(nullptr), prevf(nullptr), nextf(nullptr) {}
	void addLoopToFace(Loop *lp);
};

class Solid {
public:
	int solid_id;	//�����
	Face *sfaces;	//ָ�򹹳ɸ������
	Edge *sedges;	//ָ�򹹳ɸ���ı�
	Vertex *svertexs;	//ָ�򹹳ɸ���Ķ���
	Solid *prevs;	//ָ��ǰһ����
	Solid *nexts;	//ָ���һ����
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
