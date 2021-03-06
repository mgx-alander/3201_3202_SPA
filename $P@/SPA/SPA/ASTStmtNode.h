#pragma once
#include "AstNode.h"


typedef int Index;

class ASTStmtNode : 
	public ASTNode
{
private:
	int stmtNumber;
	void setValue(Index value);
	void addChildToWhile(ASTNode* c, int);
	void addChildToIF(ASTNode* c, int);
	void addChildToAssign(ASTNode* c, int);

public:
	ASTStmtNode(int stmtNo, NodeType nodeType, Index value);
	int getStmtNumber() const;

	virtual ASTNode* addChild(ASTNode* c);
	virtual ASTNode* addChild(ASTNode* c, int childLoc);
};

