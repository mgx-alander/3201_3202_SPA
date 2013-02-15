#include "PQLAffectsProcessor.h"
#include "Helper.h"

bool PQLAffectsProcessor::isSatifyAffects(STMT a1, STMT a2)
{
	if (a1 <= 0 || a2 <= 0) return false;
	
	//Get Corresponding ASTNodes / CFGNodes from table..
	ASTStmtNode* a1ASTNode = PKB::stmtRefMap.at(a1).getASTStmtNode();
	ASTStmtNode* a2ASTNode = PKB::stmtRefMap.at(a2).getASTStmtNode();
	CFGNode* a1CFGNode = PKB::stmtRefMap.at(a1).getCFGNode(); 
	CFGNode* a2CFGNode = PKB::stmtRefMap.at(a2).getCFGNode(); 

	if (a1CFGNode->getProcedure() != a2CFGNode->getProcedure()) { //Check if same procedure
		PKB::affects.insertAffects(a1, a2, false);
		return false; 
	}
	if (a1ASTNode->getType() != ASTNode::Assign || a2ASTNode->getType() != ASTNode::Assign) { // return false if either stmt is not assign
		PKB::affects.insertAffects(a1, a2, false);
		return false; 
	}
	VAR modifiedVar = a1ASTNode->getValue(); // get the variable being modified

	if (!(PKB::next.isNextStar(a1, a2) && PKB::uses.isUsedStmt(a2, modifiedVar))) { //if nextStar(a1, a2) and uses(a2, v) does not hold 
		PKB::affects.insertAffects(a1, a2, false);
		return false;
	}

	return true;
}

bool PQLAffectsProcessor::isAffects(STMT a1, STMT a2) {

	if (PQLAffectsProcessor::isSatifyAffects(a1, a2) == false) 
		return false;

	ASTStmtNode* a1ASTNode = PKB::stmtRefMap.at(a1).getASTStmtNode();
	VAR modifiedVar = a1ASTNode->getValue(); // get the variable being modified
	CFGNode* a1CFGNode = PKB::stmtRefMap.at(a1).getCFGNode(); 

	std::queue<CFGNode*> nodesQue;
	set<STMT> visited;

	nodesQue.push(a1CFGNode);

	for(int i=0;i<a1CFGNode->getNextNodes().size();i++) 
		nodesQue.push(a1CFGNode->getNextNodes().at(i));
	
	CFGNode* tempnode = nodesQue.front();
	int selfAffectsCount = 0;

	int i = 0;

	while(nodesQue.size() > 0) {

		if(tempnode->getType() != CFGNode::DummyNode) {//chk not a dummy node
			STMT curr = tempnode->getProgramLines()[i];

			i += 1;

			ASTStmtNode* n = PKB::stmtRefMap.at(curr).getASTStmtNode();
			
			if (visited.find(curr) != visited.end() || selfAffectsCount >= 2)  //Self Affects = at least twice
				continue;
			
			if (curr != a1) {
				visited.insert(curr);
			} else if (curr == a1 && a1 == a2)	{
				selfAffectsCount += 1;
			}

			if (curr == a2) {
				PKB::affects.insertAffects(a1, a2, true);
				return true;
			}

			if (!PKB::next.isNextStar(a1, curr)) //if next* does not hold
				continue;
			
			if (n->getType() == ASTNode::Call) { // the variable is modified along the path
				if (PKB::modifies.isModifiedProc(n->getValue(), modifiedVar)){
					continue;
				}
			}

			if (n->getType() == ASTNode::Assign) {
				if (PKB::modifies.isModifiedStmt(n->getStmtNumber(), modifiedVar)){
					continue;
				}
			}
		}
		
		
		if (tempnode->getType() == CFGNode::DummyNode || i == tempnode->getProgramLines().size() - 1 ) {
			
			vector<CFGNode*> next = tempnode->getNextNodes(); 
			
			for(int i=0; i<next.size(); i++){
				if (next.at(i) != tempnode)
					nodesQue.push(next.at(i));
			}
		
			i = 0;
			nodesQue.pop();
			tempnode = nodesQue.front();
		}
		
	}
	return false;
}

bool PQLAffectsProcessor::isAffectsStar(STMT a1, STMT a2) {
	
	if (!isSatifyAffects(a1, a2)) 
		return false;

	ASTStmtNode* a1ASTNode = PKB::stmtRefMap.at(a1).getASTStmtNode();
	CFGNode* a1CFGNode = PKB::stmtRefMap.at(a1).getCFGNode(); 

	std::queue<CFGNode*> nodesQue;
	set<STMT> visited;

	nodesQue.push(a1CFGNode);

	for(int i=0;i<a1CFGNode->getNextNodes().size();i++) 
		nodesQue.push(a1CFGNode->getNextNodes().at(i));
	
	CFGNode* tempnode = nodesQue.front();

	int i = 0;

	while(nodesQue.size() > 0) {

		if(tempnode->getType() != CFGNode::DummyNode) {//chk not a dummy node
			STMT curr = tempnode->getProgramLines()[i];

			i += 1;

			ASTStmtNode* n = PKB::stmtRefMap.at(curr).getASTStmtNode();
			
			if (visited.find(curr) != visited.end())  //Self Affects = at least twice
				continue;
			
			if (curr != a1) {
				visited.insert(curr);
			} 

			if (!PKB::next.isNextStar(a1, curr)) //if next* does not hold
				continue;
			
			if (PKB::affects.isAffects(a1, curr) && PKB::affects.isAffects(curr, a2)) { //transitive closure check
				PKB::affects.insertAffectsStar(a1, a2, true);
				return true;
			}
		}
		
		
		if (tempnode->getType() == CFGNode::DummyNode || i == tempnode->getProgramLines().size() - 1 ) {
			
			vector<CFGNode*> next = tempnode->getNextNodes(); 
			
			for(int i=0; i<next.size(); i++){
				if (next.at(i) != tempnode)
					nodesQue.push(next.at(i));
			}
		
			i = 0;
			nodesQue.pop();
			tempnode = nodesQue.front();
		}
		
	}
	return false;
}

bool PQLAffectsProcessor::isAffectsBip(STMT a1, STMT a2) {


	return false; 
		
}