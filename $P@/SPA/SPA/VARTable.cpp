#pragma once
#include "StdAfx.h"
#include "VARTable.h"
#include "SPAException.h"

/**
* This method will be used as a constructor to create VarTable
*/
VARTable::VARTable()
{
}

/**
* This method will be use to add variable into the variable table
* @param VARName the variable being added
* @return The index of the variable in the var table
*/
VARIndex VARTable::insertVAR(string VARName)
{
	for(int i=0;i<vars.size();i++)
	{
		if(vars[i] == VARName)
			return i;
	}
	vars.push_back(VARName);
	return (vars.size() -1);
}

/**
* This method will be used to get the amount of variable in the variable table
* @return The size of the  var table
*/
int VARTable::getSize()
{
	return vars.size();
}

/**
* This method will be used to return the string of the variable at index i in the variable table
* @param i the index of the variable being requested for
* @return the actual string of the variable at index i
*/
string VARTable::getVARName(VARIndex i)
{
	if (i > vars.size() - 1){
		throw SPAException("Invalid Index, there is no variable at given location ");
	}
	else{
		return vars.at(i);
	}
}

/**
* This method will be used return the index of the VARName variable in the variable table
* @param VARName the name of the variable being requested for
* @return The index of the variable in the var table
*/
VARIndex VARTable::getVARIndex(string VARName)
{
	for(int i=0;i<vars.size();i++)
	{
		if(vars[i] == VARName)
			return i;
	}
	
	return -1;
}

/**
* This method will be used to check if a index is a valid variable table index
* @param i the index of the variable table which is being checked
* @return true if the variable index exists, false otherwise
*/
bool VARTable::isExists(VARIndex i)
{
	return (i >= 0 && i < vars.size());
}
