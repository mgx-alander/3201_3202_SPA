#pragma once
#include "stdAfx.h"
#include "PKB.h"
#include "CFGNode.h"

class PQLNextProcessor
{
private:
	

public:
	static bool isNext(PROG_LINE p1, PROG_LINE p2);
	static vector<PROG_LINE> getNext(PROG_LINE p1);
	static vector<PROG_LINE> getPrevious(PROG_LINE p2);

	static bool isNextStar(PROG_LINE p1, PROG_LINE p2);
	static vector<PROG_LINE> getPreviousStar(PROG_LINE p2);
	static vector<PROG_LINE> getNextStar(PROG_LINE p1);
	static vector<PROG_LINE> getSelfNextStar();

	static bool isNextBip(PROG_LINE p1, PROG_LINE p2);
	static vector<PROG_LINE> getNextBip(PROG_LINE p1);
	static vector<PROG_LINE> getPreviousBip(PROG_LINE p2);

	static bool isNextBipStar(PROG_LINE p1, PROG_LINE p2);
	static vector<PROG_LINE> getNextBipStar(PROG_LINE p1);
	static vector<PROG_LINE> getPreviousBipStar(PROG_LINE p2);
	static vector<PROG_LINE> getSelfNextBipStar();
};
