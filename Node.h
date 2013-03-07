/*
 * Node.h
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

#include "Production.h"
#include "State.h"

#include <unordered_map>
#include <list>

#ifndef NODE_H_
#define NODE_H_

using namespace std;

class Node
{

public:

	Node( Node* father, string grammarElement, vector<string> nodeProduction, Production* productionPointer, float productionProbability );

	virtual ~Node();

	State* expandNode( State* stateToExpandPointer, unordered_map< string, list<Production> >* earleyProductions );

	float print( bool printProbabilities );

	Production* getProductionPointer();

private:

	explicit Node() {}

	string grammarElement;
	vector<string> production;
	Node* father;

	vector<Node*> children;

	float productionProbability;

	Production* productionPointer;

};

#endif /* NODE_H_ */
