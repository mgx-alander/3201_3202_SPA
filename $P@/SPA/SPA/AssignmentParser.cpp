#pragma once
#include "StdAfx.h"
#include "AssignmentParser.h"
#include "PKB.h"
#include "Helper.h"

int AssignmentParser::getOperatorWeight(string token)
{
	if (token == "+" || token == "-") //+ and - has weight 1
		return 1; 
	// * has weight 2, expandable for divide too, 
	// for expotential just make it to has weight 3 but that is out of scope for simple
	// It will become COMPLICATED =(
	else if (token == "*") 
		return 2;
	else
		return 0; //else weightless
}

// Test if token is an operator    
bool AssignmentParser::isOperator(string token)    
{    
    return (token == "+" || token == "-" ||  token == "*");
}    
  
int AssignmentParser::compareOprPrecedence(string opr1, string opr2)    
{    
	 if (opr1 == ";" || opr2 == ";")
		return -1;
	 else
		return getOperatorWeight(opr1) - getOperatorWeight(opr2);
}

//Some References
//http://www.engr.mun.ca/~theo/Misc/exp_parsing.htm
//http://www.technical-recipes.com/2011/a-mathematical-expression-parser-in-java/

/**
* Process the mathematical expression to form an expression sub tree 
* @param expr	The Mathematical expression to parse
* @return The Expression Sub Tree
*/
ASTExprNode* AssignmentParser::processAssignment(MathExpression expr)
{
	if(expr.at(expr.size()-1) !=";")
		expr.push_back(";");
	

	stack<string> operators, subExprBrackets; 
	stack<ASTExprNode*> operands;

	vector<string> subExpr;

	//before we actually build the tree, check that if it is even valid, if it is not valid
	//then there is no point going thru the shunting yard algorithm
	if (!AssignmentParser::isValidExpr(expr)){
		string msg;
		for (unsigned int i = 0; i < expr.size(); i++) {
			msg += expr.at(i);
		}
		throw SPAException(msg + " is an invalid expression");
	}
	//expr.push_back("+");
	for (unsigned int i = 0; i < expr.size(); i++ ) {
		string token = expr[i]; 
		if (token == "/" || token == "^" || token == "%")
			throw SPAException("Operator not supported, use '+', '-' or '*' only");
		
		if (token == ";") //terminating sequence for assignment
			{
				if (operators.size() >= 1 && operands.size() >= 2 &&
					AssignmentParser::compareOprPrecedence("+", operators.top()) != 0)	{
				
				ASTExprNode* oprNode = new ASTExprNode(ASTNode::Operator, operators.top());
					operators.pop();

					ASTExprNode* rightChild = operands.top();
					operands.pop();
					
					ASTExprNode* leftChild = operands.top();
					operands.pop();

					oprNode->addChild(leftChild, 1);
					oprNode->addChild(rightChild, 2);

					operands.push(oprNode);
					//operators.push(token);
				}
				break; 
		}

		if (token == ")" && subExprBrackets.size() == 0)
			throw SPAException("Please check your expression, additional brackets are found!");

		if (token == "(" && subExprBrackets.size() == 0){ 	//Bracket For Shunting Yard
			subExprBrackets.push(token);

		} else if (subExprBrackets.size() > 0){
			if (token == "("){
				subExprBrackets.push(token);
				subExpr.push_back(token);

			} else if (token == ")") {
				
				subExprBrackets.pop();

				if (subExprBrackets.size() > 0) {
					subExpr.push_back(token);

				} else if (subExprBrackets.size() == 0) {
					subExpr.push_back(";");
					operands.push(AssignmentParser::processAssignment(subExpr)); //recursive call here
					subExpr.clear();
				}
			} else {
				subExpr.push_back(token);
			}

		} else if (AssignmentParser::isOperator(token)) {//+
			if (operators.empty()) {//if the operator stack is empty simply push
				operators.push(token);
			
			} else { //Compare the precedence of + with the top of the stack 
				if (AssignmentParser::compareOprPrecedence(token, operators.top()) > 0)	{

					operators.push(token); //if it is greater, push
				
				} else { //else pop and form a sub tree
					
					ASTExprNode* oprNode = new ASTExprNode(ASTNode::Operator, operators.top());
					operators.pop();

					ASTExprNode* rightChild = operands.top();
					operands.pop();
					
					ASTExprNode* leftChild = operands.top();
					operands.pop();

					oprNode->addChild(leftChild, 1);
					oprNode->addChild(rightChild, 2);

					operands.push(oprNode);
					operators.push(token);
				}
			}
		} else { //it is an operand

			if (Helper::isNumber(token)){ //Check that it is a constant, if it is create a constant node and push it into operand stack
				int value = atoi(token.c_str());
				ASTExprNode* constNode = new ASTExprNode(ASTNode::Constant, value);
				operands.push(constNode);

			} else { //not a const, then must be variable...

				VAR i = PKB::variables.getVARIndex(token);
				if (i == -1){ //if variable cannot be found then error
					//cout << token << " cannot be found " << endl;
					throw SPAException("Variable cannot be found in assignment statement!");
				} else { //create an variable node and push into operand stack
					ASTExprNode* varNode = new ASTExprNode(ASTNode::Variable, i);
					operands.push(varNode);
				}
			}

			//at this pt ok
			//if(prev was a *)
		}
	}
	
	//Build the complete right sub tree to be returned to assign node

	//operators.pop();

	stack<string> operatorst; 
	stack<ASTExprNode*> operandst;
	while (!operators.empty()) 
	{
		string g= operators.top();
		operatorst.push(g);
		operators.pop();
	}
	operators = operatorst;
	while (!operands.empty()) 
	{
		ASTExprNode* a = operands.top();
		operandst.push(a);
		operands.pop();
	}
	operands=operandst;

	while (!operators.empty()) {
		ASTExprNode* oprNode = new ASTExprNode(ASTNode::Operator, operators.top());
					
		ASTExprNode* leftChild = operands.top();
		operands.pop();
					
		ASTExprNode* rightChild = operands.top();
		operands.pop();

		oprNode->addChild(leftChild, 1);
		oprNode->addChild(rightChild, 2);

		operands.push(oprNode);
		operators.pop();
	}
	return operands.top();
}




/**
* This method will be used to determine wheather an expression is a valid mathematical expression
* @param expr the math expression being tested
*/
bool AssignmentParser::isValidExpr(MathExpression expr)
{
	stack<string> brackets;
	int expect = 0; //0=constant or var, 1 = opr

	for(size_t i=0; i<expr.size(); i++) {
		string token = expr.at(i);
		
		if (token == ";" && i == expr.size() - 1)  //terminating sequence for assignment
			break;

		else if (token == ";" && i < expr.size() - 1)
			throw SPAException("Terminating Sequence found in the middle of mathematical expression");
			
		else if(expect == 1 &&  token == "(")
			expect = 0;
		
		else if(expect == 0 &&  token == ")")
			return false;
		
		if(token == "("){
			brackets.push(token);

		} else if (token == ")") {
			if(brackets.size() == 0 || brackets.top() != "(")
				return false;
			brackets.pop();

		} else {
			if(AssignmentParser::isOperator(token)) {
				if(expect == 0)
					return false;
				expect = 0;
			} else {//var or constant
				if(expect == 1)
					return false;
				expect = 1;
			}
		}
	}
	return (brackets.size() == 0);

}