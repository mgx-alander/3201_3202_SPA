#pragma once
#include "MultiQueryEval.h"

vector<string> MultiQueryEval::evaluateQuery(MultiQueryTree qt)
{
	/*
	Create querytree, which pproduces a query tree on the reladitions and a disjoint set on the variables as they are being read

	sort components in order such that unmarked first, then marked next;*/
	StagingArea stage;
	vector<vector<vector<int>>> bigAnswers;
	bool returnAll = true;

	for (auto it = qt.getTree().begin(); it != qt.getTree().end(); it++)
	{
		QueryCluster cluster = *it;

		for (auto it = cluster.conditions.begin(); it != cluster.conditions.end(); it++) {
			//each synonym should at most have one condition
			set<int> table;
			for (auto it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++) {
				switch ((*it2).first) {
				case QueryEnums::ProcName: //TODO: differentiate between call statement and actual proc
					if((*it2).second < PKB::procedures.getSize());
						table.insert((*it2).second);
					break;
				case QueryEnums::VarName:
					if((*it2).second < PKB::variables.getSize());
						table.insert((*it2).second);
					break;
				case QueryEnums::Value:
					if (PKB::constantsTable.count((*it2).second) > 0)
						table.insert(1);
				case QueryEnums::StmtNo:
					if((*it2).second <= PKB::maxProgLines);
						table.insert((*it2).second);
					break;
				default:
					throw SPAException("Invalid condition type");
				}
			}
			stage.initIndividualTable((*it).first, table);
		}

		for (auto it = cluster.relations.begin(); it != cluster.relations.end(); it++) {
			for (auto it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++) {
				switch ((*it2).first) {
				case QueryEnums::Assign:
					stage.join(
					break;
				case QueryEnums::While:
					for (auto it = PKB::whileTable.begin(); it != PKB::whileTable.end(); it++)
						table.insert(*it);
					break;
				case QueryEnums::If:
					for (auto it = PKB::ifTable.begin(); it != PKB::ifTable.end(); it++)
						table.insert(*it);
					break;
				case QueryEnums::Call:
					for (auto it = PKB::callTable.begin(); it != PKB::callTable.end(); it++)
						table.insert(*it);
					break;
				case QueryEnums::Procedure: {
					int size = PKB::procedures.getSize();
					for (int i = 0; i < size; i++)
						table.insert(i);
					break; }
				case QueryEnums::Stmt: {
					int size = PKB::maxProgLines;
					for (int i = 0; i < size; i++)
						table.insert(i);
					break; }
				case QueryEnums::Variable: {
					int size = PKB::variables.getSize();
					for (int i = 0; i < size; i++)
						table.insert(i);
					break; }
				case QueryEnums::Constant:
					for(auto it = PKB::constantsTable.begin(); it != PKB::constantsTable.end(); it++)
						table.insert((*it).first);
					break;
				case QueryEnums::WildCard:
					break;
				default:
					throw SPAException("Invalid first relationship parameter type");
				}
			}

		}/*
		firstcase(answers, cluster[0]) //modifies(s1,s2) -> vector<IntOrString> <s1,s2> that satisfies me
		done[0] = true;
		while (still have unevaluated reladition in cluster) {
			//get those relatditions (condiitions higher priority) that are in vector<IntOrString> (*), then get reladition where one is in, one is out
			if (last one in unstared && !evaluateStarred(answers, reladition)) {
				returnAll = false;
				break; //fail
			} else {
				evaluateUnStarred(answers, reladition);
				kill that reladition
			}
		}
		if (marked)
			bigAnswers.push_bacK(project(StarredInComponent, answers));*/
	}
	
	/*//cartesian product the answers in bigAnswers
	vector<vector<IntOrString>>* accumulator = &bigAnswers[0];

	if (bigAnswers.size() == 0) {
		if (returnAll) {
			accumulator = getAllVariables(select[0]);
			for (int i = 1; i < select.size(); i++)
				CartesianProduct(answers, getAllVariables(select[i]));
		}
	} else {
		for (int i = 1; i < bigAnswers.size(); i++) {
			vector<vector<IntOrString>> newanswers;
			for (vector<IntOrString> t1 : accumulator)
				for(vector<IntOrString> t2 : bigAnswers[i])
					newanswers.push_back(make_vector<IntOrString>(t1, t2));
			accumulator = &newanswers;
		}
	}

	for (vector<IntOrString> t : accumulator) {
		ostringstream ansss;
		for (entries : t)
			ans << entries << " ";
		string ans = ans.str()
		results.push_back(ans.substr(0, ans.size() - 1));
	}*/

} //TADA!

/*--------------------------
Select <s1,v2,s3> such that modifies(s1,v1) and uses(s2,v2) and modifies(s1,v2) and uses(s3,v3) and modifies(totally,unrelated)
{s1,v1,s2,v2)* , (s3,v3)* , (totally,unrelated)
modifies(s1,v1)
answers = <s1,v1>

uses(s2,v2) (if (answersNameSet.count(s2) > 0 || answersNameSet.count(v2)) > 0 -> false)
modifies(s1,v2) (if (answersNameSet.find(s1) || answersNameSet.find(v2)) -> true)
answers = <s1,v1,v2>

uses(s2,v2)  (if (answersNameSet.find(s2) || answersNameSet.find(v2)) -> true)
answers = <s1,v1,v2,s2>
Example OVER
--------------------------*/

void MultiQueryEval::evaluateStarred(vector<vector<int>> *answers, MultiQueryTree reladition) {
		vector<vector<IntOrString>> newanswers;
		for (ans : answers) {
			vector<type2> toCartesianProduct = getRelation(ans[index1]) //needs to switch
			for (type2 : toCartesianProduct) {
				newanswers.add(flat(new vector<IntOrString>(ans, type2)));
				answersNameVector.push_back(type2);
				answersNameSet.insert(type2);
			}
		}
		answers = newanswer;
	}
}

void MultiQueryEval::bool evaluateUnstarred(vector<vector<IntOrString>> *answers, QueryTreeNode reladition) {
	if (answersNameSet.find(relatdition.first)) && answersNameSet.find(relatdition.second()) {
		int index1 = answersNameVector.find(relatdition.first);
		int index2 = answersNameVector.find(relatdition.secoond);
		for (ans : answers) {
			if satsify reladition(get<index1> ans, get<index2> ans)
				return true;
		}
	} else {
		if (answersName.find(relatdition.first)) {
			int index1 = answersNameVector.find(relatdition.first);
			int index2 = answersNameVector.find(relatdition.second);
		else {
			int index1 = answersNameVector.find(relatdition.second);
			int index2 = answersNameVector.find(relatdition.first);
		}
		for (ans : answers) {
			vector<type2> toCartesianProduct = getRelation(ans[index1])
			if (toCartesianProduct.size() > 0)
				return true;
		}
	}
	return false;
}