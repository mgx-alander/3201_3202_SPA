#pragma once
#include "stdAfx.h"
#include "PROCTable.h"
#include "VARTable.h"
#include "ASTNode.h"
#include "ModifiesTable.h"
#include "CallsTable.h"
#include "UsesTable.h"
#include "FollowsTable.h"
#include "ParentTable.h"
#include "CFGNode.h"
#include "NextTable.h"
#include "AffectsTable.h"
#include "StmtRef.h"
#include "RulesOfEngagement.h"

class PKB
{
public:
	static vector<pair<RulesOfEngagement::QueryRelations, int>> sortorder;

	static ASTNode* rootNode;
	static void addToCFGList(CFGNode* start);

	static int maxProgLines;

	static PROCTable procedures;
	static VARTable variables;
	
	static UsesTable uses;
	static ModifiesTable modifies;
	static CallsTable calls;

	static FollowsTable follows;
	static ParentTable parent;

	static AffectsTable affects;
	static NextTable next;

	static vector<ASTNode::NodeType> statementTable;
	static unordered_set<STMT> assignTable;
	
	static unordered_set<STMT> callTable;
	
	static unordered_set<STMT> whileTable;
	
	static unordered_set<STMT> ifTable;
	
	static unordered_set<STMT> statementListTable;
	
	static vector<StmtRef> stmtRefMap;
	static vector<pair<STMT, STMT>> TheBeginningAndTheEnd;
	static unordered_map<int, vector<STMT>> constantsTable;

	static vector<CFGNode*> CFGHeads;
	static vector<CFGNode*> CFGTails;

	//Nodes
	static unordered_map<STMT, ASTNode*> assignNodes;
	static unordered_map<STMT, ASTNode*> callNodes;
	static unordered_map<STMT, ASTNode*> whileNodes;
	static unordered_map<STMT, ASTNode*> ifNodes;
	static unordered_map<ASTNode*, STMT> assignNodesBack;
	static unordered_map<ASTNode*, STMT> callNodesBack;
	static unordered_map<ASTNode*, STMT> whileNodesBack;
	static unordered_map<ASTNode*, STMT> ifNodesBack;
	static unordered_map<ASTNode*, STMT> statementListNodesBack;
	static vector<ASTNode*> statementNodes;

	static vector<vector<ASTNode*>> varNodes;
	static vector<vector<ASTNode*>> oprNodes;
	static unordered_map<int, vector<ASTNode*>> constNodes;
	static vector<ASTNode*> stmtLstNodes;

	static vector<ASTNode*> getNodes(ASTNode::NodeType);

	static vector<ASTNode*> getNodes(ASTNode::NodeType, int);
};

