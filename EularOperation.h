#pragma once
#ifndef EULAR_OPERATION_H
#define EULAR_OPERATION_H

#include "HalfEdgeDataStructure.h"
#include<vector>
class EularOperation {
public:
	EularOperation() : solid(nullptr) {}
	Solid *mvfs(Point p, Vertex * &v);	//造体：生成一个体，一个面，一个外环，一个点。
	HalfEdge *mev(Point p2, Vertex *v1, Loop *lp);	//造边 : 生成一个新点，同时生成一条连接新点与另一给定点的新边。
	Loop *mef(Vertex *v1, Vertex *v2, Loop *lp);	//造面 : 以两个给定点生成一条新边，同时生成一个新的环、新的面。
	Loop *kemr(Vertex *v1, Vertex *v2, Loop *lp);	//造环:删除一条边，生成该边某一邻面上的新的内环
	void kfmrh(Face *f1, Face *f2);	//造柄
	Solid *sweep(double dist, double dir[3]);
	Solid *getSolid() {
		return solid;
	}
	Solid *setSolid(Solid *_solid) {
		solid = _solid;
		return solid;
	}
	bool isSweepFace;

private:
	Solid *solid;
};





#endif // !EULAR_OPERATION_H
