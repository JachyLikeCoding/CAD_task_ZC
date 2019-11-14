#include "pch.h"
#include "EularOperation.h"

/*mvfs ---- ���壺����һ���壬һ���棬һ���⻷��һ����*/
Solid *EularOperation::mvfs(Point p, Vertex * &newVertex) {
	solid = new Solid();
	Face *newFace = new Face(solid, solid->fList.size());
	Loop *newLoop = new Loop(newFace, solid->lpList.size());
	newVertex = new Vertex(p, solid->vList.size());
	//solid->sfaces = newFace;
	solid->svertexs = newVertex;
	newFace->floops = newLoop;

	solid->lpList.push_back(newLoop);
	solid->vList.push_back(newVertex);
	//solid->fList.push_back(newFace);

	solid->addFaceToSolid(newFace, solid);
	//solid->loopCount++;
	return solid;
}

/*��� : ����һ���µ㣬ͬʱ����һ�������µ�����һ��������±ߡ�*/
HalfEdge *EularOperation::mev(Point p2, Vertex *v1, Loop *lp) {
	Vertex *v2 = new Vertex(p2, solid->vList.size());
	solid->vList.push_back(v2);
	HalfEdge *he1 = new HalfEdge(v1, v2, lp);
	HalfEdge *he2 = new HalfEdge(v2, v1, lp);

	Edge *edge = new Edge(he1, he2, solid->edgeCount);	
	he1->brotherhe = he2;
	he2->brotherhe = he1;
	he1->edge = he2->edge = edge;

	//��һ���ߵĹ���
	if (lp->ledge == nullptr) {
		he1->nxt = he2;
		he2->nxt = he1;
		he1->prv = he2;
		he2->prv = he1;
		lp->ledge = he1;
	}else {
		HalfEdge *he = lp->ledge;
		while(he->endv != v1){
			he = he->nxt;
		}
		he2->nxt = he->nxt;
		he->nxt->prv = he2;
		he2->prv = he1;
		he1->nxt = he2;
		he->nxt = he1;
		he1->prv = he;
	}

	solid->addEdgeToSolid(edge, solid);

	return he1;
}

/*���� : ����������������һ���±ߣ�ͬʱ����һ���µĻ����µ��档*/
Loop *EularOperation::mef(Vertex *v1, Vertex *v2, Loop *lp) {
	HalfEdge *he1 = new HalfEdge(v1, v2);
	HalfEdge *he2 = new HalfEdge(v2, v1);
	Edge *edge = new Edge(he1, he2, solid->edgeCount);
	Face *newface = new Face(solid, solid->fList.size());
	Loop *newlp = new Loop(newface, solid->lpList.size());
	he1->edge = he2->edge = edge;
	he1->brotherhe = he2;
	he2->brotherhe = he1;

	HalfEdge *tmp_he1, *tmp_he2; 
	HalfEdge *he = lp->ledge;
	//�������µİ�߼��뵽����
	while (he->endv != v1) {
		he = he->nxt;
	}
	tmp_he1 = he;
	while (he->endv != v2) {
		he = he->nxt;
	}
	tmp_he2 = he;

	Face *sweepFace = lp->lface;
	he = he->nxt;
	while (he->endv != v2) {
		he = he->nxt;
	}
	if (he != tmp_he2) {
		sweepFace = newface;
	}

	//��һ�����󻷷ֿ��γ�����С��
	tmp_he1->nxt->prv = he2;
	he2->nxt = tmp_he1->nxt;
	tmp_he1->nxt = he1;
	he1->prv = tmp_he1;
	tmp_he2->nxt->prv = he1;
	he1->nxt = tmp_he2->nxt;
	tmp_he2->nxt = he2;
	he2->prv = tmp_he2;
	
	he1->wloop = lp;
	he2->wloop = newlp;
	lp->ledge = he1;
	newlp->ledge = he2;

 	newlp->updateHalfEdgeOfLoop(newlp);
	solid->lpList.push_back(newlp);
	//solid->loopCount++;//���---------------------------------------------------------------
	//newlp->loop_id = solid->loopCount;//���------------------
	newface->floops = newlp;
	solid->addFaceToSolid(newface, solid);
	solid->addEdgeToSolid(edge, solid);
	//newface->addLoopToFace(newlp);//���-----------------

	if (!isSweepFace) {
		solid->sweepList.push_back(sweepFace);
	}
	return lp;
}

/*�컷:ɾ��һ���ߣ����ɸñ�ĳһ�����ϵ��µ��ڻ�*/
Loop * EularOperation::kemr(Vertex *outv, Vertex *inv, Loop *lp)
{
	HalfEdge *he = lp->ledge;
	HalfEdge *he1, *he2;
	while (he->startv != outv || he->endv != inv) { 
		he = he->nxt; 
	}
	he1 = he;
	while (he->endv != outv || he->startv != inv) {
		he = he->nxt; 
	}
	he2 = he;

	//�������ӹ�ϵ
	he1->nxt->prv = he2->prv;
	he2->prv->nxt = he1->nxt;
	he1->prv->nxt = he2->nxt;
	he2->nxt->prv = he1->prv;

	//ָ�������������
	lp->ledge = he1->prv;
	Face *tmpface = he1->prv->brotherhe->wloop->lface;
	Loop *inlp = new Loop(tmpface, solid->lpList.size());
	inlp->ledge = he2->prv;
	inlp->updateHalfEdgeOfLoop(inlp);
	tmpface->inloops.push_back(inlp);
	//tmpface->addLoopToFace(inlp);
	solid->lpList.push_back(inlp);
	solid->loopCount++;

	//ɾ�������ߺ��������
	solid->deleteEdgeFromSolid(he1->edge, solid);
	delete he1, he2;
	he1 ,he2 = nullptr;

	return lp;
}

void EularOperation::kfmrh(Face * f1, Face * f2)
{
	Loop *loop = f2->floops;
	loop->lface = f1;
	f1->inloops.push_back(loop);
	solid->deleteFaceFromSolid(f2, solid);
	
	delete f2;
	f2 = nullptr;
}

Solid *EularOperation::sweep(double dist, double dir[3])
{
	vector<Face *> sweepList = solid->sweepList;
	Loop *loop;
	HalfEdge *he, *first_uphe, *uphe;
	Vertex *firstv, *upvertex, *prevupv, *nextv, *up;
	Point uppoint;
	std::cout << "sweeplist size: " << sweepList.size() << endl;
	for (int i = 0; i < sweepList.size(); i++) {
		loop = sweepList[i]->floops;
		he = loop->ledge;
		firstv = he->startv;	//firstv = first Vertex of loop
		//firstup = firstv + dv
		uppoint.setPoint(firstv->vcoor[0] + dist * dir[0], firstv->vcoor[1] + dist * dir[1], firstv->vcoor[2] + dist * dir[2]);
		//mev(firstv, firstup, newe)
		first_uphe = mev(uppoint, firstv, loop);
		std::cout << "mev: uppoint" << uppoint;
		std::cout << "		firstv" << firstv->point << endl;
		upvertex = first_uphe->endv;
		prevupv = upvertex;
		he = he->nxt;
		nextv = he->startv;

		while (nextv != firstv) {
			uppoint.setPoint(nextv->vcoor[0] + dist * dir[0], nextv->vcoor[1] + dist * dir[1], nextv->vcoor[2] + dist * dir[2]);
			std::cout <<"mev: uppoint" <<  uppoint;
			std::cout << "		nextv" << nextv->point << endl;
			uphe = mev(uppoint, nextv, loop);
			up = uphe->endv;
			mef(prevupv, up, loop);
			std::cout << "mef: prevupv" << prevupv->point;
			std::cout << "		up" << up->point << endl;
			prevupv = up;
			he = he->nxt;
			nextv = he->startv;
		}
		mef(prevupv, upvertex, loop);
		std::cout << "mef: prevupv" << prevupv->point;
		std::cout << "		upvertex" << upvertex->point << endl;
	}
	return solid;
}


