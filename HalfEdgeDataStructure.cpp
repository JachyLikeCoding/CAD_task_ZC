#include "pch.h"
#include "HalfEdgeDataStructure.h"

void Point::setPoint(double x, double y, double z)
{
	coord[0] = x;
	coord[1] = y;
	coord[2] = z;
}

void Point::setPoint(Point point)
{
	coord[0] = point.coord[0];
	coord[1] = point.coord[1];
	coord[2] = point.coord[2];
}

void Solid::addEdgeToSolid(Edge * edge, Solid *&solid)
{
	if (solid->sedges == nullptr) {	//Ê×±ß
		solid->sedges = edge;
	}
	else {	
		Edge *eg = solid->sedges;
		while (eg->nexte != nullptr) {
			eg = eg->nexte;
		}
		eg->nexte = edge;
		edge->preve = eg;
	}
	solid->edgeCount++;
}

void Solid::addFaceToSolid(Face * newface, Solid *&solid)
{
	if (solid->sfaces == nullptr) {
		solid->sfaces = newface;
	}
	else {
		Face *f = solid->sfaces;
		while (f->nextf != nullptr) {
          	f = f->nextf;
		}
		f->nextf = newface;
		newface->nextf = nullptr;
		newface->prevf = f;
	}
	solid->faceCount++;
	solid->fList.push_back(newface);
}

void Solid::deleteFaceFromSolid(Face * face, Solid *&solid)
{
	Face *tmpface = solid->sfaces;
	if (tmpface == face) {
		solid->sfaces = face->nextf;
		solid->faceCount--;
	}
	else {
		while (tmpface != nullptr && tmpface != face) {
			tmpface = tmpface->nextf;
		}
		tmpface->prevf->nextf = tmpface->nextf;
		tmpface->nextf->prevf = tmpface->prevf;
		solid->faceCount--;
	}
}

void Solid::deleteEdgeFromSolid(Edge * edge, Solid *&solid)
{
	Edge *tmpeg = solid->sedges;
	if (tmpeg == edge) {
		solid->sedges = edge->nexte;
		solid->edgeCount--;
	}
	else {
		while (tmpeg != nullptr && tmpeg != edge){
				tmpeg = tmpeg->nexte;
		}
		tmpeg->preve->nexte = tmpeg->nexte;
		tmpeg->nexte->preve = tmpeg->preve;
		solid->edgeCount--;
	}
}


void Loop::updateHalfEdgeOfLoop(Loop *&loop)
{
	HalfEdge *he = loop->ledge;
	if (he == nullptr) {
		return;
	}
	do {
		he->wloop = loop;
		he = he->nxt;
	} while (he != loop->ledge);
}

void Face::addLoopToFace(Loop * lp)
{
	lp->lface = this;
	if (this->floops == nullptr) {
		this->floops = lp;
	}
	else {
			this->inloops.push_back(lp);
	}
}
