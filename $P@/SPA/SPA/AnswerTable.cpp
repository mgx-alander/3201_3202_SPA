#pragma once
#include "AnswerTable.h"
#include "PKB.h"
#include "Helper.h"

AnswerTable::AnswerTable()
{
}

AnswerTable::AnswerTable(SynonymTable synonymTable, string synonym)
{
	header.push_back(synonym);
	synonymPosition.insert(pair<string, int>(synonym, 0));
	bool unrestricted = true;

	vector<int> table;
	unordered_map<string, string> attributes = synonymTable.getAllAttributes(synonym);
	unordered_set<RulesOfEngagement::QueryReladition> selfReferences = synonymTable.getAllSelfReferences(synonym);
	vector<pair<RulesOfEngagement::QueryReladition, RulesOfEngagement::QueryVar>> firstGeneric =
		synonymTable.getAllFirstGeneric(synonym);
	vector<pair<RulesOfEngagement::QueryReladition, string>> firstSpecific =
		synonymTable.getAllFirstSpecific(synonym);
	vector<pair<RulesOfEngagement::QueryReladition, RulesOfEngagement::QueryVar>> secondGeneric =
		synonymTable.getAllSecondGeneric(synonym);
	vector<pair<RulesOfEngagement::QueryReladition, string>> secondSpecific =
		synonymTable.getAllSecondSpecific(synonym);

	if (attributes.count("stmtNo") > 0) {
		table.push_back(Helper::stringToInt(attributes["stmtNo"]));
		unrestricted = false;
	}

	if (unrestricted && attributes.count("procName") > 0) {
		if (synonymTable.getType(synonym) == RulesOfEngagement::Procedure)
			table.push_back(PKB::procedures.getPROCIndex(attributes["procName"]));
		else if (synonymTable.getType(synonym) == RulesOfEngagement::Calls)
			table = PKB::calls.getStmtCall(
			PKB::procedures.getPROCIndex(attributes["procName"]));
		unrestricted = false;
	}

	if (attributes.count("varName") > 0) {
		table.push_back(PKB::variables.getVARIndex(attributes["varName"]));
		unrestricted = false;
	}

	if (attributes.count("value") > 0) {
		table.push_back(Helper::stringToInt(attributes["value"]));
		unrestricted = false;
	}

	if (unrestricted)
		table = RulesOfEngagement::getType(synonymTable.getType(synonym))();

	for (auto it = selfReferences.begin(); it != selfReferences.end(); it++) {
		RulesOfEngagement::isRelation rel = RulesOfEngagement::getRelation(*it);
		vector<int> table2;
		for (auto it2 = table.begin(); it2 != table.end(); it2++)
			if (rel(*it2, *it2))
				table2.push_back(*it2);
		table = table2;
	}

	for (auto it = firstGeneric.begin(); it != firstGeneric.end(); it++) {
		//to optimise
		RulesOfEngagement::isRelation rel = RulesOfEngagement::getRelation((*it).first);
		vector<int> table2 = RulesOfEngagement::getType((*it).second)();
		vector<int> newTable;
		for (auto it2 = table.begin(); it2 != table.end(); it2++)
			for (auto it3 = table2.begin(); it3 != table2.end(); it3++)
				if (rel(*it2, *it3)) {
					newTable.push_back(*it2);
					break;
				}
		table = newTable;
	}

	for (auto it = firstSpecific.begin(); it != firstSpecific.end(); it++) {
		//to optimise
		RulesOfEngagement::isRelation rel = RulesOfEngagement::getRelation((*it).first);
		int arg = RulesOfEngagement::convertArgumentToInteger((*it).first, false, (*it).second);
		vector<int> newTable;
		for (auto it2 = table.begin(); it2 != table.end(); it2++)
			if (rel(*it2, arg))
				newTable.push_back(*it2);
		table = newTable;
	}

	for (auto it = secondGeneric.begin(); it != secondGeneric.end(); it++) {
		//to optimise
		RulesOfEngagement::isRelation rel = RulesOfEngagement::getRelation((*it).first);
		vector<int> table2 = RulesOfEngagement::getType((*it).second)();
		vector<int> newTable;
		for (auto it2 = table.begin(); it2 != table.end(); it2++)
			for (auto it3 = table2.begin(); it3 != table2.end(); it3++)
				if (rel(*it3, *it2)) {
					newTable.push_back(*it2);
					break;
				}
		table = newTable;
	}

	for (auto it = secondSpecific.begin(); it != secondSpecific.end(); it++) {
		//to optimise
		RulesOfEngagement::isRelation rel = RulesOfEngagement::getRelation((*it).first);
		int arg = RulesOfEngagement::convertArgumentToInteger((*it).first, true, (*it).second);
		vector<int> newTable;
		for (auto it2 = table.begin(); it2 != table.end(); it2++)
			if (rel(arg, *it2))
				newTable.push_back(*it2);
		table = newTable;
	}

	//convert vector<int> to vector<vector<int>>
	for (auto it = table.begin(); it != table.end(); it++) {
		vector<int> temp;
		temp.push_back(*it);
		answers.push_back(temp);
	}
}

void AnswerTable::combine(string ownSynonym, AnswerTable otherTable,
	string otherSynonym, RulesOfEngagement::isRelation rel)
{
	int firstRelIndex = synonymPosition[ownSynonym];
	int secondRelIndex = otherTable.synonymPosition[otherSynonym];

	vector<vector<int>> newTable;
	for (auto it = answers.begin(); it != answers.end(); it++) {
		for (auto it2 = otherTable.answers.begin(); it2 != otherTable.answers.end(); it2++)
			if (rel((*it)[firstRelIndex], (*it2)[secondRelIndex])) {
				vector<int> newRow(*it);
				newRow.insert(newRow.end(), (*it2).begin(), (*it2).end());
				newTable.push_back(newRow);
			}
	}
	answers = newTable;

	for (auto it = otherTable.header.begin(); it != otherTable.header.end(); it++) {
		synonymPosition.insert(pair<string, int>(*it, header.size()));
		header.push_back(*it);
	}
}

void AnswerTable::prune(string firstSynonym,
	string secondSynonym, RulesOfEngagement::isRelation rel)
{
	int firstRelIndex = synonymPosition[firstSynonym];
	int secondRelIndex = synonymPosition[secondSynonym];

	vector<vector<int>> newTable;
	for (auto it = answers.begin(); it != answers.end(); it++)
		if (rel((*it)[firstRelIndex], (*it)[secondRelIndex]))
			newTable.push_back(*it);
	answers = newTable;
}

void AnswerTable::patternPrune(string synonym, bool modifiesIsSynonym,
	int modifies, string uses)
{
	int firstRelIndex = synonymPosition[synonym];

	vector<vector<int>> newTable;
	if (modifiesIsSynonym) {
		for (auto it = answers.begin(); it != answers.end(); it++)
			if (RulesOfEngagement::satisfyPattern((*it)[firstRelIndex], (*it)[modifies], uses))
				newTable.push_back(*it);
	} else {
		for (auto it = answers.begin(); it != answers.end(); it++)
			if (RulesOfEngagement::satisfyPattern((*it)[firstRelIndex], modifies, uses))
				newTable.push_back(*it);
	}
	answers = newTable;
}

AnswerTable AnswerTable::project(vector<string> selection)
{
	AnswerTable newTable;

	vector<int> indices;
	for (auto it = selection.begin(); it != selection.end(); it++)
		indices.push_back(synonymPosition[*it]);

	for (auto it = indices.begin(); it != indices.end(); it++) {
		newTable.synonymPosition[header[*it]] = newTable.header.size();
		newTable.header.push_back(header[*it]);
	}
	
	for (auto it = answers.begin(); it != answers.end(); it++) {
		vector<int> newRow;
		for (auto it2 = indices.begin(); it2 != indices.end(); it2++)
			newRow.push_back((*it)[*it2]);
		newTable.answers.push_back(newRow);
	}

	return newTable;
}

void AnswerTable::cartesian(AnswerTable otherTable)
{
	vector<vector<int>> newTable;
	for (auto it = answers.begin(); it != answers.end(); it++) {
		for (auto it2 = otherTable.answers.begin(); it2 != otherTable.answers.end(); it2++) {
			vector<int> newRow(*it);
			newRow.insert(newRow.end(), (*it2).begin(), (*it2).end());
			newTable.push_back(newRow);
		}
	}
	answers = newTable;

	for (auto it = otherTable.header.begin(); it != otherTable.header.end(); it++) {
		synonymPosition.insert(pair<string, int>(*it, header.size()));
		header.push_back(*it);
	}
}

vector<string> AnswerTable::getHeader()
{
	return header;
}

unsigned int AnswerTable::getSize()
{
	return answers.size();
}

vector<int> AnswerTable::getRow(int index)
{
	return answers[index];
}