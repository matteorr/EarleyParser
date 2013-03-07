/*
 * State.cpp
 *
 *  Created on: Oct 17, 2012
 *      Author: Matteo Ruggero Ronchi
 *
 *  Copyright 2012 Matteo Ruggero Ronchi - matrronchi.license@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "State.h"

using namespace std;

State::State( string nonTerminal, Production* productionPointer, State* backtraceState, int inputStringProgress, int productionProgress )
: productionProgress(0), printed(false)
{

	//cout << endl << endl << "!!!!!!!!!!!![DEBUG]: OTHER CONSTRUCTOR CALLED!!!!!!!!!!!!!" << endl << endl << endl;

	this->nonTerminal = nonTerminal;
	this->productionPointer = productionPointer;

	this->backtraceStateVector.push_back( backtraceState );

	this->inputStringProgress = inputStringProgress;
	this->productionProgress = productionProgress;
}

State::~State()
{
	//cout << endl << endl << "!!!!!!!!!!!![DEBUG]: DESTRUCTOR CALLED!!!!!!!!!!!!!" << endl << endl << endl;
}

vector<State*>* State::getBacktraceStateVector()
{
	return &( this->backtraceStateVector );
}

int State::getInputStringProgress()
{
	return this->inputStringProgress;
}

const string  State::getNonTerminal()
{
	return this->nonTerminal;
}

Production* State::getProductionPointer() const
{
	if( this->productionPointer == NULL ){
		cout << "[ERROR]: production pointer is null" << endl;
		exit(1);
	}

	return this->productionPointer;
}

int State::getProductionProgress() const
{
	return this->productionProgress;
}

bool State::isComplete()
{
	if( this->getProductionProgress() == this->getProductionPointer()->getProductionContent().size() )
		return true;
	else
		return false;
}

string State::nextSymbolToProcess()
{
	return this->getProductionPointer()->getProductionContent().at( this->productionProgress );
}

bool State::getPrinted()
{
	return this->printed;
}

void State::setPrintedTrue()
{
	this->printed = true;
}
