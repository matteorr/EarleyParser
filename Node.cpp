/*
 * Node.cpp
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

#include "Node.h"

using namespace std;

Node::Node( Node* father, string grammarElement, vector<string> nodeProduction, Production* productionPointer, float productionProbability )
{
	this->father = father;

	this->grammarElement = grammarElement;
	this->production = nodeProduction;
	this->productionProbability = productionProbability;

	this->productionPointer = productionPointer;

}

Node::~Node()
{
	for( vector<Node*>::iterator it = this->children.begin(); it != this->children.end(); it++ ){
		delete (*it);
	}
}

Production* Node::getProductionPointer()
{
	return this->productionPointer;
}

State* Node::expandNode( State* currentStateToExpand, unordered_map< string, list<Production> >* earleyProductions )
{
	State* nextStateToExpand;

	cout << endl << "currently in expansion of - " << currentStateToExpand->getNonTerminal();
	for( unsigned int i = 0; i < currentStateToExpand->getProductionPointer()->getProductionContent().size(); i++ )
	{
		cout << " " << currentStateToExpand->getProductionPointer()->getProductionContent().at(i) << " ";
	}

	for( unsigned int i = this->production.size(); i > 0; i-- )
	{

/*		cout << endl << "check-------------------------------------------------" << endl;
		cout << endl << "[DEBUG]: production of this node - size: " << this->production.size() << endl;
		for( unsigned int k = 0; k < this->production.size(); k++ )
		{
			cout << this->production.at(k) << " ";
		}
		cout << endl << "[DEBUG]: production of current state to expand - size: " << currentStateToExpand->getProductionPointer()->getProductionContent().size() << endl;
		for( unsigned int k = 0; k < currentStateToExpand->getProductionPointer()->getProductionContent().size(); k++ )
		{
			cout << currentStateToExpand->getProductionPointer()->getProductionContent().at(k) << " ";
		}
		cout << endl << "-------------------------------------------------finish" << endl << endl;

		if( this->productionPointer != currentStateToExpand->getProductionPointer() )
		{
			exit(1);
		}
*/
		unordered_map< string, list<Production> >::iterator got = earleyProductions->find( this->production.at( i - 1 ) );
		if( got != earleyProductions->end() )
		{
			/*
			 * the element contained in the production
			 * at current position of loop is NON TERMINAL
			 * and needs to be expanded
			 */
			if( currentStateToExpand->getBacktraceStateVector()->size() > 1 ){

				cout << endl << "BACKTRACE VECTOR BIGGER THAN 1" << endl;

				for( unsigned j = 0; j < currentStateToExpand->getBacktraceStateVector()->size(); j++ )
				{
					State* state = currentStateToExpand->getBacktraceStateVector()->at(j);
					cout << endl << "[DEBUG]: at 0 -" << state->getNonTerminal() << " size: " << state->getProductionPointer()->getProductionContent().size() << endl;
					for( unsigned int k = 0; k < state->getProductionPointer()->getProductionContent().size(); k++ )
					{
						cout << state->getProductionPointer()->getProductionContent().at(k) << " ";
					}
					cout << endl;
				}

				for( unsigned j = 0; j < currentStateToExpand->getBacktraceStateVector()->size(); j++ )
				{
					if( this->father->getProductionPointer() == currentStateToExpand->getBacktraceStateVector()->at(j)->getProductionPointer() )
					{
						currentStateToExpand = currentStateToExpand->getBacktraceStateVector()->at(j);
						cout << "[DEBUG]: matching production pointer at position " << j << endl;
						break;
					}

					if( j == currentStateToExpand->getBacktraceStateVector()->size() - 1 )
					{
						cout << "[ERROR]: no matching production pointer correspondent to production in this node!" << endl;
						exit(1);
					}
				}

			}else{
				nextStateToExpand = currentStateToExpand->getBacktraceStateVector()->at(0);
			}

			// TODO CHECK THAT this->production.at( i - 1 ) == nextStateToExpand->getNonTerminal()

			Node* child;
			if( this->productionProbability >= 0 )
			{
				child =	new Node(
								this,
								nextStateToExpand->getNonTerminal(),
								nextStateToExpand->getProductionPointer()->getProductionContent(),
								nextStateToExpand->getProductionPointer(),
								nextStateToExpand->getProductionPointer()->getProductionProbability()
							);
			}else{

				child = new Node(
								this,
								nextStateToExpand->getNonTerminal(),
								nextStateToExpand->getProductionPointer()->getProductionContent(),
								nextStateToExpand->getProductionPointer(),
								-1
							);
			}

			/*
			 * insert at beginning of vector
			 */
			this->children.insert( this->children.begin(), child );

			currentStateToExpand = child->expandNode( nextStateToExpand, earleyProductions );

			cout << endl << "finished expanding - " << nextStateToExpand->getNonTerminal() << endl;

		}else{
			/*
			 * the element is TERMINAL
			 * and needs not to be exapanded
			 */

			Node* child;
			if( this->productionProbability >= 0 )
			{
				/*
				 * leaf nodes have probabilities only
				 * if terminal symbols probabilities are
				 * specified in input
				 */

				/*
				 * NULL is inserted because terminal
				 * symbols do not have a production
				 */
				child =	new Node(
								this,
								this->production.at( i - 1 ),
								vector<string>(),
								//nextStateToExpand->getProductionPointer()->getProductionProbability()
								//nextStateToExpand->getProductionPointer(),
								NULL,
								1
							);
			}else{

				child = new Node(
								this,
								this->production.at( i - 1 ),
								vector<string>(),
								//nextStateToExpand->getProductionPointer(),
								NULL,
								-1
							);
			}

			/*
			 * insert at beginning of vector
			 */
			this->children.insert( this->children.begin(), child );

			if( currentStateToExpand->getBacktraceStateVector()->size() > 1 ){

				cout << endl << "BACKTRACE VECTOR BIGGER THAN 1" << endl;

				for( unsigned j = 0; j < currentStateToExpand->getBacktraceStateVector()->size(); j++ )
				{
					State* state = currentStateToExpand->getBacktraceStateVector()->at(j);
					cout << endl << "[DEBUG]: at 0 -" << state->getNonTerminal() << " size: " << state->getProductionPointer()->getProductionContent().size() << endl;
					for( unsigned int k = 0; k < state->getProductionPointer()->getProductionContent().size(); k++ )
					{
						cout << state->getProductionPointer()->getProductionContent().at(k) << " ";
					}
					cout << endl;
				}

				for( unsigned j = 0; j < currentStateToExpand->getBacktraceStateVector()->size(); j++ )
				{
					if( this->father->getProductionPointer() == currentStateToExpand->getBacktraceStateVector()->at(j)->getProductionPointer() )
					{
						currentStateToExpand = currentStateToExpand->getBacktraceStateVector()->at(j);
						cout << "[DEBUG]: matching production pointer at position " << j << endl;
						break;
					}

					if( j == currentStateToExpand->getBacktraceStateVector()->size() - 1 )
					{
						cout << "[ERROR]: no matching production pointer correspondent to production in this node!" << endl;
						exit(1);
					}
				}

			}else{
				currentStateToExpand = currentStateToExpand->getBacktraceStateVector()->at(0);
			}

			if( currentStateToExpand->getBacktraceStateVector()->size() > 1 ){

				cout << endl << "BACKTRACE VECTOR BIGGER THAN 1" << endl;

				for( unsigned j = 0; j < currentStateToExpand->getBacktraceStateVector()->size(); j++ )
				{
					State* state = currentStateToExpand->getBacktraceStateVector()->at(j);
					cout << endl << "[DEBUG]: at 0 -" << state->getNonTerminal() << " size: " << state->getProductionPointer()->getProductionContent().size() << endl;
					for( unsigned int k = 0; k < state->getProductionPointer()->getProductionContent().size(); k++ )
					{
						cout << state->getProductionPointer()->getProductionContent().at(k) << " ";
					}
					cout << endl;
				}

				//for( unsigned j = currentStateToExpand->getBacktraceStateVector()->size() - 1; j >= 0; j-- )
				for( unsigned j = 0; j < currentStateToExpand->getBacktraceStateVector()->size(); j++ )
				{
					if( this->father->getProductionPointer() == currentStateToExpand->getBacktraceStateVector()->at(j)->getProductionPointer() )
					{
						currentStateToExpand = currentStateToExpand->getBacktraceStateVector()->at(j);
						cout << "[DEBUG]: matching production pointer at position " << j << endl;
						break;
					}else{
						cout << "[DEBUG]: no matching production pointer at position " << j << endl;
					}

					if( j == currentStateToExpand->getBacktraceStateVector()->size() - 1/*0*/ )
					{
						cout << "[ERROR]: no matching production pointer correspondent to production in this node!" << endl;
						exit(1);
					}
				}

			}else{
				currentStateToExpand = currentStateToExpand->getBacktraceStateVector()->at(0);
			}
		}
	}

	return currentStateToExpand;
}

float Node::print( bool printProbabilities )
{
	float prob = -1;

	cout << "Node [" << this->grammarElement << "]{";

	if( printProbabilities == true ){

		if( this->children.size() == 0 )
		{
			/*
			 * we are in a leaf node
			 */
			prob = this->productionProbability;

		}else{

			prob = this->productionProbability;
			for( vector<Node*>::iterator it = this->children.begin(); it != this->children.end(); it++ )
			{
				prob = prob * (*it)->print( printProbabilities );
			}
		}
	}else{

		for( vector<Node*>::iterator it = this->children.begin(); it != this->children.end(); it++ )
		{
			prob = (*it)->print( printProbabilities );
		}
	}
	cout << "prob [" << prob << "]}";

	return prob;
}
