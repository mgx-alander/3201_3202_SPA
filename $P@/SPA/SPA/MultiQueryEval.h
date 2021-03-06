#pragma once
#include "stdafx.h"
#include "RulesOfEngagement.h"
#include "SynonymTable.h"
#include "DisjointSet.h"
#include "ASTNode.h"
#include "ASTExprNode.h"

class MultiQueryEval
{
private:
	MultiQueryEval(const string& query);
	string getToken();
	string getToken2();
	bool matchToken(const string& match, bool error);

	void validate();
	void optimise();
	void evaluate(list<string>& results);

	//MultiQueryEval(const string& query, list<string>& results);

	bool selectBOOLEAN;
	bool died;
	bool earlyQuit;
	size_t pos;
	const string query;
	
	//SYMBOL MEANINGS
	//< >: a map,		< [ ], [] >: a map with 2 elements
	//[ ]: a mapping,	[ a, b ]: a mapping from a to b
	//{ }: a set,		{ a, b, c}: a set with 3 elements

	unordered_map<RulesOfEngagement::QueryRelations, unordered_map<string,
		unordered_set<string>>> relationStore;
	//< [Follows, < [s1, {s2, s3}], [s2, {s3}] > ], [Modifies, < [s1, {v1, v2}], [s2, {v2, v3}] > ] >

	unordered_map<string, unordered_map<string, string>> conditionStore;
	//< [ca, < ["stmt#", 10], ["procName", "hi"] >, [s, < ["stmt#", 5] > ] >

	unordered_map<string, unordered_map<string, unordered_map<string, string>>> condition2Store;
	//< [ca, < ["stmt#", < [s2, "stmt#"], [co, "value"] > ], ["procName", < [p1, "procName"], [p2, procName"] > ] > ] >

	unordered_map<string, unordered_set<string>> patternAssignUsesStore;
	//< [a1, {_"x"_, _"a+b"_} ], [a2, {"x", "y"}] >
	
	unordered_map<string, RulesOfEngagement::QueryVar> stringToQueryVar;
	unordered_map<string, int> stringCount;
	unordered_set<string> mustHandle;
	vector<pair<string, string>> selects;
	unordered_map<string, unordered_set<string>> selectsSet;
	
	SynonymTable synonymTable;
	DisjointSet dsSynonym;
	DisjointSet dsAlias;
	unordered_map<string, string> aliasMap;
	//list of condition, with double synonyms
	vector<RulesOfEngagement::Condition> conditionsList;

public:
	static void evaluateQuery(const string& query, list<string>& results);
};