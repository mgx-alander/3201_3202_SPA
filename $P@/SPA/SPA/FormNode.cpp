#include "FormNode.h"
#include <sstream>

/**
* This method is the constructor of FormNode
* @param nodeType the type of the node
* @param value the value of formNode
*/
FormNode::FormNode(FormType nodeType, int value)
{
	
	isneg = false;
	this->value = value;

}

/**
* This method is used to insert query into a list
* @param f the formNode to be used
*/
void FormNode::getVect(std::vector<FormNode*>* f)
{
	if(this->fType == FormNode::query)
		f->push_back(this);
	else
	{
		//not a query so do left and rigt
		this->children.at(0)->getVect(f);
		this->children.at(1)->getVect(f);
	}
}

/**
* This method is used to insert all the 'query' and 'or' into a list
* @param f the formNode to be used
*/
void FormNode::getVect_break_or(std::vector<FormNode*>* f)
{
	if(this->fType == FormNode::query)
		f->push_back(this);
	else if(this->value == 0)
	{
		f->push_back(this);
	}
	else
	{
		//not a query so do left and rigt
		this->children.at(0)->getVect(f);
		this->children.at(1)->getVect(f);
	}
}

/**
* This method is used to check whether all operator is the same
* @return true if all operator is the same else false
*/
bool FormNode::allOpsSame()
{
	if(this->fType==FormNode::Operator)
	{
		if(this->isneg)
			return false;

		return (this->children.at(0)->allOpsSame(this->value) && this->children.at(1)->allOpsSame(this->value));
	}
	else
		return true;
}

/**
* This method is used to check whether all operator is the same
* @param i next index to check
* @return true if all operator is the same else false
*/
bool FormNode::allOpsSame(int i)
{
	if(this->fType==FormNode::Operator)
	{
		if(this->isneg)
			return false;

		if(this->value != i)
			return false;
		return (this->children.at(0)->allOpsSame(i) && this->children.at(1)->allOpsSame(i));
	}
	else
		return true;
}

/**
* This method is used to get those that is not string
* @param qry
* @return list of string that is not a string type
*/
std::vector<std::vector<std::string>>* FormNode::GetNotStringVect(std::vector<std::string>* qry)
{
	std::vector<std::vector<std::string>>* str = new std::vector<std::vector<std::string>>();
	GetNotStringVect(str,qry);
	return str;
	
}

/**
* This method is used to insert all the not string into a list
* @param qry the qry to be used
*/
void FormNode::GetNotStringVect(std::vector<std::vector<std::string>>*  str,std::vector<std::string>* qry)
{
	string isnot="";

	//if(this->isneg)
	{
		//isnot = "!";
	}
	if(this->fType==FormNode::Operator && this->value == 0)
	{
		vector<string>* temp = new std::vector<string>();
		
		this->children.at(0)->printNotNoBracket(qry,temp);
		this->children.at(1)->printNotNoBracket(qry,temp);
		str->push_back(*temp);
	}
	else if(this->fType==FormNode::Operator && this->value == 1)
	{
		this->children.at(0)->GetNotStringVect(str,qry);
			this->children.at(1)->GetNotStringVect(str,qry);
	}
	else
	{
		vector<string>* temp = new std::vector<string>();

		if(this->isneg == true)
		temp->push_back(qry->at(this->value));

		str->push_back(*temp);
	}
}

/**
* This method is used to print Not no Bracket
* @param qry the qry to be used
* @param storage to store the Not Query
*/
void FormNode::printNotNoBracket(vector<string>* qry,vector<string>* storage)
{

	string isnot="";


	//if(this->isneg)
	{
	//	isnot = "!";
	}
	if(this->fType==FormNode::Operator && this->value == 0)
	{
		
			this->children.at(0)->printNotNoBracket(qry,storage);
			this->children.at(1)->printNotNoBracket(qry,storage);
	}
	else if(this->fType==FormNode::Operator && this->value == 1)
	{
		
		this->children.at(0)->printNotNoBracket(qry,storage);
			this->children.at(1)->printNotNoBracket(qry,storage);
	}
	else
	{
		if(this->isneg == true)
		storage->push_back(qry->at(this->value));
		
	}

	
}

/**
* This method is used to get the List of String from query
* @param qry the qry to be used
* @return list of string
*/
std::vector<std::string>* FormNode::GetStringVect(std::vector<std::string>* qry)
{
	std::vector<std::string>* str = new std::vector<std::string>();
	GetStringVect(str,qry);
	return str;
	
}

/**
* This method is used to get the pruned List of String from query
* @param qry the qry to be used
* @return list of string
*/
std::vector<std::string>* FormNode::GetStringVectPruned(std::vector<std::string>* qry)
{
	std::vector<std::string>* str = new std::vector<std::string>();
	GetStringVectPruned(str,qry);
	return str;
	
}

/**
* This method is used to insert the List of String from query
* @param qry the qry to be used
*/
void FormNode::GetStringVect(std::vector<std::string>* str,std::vector<std::string>* qry)
{
	string isnot="";

	//if(this->isneg)
	{
		//isnot = "!";
	}
	if(this->fType==FormNode::Operator && this->value == 0)
	{
		string temp = isnot+(""+this->children.at(0)->printNoBracket(qry) +  " such that " + this->children.at(1)->printNoBracket(qry)+"");
		str->push_back(temp);
	}
	else if(this->fType==FormNode::Operator && this->value == 1)
	{
		this->children.at(0)->GetStringVect(str,qry);
			this->children.at(1)->GetStringVect(str,qry);
	}
	else
	{
		

		str->push_back(isnot+qry->at(this->value));
	}
}

/**
* This method is used to insert the pruned List of String from query
* @param qry the qry to be used
*/
void FormNode::GetStringVectPruned(std::vector<std::string>* str,std::vector<std::string>* qry)
{
	string isnot="";

	//if(this->isneg)
	{
		//isnot = "!";
	}
	if(this->fType==FormNode::Operator && this->value == 0)
	{
		string s1 = this->children.at(0)->printNoBracketPruned(qry);
		string s2 = this->children.at(1)->printNoBracketPruned(qry)+"";

		if(s1 == "" && s2 =="")
		{
			str->push_back("");
		}
		else if(s1 == "")
		{
			str->push_back(s2);
		}
		else if(s2=="")
		{
			str->push_back(s1);
		}
		else{
		string temp = isnot+""+ s1+  " such that " +s2 ;
		str->push_back(temp);
		}
	}
	else if(this->fType==FormNode::Operator && this->value == 1)
	{
		this->children.at(0)->GetStringVectPruned(str,qry);
			this->children.at(1)->GetStringVectPruned(str,qry);
	}
	else
	{
		
		if(!this->isneg)
		str->push_back(isnot+qry->at(this->value));
		else
		{
			str->push_back("");//removeBind(isnot+qry->at(this->value)));
		}
	}
}

/**
* This method is used to print the list of FormNode
* @return string to be print out
*/
string FormNode::print() 
{
	
	string isnot="";

	if(this->isneg)
	{
		isnot = "!";
	}
	if(this->fType==FormNode::Operator && this->value == 0)
	{
		return isnot+("("+this->children.at(0)->printNoBracket() +  " such that " + this->children.at(1)->printNoBracket()+")");
	}
	else if(this->fType==FormNode::Operator && this->value == 1)
	{
		return isnot+(""+this->children.at(0)->print() +  " or " + this->children.at(1)->print()+"");
	}
	else
	{
		std::ostringstream asd;   // stream used for the conversion

		asd << (this->value);      // insert the textual representation of 'Number' in the characters in the stream

		string Result = asd.str();
		return isnot+Result;
	}
}

/**
* This method is used to print the list of NoBracket of FormNode
* @return string to be print out
*/
string FormNode::printNoBracket()
{

	string isnot="";


	if(this->isneg)
	{
		isnot = "!";
	}
	if(this->fType==FormNode::Operator && this->value == 0)
	{
		return isnot+(""+this->children.at(0)->printNoBracket() +  " such that " + this->children.at(1)->printNoBracket()+"");
	}
	else if(this->fType==FormNode::Operator && this->value == 1)
	{
		return isnot+(""+this->children.at(0)->printNoBracket() +  " or " + this->children.at(1)->printNoBracket()+"");
	}
	else
	{
		std::ostringstream asd;   // stream used for the conversion

		asd << (this->value);      // insert the textual representation of 'Number' in the characters in the stream

		string Result = asd.str();
		return isnot+Result;
	}
}

/**
* This method is used to remove bound of a query
* @param str string to be used
* @return processed String
*/
string FormNode::removeBind(std::string str)
{
	if(str.find("with",0) != -1)
	{
		string t = str.substr(0,str.find("with",0));
		str = t;
		//return str.substr(0,str.find("with",0));
	}
	string temp = str;

	int index =0;
	vector<string> tpr;

	while(index<temp.size())
	{
		int end = temp.find_first_of("() ",index);
		
		
		string sub = temp.substr(index,end-index);

		char found = temp[end];

		if(found == '(')
				tpr.push_back("(");
			else if(end == ')')
				tpr.push_back(")");
		if(end == -1)
		{
			
			tpr.push_back(temp.substr(index,temp.size()-index));
			
			break;
		}
		
		index = end+1;
		if(sub != "")
		tpr.push_back(sub);
	}
	vector<string> cleared;
	bool start = false;
	string first="";
	string second ="";
	bool firstclause = true;

	bool instr = false;
	int substr = 0;
	
	for(int i=0;i<tpr.size();i++)
	{
		if(tpr.at(i) == "\"")
			instr = !instr;

		if(tpr.at(i) == "(" && !instr)
		{
			if(substr == 0)
				substr = i;
			start = true;
			continue;
		}
		if(tpr.at(i) == ")" && !instr)
			start = false;

		if(start == true)
		{
			if(tpr.at(i) == ",")
			{
				firstclause = false;
				continue;
			}
			if(firstclause)
				first = first + tpr.at(i);
			else
				second = second + tpr.at(i);
			cleared.push_back(tpr.at(i));
		}
	}

	string finalq = "";
	int ii=0;
	while(ii<substr)
	{
		finalq = finalq + tpr.at(ii);
		ii++;
	}

	if(Helper::isNumber(first))
		first = "_";
	else if(first.at(0) == '"' && first.at(first.size()-1) == '"')
		first = "\"_\"";

	if(Helper::isNumber(second))
		second = "_";
	else if(second.at(0) == '"' && second.at(second.size()-1) == '"')
		second = "\"_\"";
	
	finalq = finalq  +"("+first+","+second+")";
	
	
	//loop thru look for int or wat

	return finalq;
}

/**
* This method is used to remove bound of a query
* @param qry string to be used
* @return processed String
*/
string FormNode::printNoBracketPruned(vector<string>* qry)
{

	
	//if(this->isneg)
	{
		//isnot = "!";
	}
	if(this->fType==FormNode::Operator && this->value == 0)
	{
		string s1 = this->children.at(0)->printNoBracketPruned(qry);
		string s2 = this->children.at(1)->printNoBracketPruned(qry);

		if(s1=="" && s2=="")
		{return "";
		}
		else if(s1=="")
		{
			return s2;
		}
		else if(s2=="")
		{
			return s1;
		}
		else
		return (""+ s1+  " such that " + s2+"");
	}
	else if(this->fType==FormNode::Operator && this->value == 1)
	{
		string s1 = this->children.at(0)->printNoBracketPruned(qry);
		string s2 = this->children.at(1)->printNoBracketPruned(qry);
		return (""+s1 +  " or " +s2 +"");
	}
	else
	{
		//std::ostringstream asd;   // stream used for the conversion

		//asd << (this->value);      // insert the textual representation of 'Number' in the characters in the stream

		//string Result = asd.str();
		if(!this->isneg)
			return qry->at(this->value);
		else
			return "";//removeBind(isnot+qry->at(this->value));
	}
}

/**
* This method is used to remove bound of a query
* @param qry string to be used
* @return processed String
*/
string FormNode::printNoBracket(vector<string>* qry)
{

	string isnot="";


	//if(this->isneg)
	{
	//	isnot = "!";
	}
	if(this->fType==FormNode::Operator && this->value == 0)
	{
		return isnot+(""+this->children.at(0)->printNoBracket(qry) +  " such that " + this->children.at(1)->printNoBracket(qry)+"");
	}
	else if(this->fType==FormNode::Operator && this->value == 1)
	{
		return isnot+(""+this->children.at(0)->printNoBracket(qry) +  " or " + this->children.at(1)->printNoBracket(qry)+"");
	}
	else
	{
		//std::ostringstream asd;   // stream used for the conversion

		//asd << (this->value);      // insert the textual representation of 'Number' in the characters in the stream

		//string Result = asd.str();
		return isnot+qry->at(this->value);
	}
}

/**
* This method is clone the formNode
* @return Cloned FormNode
*/
FormNode* FormNode::Clone()
{
	FormNode* f;

	f = new FormNode(this->fType,this->value);
	f->children = this->children;

	f->isneg = this->isneg;

	
	return f;


}

/**
* This method is for negation
*/
void FormNode::negate()
{
	this->isneg = !this->isneg;
}

/**
* This method is the constructor of FormNode
* @param nodeType the type of the node
* @param value the value of formNode
*/
FormNode::FormNode(FormType nodeType, string value)//std::vector<std::string> value)
{
	isneg = false;
	this->fType = nodeType;
	if(nodeType == FormType::query)
	{
		string temp = value.replace(value.find("stub"), 4, "");
		this->value = atoi(temp.c_str());
		//string temp = value.replace("stub","");
		return;
	}
	if(value.compare("and")==0)
	{
		this->value = 0;
	}
	else if(value.compare("or")==0)
	{
		this->value = 1;
	}
	else if(value.compare("st")==0)
	{
		this->value = 0;
	}

}

/**
* This method is used to deconstruct
*/
FormNode::~FormNode(void)
{
}

/**
* Add a child node to the FormNode, 
* @param c the child node to add
* @return its reference
*/
FormNode* FormNode::addChild(FormNode* c, int childLoc)
{
	
			if (children.size() == 0 && childLoc == 1) {
				children.push_back(c);
			}
			else if (children.size() == 1 && childLoc == 2) {
				children.push_back(c);
			}
			else if (children.size() == 0 && childLoc == 2) {
				throw SPAException("Invalid Operation: First child must be added first before adding the second!");
			}
			else {//do replacement
				//http://www.java2s.com/Tutorial/Cpp/0320__vector/Insertelementbyindex.htm
				vector<FormNode*>::iterator itr;
				itr = children.begin() + (childLoc - 1);
				*itr = c;
			
			}
	
	return this;
}