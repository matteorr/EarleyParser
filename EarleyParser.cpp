/*
 * EarleyParser.cpp
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

#include "EarleyParser.h"

using namespace std;

EarleyParser::EarleyParser()
{
	// TODO Auto-generated constructor stub
}

EarleyParser::~EarleyParser()
{
	for( unsigned int i = 0; i <= this->inputVector.size(); i++ )
	{
		//int j = 0;
		//cout << i << "  *****************************************************" << endl << "*****************************************************" << endl;
		for( vector<State*>::iterator it = this->earleyParserChart.at(i).begin(); it != this->earleyParserChart.at(i).end(); it++ )
		{
			//j++;
			delete *it;

		}
		//cout << endl << endl << "DELETED STATEs: " << j << endl << endl;

	}

	for( vector<Node*>::iterator it = this->returnVector.begin(); it != this->returnVector.end(); it++ )
	{
		delete *(it);
	}

}

void EarleyParser::initialize( string filePath, bool probablityParser )
{
	this->probabilityParser = probablityParser;

	string line;
	ifstream inputFile;

	inputFile.open( filePath.c_str(), fstream::in );

	if( inputFile.is_open() )
	{
		while( inputFile.good() )
		{
			/*
			 * reads a line (production) from the context free input grammar
			 */
			getline( inputFile, line );

			//skips empty line
			if( line.size() == 0 ) continue;

			cout << "[DEBUG]: " << line << endl;

			/*
			 * extracts tokens of the production
			 * separated by "->"
			 * first token is terminal symbol
			 * second token is production rule
			 * third tolken is production probability
			 */
			vector< string > tokenVector;
			this->splitToVector( line, &tokenVector, "->", false );

			//skips comment line
			if( tokenVector.at(0) == "#" ) continue;

			for( unsigned int i = 0; i < tokenVector.size(); i++ ){
				cout << "[DEBUG]: " << "line token: [" << tokenVector.at(i) << "]" << endl;
			}

			/*
			 * extracts all symbols in the production
			 * separated by " "
			 */
			vector< string > productionTokenVector;
			this->splitToVector( tokenVector.at(1), &productionTokenVector, " ", false );
			for( unsigned int i = 0; i < productionTokenVector.size(); i++ ){
				cout << "[DEBUG]: " << "production token: [" << productionTokenVector.at(i) << "]" << endl;
			}

			/*
			 * looks in the hash table of productions
			 * if the non terminal symbol was allready inserted
			 */
			EarleyParser::productionHashTable::iterator got = this->earleyProductions.find( tokenVector.at(0) );
			if( got != this->earleyProductions.end() )
			{
				/*
				 * element found in hash table (key tokenVector->at(0) contained)
				 * add a new Production at the end of the list
				 */

				cout << "[DEBUG]: found [" << got->first << "] adding Production to existing list" << endl << endl;

				got->second.push_back( Production( atof( tokenVector.at(2).c_str() ), productionTokenVector.size(), productionTokenVector ) );

			}else{
				/*
				 * element not found in hash table (key tokenVector->at(0) not contained)
				 * create a new list and save it in hash map
				 * adds new Production to the list
				 */

				list<Production> productionList;

				pair< string, list<Production> > productions( tokenVector.at(0), productionList );

				std::pair<EarleyParser::productionHashTable::iterator, bool> insertionResult;
				insertionResult = this->earleyProductions.insert( productions );
				if( insertionResult.second == true ){
					/*
					 * element inserted with success
					 */
					insertionResult.first->second.push_back( Production( atof( tokenVector.at(2).c_str() ), productionTokenVector.size(), productionTokenVector ) );

					cout << "[DEBUG]: created new list [" << productions.first << "] and inserted Production with success" << endl << endl;
				}
			}

		}
		inputFile.close();

		/*
		 * check probabilities coherence if specified by the input boolean
		 */
		if( this->probabilityParser == true ){

			for( productionHashTable::iterator i = this->earleyProductions.begin(); i != this->earleyProductions.end(); i++ ){

				float productionProbability = 0;

				cout << "[DEBUG]: checking probability of [" << i->first << "]" << endl;

				for( list<Production>::iterator j = i->second.begin(); j != i->second.end(); j++ ){

					productionProbability += j->getProductionProbability();
				}
				if( productionProbability != 1 ){
					cout << "[DEBUG]: probability of [" << i->first << "] does not sum up to 1! - BAD INPUT GRAMMAR" << endl;
					exit(1);
				}
			}
		}

	}else{
		cout << "Error: not able to open input file [" << filePath << "]" << endl;
	}
}

void EarleyParser::createDictionary( string filePath )
{
	string line;
	ifstream inputFile;

	inputFile.open( filePath.c_str(), fstream::in );

	if( inputFile.is_open() )
	{
		while( inputFile.good() )
		{
			/*
			 * reads a line (production) from the context free input grammar
			 */
			getline( inputFile, line );

			//skips empty line
			if( line.size() == 0 ) continue;

			cout << "[DEBUG]: " << line << endl;

			/*
			 * extracts tokens of the production
			 * separated by "->"
			 * first token is a word
			 * second token is the associated non terminal
			 */
			vector< string > tokenVector;
			this->splitToVector( line, &tokenVector, "->", false );

			//skips comment line
			if( tokenVector.at(0) == "#" ) continue;

			for( unsigned int i = 0; i < tokenVector.size(); i++ ){
				cout << "[DEBUG]: " << "line token: [" << tokenVector.at(i) << "]" << endl;
			}

			/*
			 * looks in the hash table of dictionary
			 * if the word was allready inserted
			 */
			EarleyParser::dictionaryHashTable::iterator got = this->earleyDictionary.find( tokenVector.at(0) );
			if( got != this->earleyDictionary.end() )
			{
				/*
				 * element found in hash table (key tokenVector->at(0) contained)
				 * error!
				 */

				cout << "[DEBUG]: BAD INPUT DICTIONARY!" << endl << endl;
				exit(1);
			}else{
				/*
				 * element not found in hash table (key tokenVector->at(0) not contained)
				 * save string in the hash map
				 */

				pair< string, string > element( tokenVector.at(0), tokenVector.at(1) );

				std::pair<EarleyParser::dictionaryHashTable::iterator, bool> insertionResult;
				insertionResult = this->earleyDictionary.insert( element );
				if( insertionResult.second == true ){
					/*
					 * element inserted with success
					 */
					cout << "[DEBUG]: [" << element.first << "][" << element.second << "] inserted with success" << endl << endl;
				}
			}

		}
		inputFile.close();

		for( dictionaryHashTable::iterator i = this->earleyDictionary.begin(); i != this->earleyDictionary.end(); i++ ){

			cout << "[DEBUG]: element [" << i->first << "][" << i->second << "] in dictionary" << endl;
		}

	}else{
		cout << "Error: not able to open input file [" << filePath << "]" << endl;
	}
}

void EarleyParser::createInputStringVector( string filePath )
{
	string line;
	ifstream inputFile;
	string inputStringToParse;

	inputFile.open( filePath.c_str(), fstream::in );

	if( inputFile.is_open() )
	{
		/*
		 * reads a line (production) from the context free input grammar
		 */
		getline( inputFile, line );

		/*
		 * extracts words of the input string
		 * separated by " "
		 */
		this->splitToVector( line, & this->inputVector, " ", false );

		for( unsigned int i = 0; i < this->inputVector.size(); i++ ){
			cout << "[DEBUG]: " << "line token: [" << this->inputVector.at(i) << "]" << endl;

			EarleyParser::dictionaryHashTable::iterator got = this->earleyDictionary.find( this->inputVector.at(i) );
			if( got != this->earleyDictionary.end() )
			{
				/*
				 * element found in hash table (key tokenVector->at(i) contained)
				 */
				if( i == 0 )
					inputStringToParse += got->second;
				else
					inputStringToParse = inputStringToParse + " " + got->second;

			}else{
				/*
				 * element not found in hash table (key tokenVector->at(i) not contained)
				 * error in input string, word not contained in dictionary
				 */
				cout << "[DEBUG]: BAD INPUT STRING!word " << this->inputVector.at(i) << " not contained in dictionary" << endl << endl;
				exit(1);
			}
		}

		cout << "[DEBUG]: input string of parser [" << inputStringToParse << "]" << endl << endl;

		inputFile.close();

		/*
		 * extracts words of the input string to parse
		 * separated by " "
		 */
		this->splitToVector( inputStringToParse, & this->inputVector, " ", false );

		for( unsigned int i = 0; i < this->inputVector.size(); i++ ){
			cout << "[DEBUG]: " << "input vector element: [" << this->inputVector.at(i) << "]" << endl;
		}


	}else{
		cout << "Error: not able to open input file [" << filePath << "]" << endl;
	}
}

list<Production>* EarleyParser::getProduction( string nonTerminal )
{
	/*
	 * looks in the hash table of productions
	 * if the non terminal symbol was allready inserted
	 */
	EarleyParser::productionHashTable::iterator got = this->earleyProductions.find( nonTerminal );
	if( got != this->earleyProductions.end() )
	{
		/*
		 * element found in hash table (key nonTerminal contained)
		 */

		//cout << "[DEBUG]: found [" << got->first << "]" << endl;

	}else{
		/*
		 * element not found in hash table (key nonTerminal not contained)
		 */

		//cout << "[DEBUG]: ERROR not found [" << nonTerminal << "] in hash map!" << endl << endl;
		return NULL;
	}

	return & got->second;
}

int EarleyParser::createChart( string mainNonTerminal )
{
	list<Production>* mainProduction;

	mainProduction = this->getProduction( mainNonTerminal );
	if( mainProduction == NULL ) return -1;

	/*
	 * providing seed of parsing algorithm
	 * main production of phrase in the context grammar
	 */
	//this->earleyParserChart = new vector<State>[ this->inputVector.size() + 1 ];
	this->earleyParserChart.resize( this->inputVector.size() + 1 );

	cout << "[DEBUG]: size of list (number of productions with " << mainNonTerminal << " as non terminal) = " << mainProduction->size() << endl;

	for( list<Production>::iterator it = mainProduction->begin(); it != mainProduction->end(); it++ )
	{
		for( unsigned int j = 0; j < it->getProductionContent().size(); j++ )
		{
			cout << "[DEBUG]: element of list " << it->getProductionContent().at(j) << endl;
		}

		State *initialState = new State( mainNonTerminal, &(*it), NULL, 0, 0 );
		this->earleyParserChart[0].push_back( initialState );
	}

	return 1;
}

void EarleyParser::parse()
{
	/*
	 * early parser main cycle
	 */
	for( unsigned int i = 0; i <= this->inputVector.size(); i++ )
	{
		cout << i << "  *****************************************************" << endl << "*****************************************************" << endl;
		/*
		 * cycles over all chart
		 */
		for( unsigned int j = 0; j < this->earleyParserChart[i].size(); j++ )
		{
			if( !( this->earleyParserChart[i].at(j)->isComplete() ) )
			{
				/*
				 * the state is not complete
				 * the production corresponding to the state has not
				 * been yet analized completely
				 */
				if( this->getProduction( this->earleyParserChart[i].at(j)->nextSymbolToProcess() ) != NULL )
				{
					/*
					 * symbol to process is a non terminal and is contained in the
					 * production hash map initialized from the input context free grammar
					 *
					 * the correct production of the symbol must be predicted
					 */
					this->predictor( this->earleyParserChart[i].at(j), i );

				}else{
					/*
					 * symbol to process is a terminal symbol
					 * which has no further expansions
					 *
					 * symbol must be simply scanned
					 */
					this->scanner( this->earleyParserChart[i].at(j), i );
				}
			}else{
				/*
				 * the state is complete
				 * all symbols in the corresponding production have been
				 * expanded (non terminals) or scanned (terminals)
				 */
				this->completer( this->earleyParserChart[i].at(j), i );
			}
		}
	}

	cout << "[DEBUG]: finished parsing input string!" << endl << endl;
}

void EarleyParser::predictor( State* stateToExpand, int positionInputString )
{
	Production* productionInStateToExpand = stateToExpand->getProductionPointer();

	if( stateToExpand->getProductionProgress() >= productionInStateToExpand->getProductionContent().size() ){
		cout << "[ERROR]: overflow in predictor!" << endl;
		exit(1);
	}
	string nonTerminalToExpand = productionInStateToExpand->getProductionContent().at( stateToExpand->getProductionProgress() );

	cout << "[DEBUG]: predictor expanding [" << nonTerminalToExpand << "][" << stateToExpand->getProductionProgress() << "][" << stateToExpand->getInputStringProgress() << "]" << endl;

	list<Production>* pointerToListOfProductions = this->getProduction( nonTerminalToExpand );

	if( pointerToListOfProductions != NULL )
	{
		for( list<Production>::iterator it = pointerToListOfProductions->begin(); it != pointerToListOfProductions->end(); it++ )
		{
			State *stateToInsert = new State( nonTerminalToExpand, &(*it), stateToExpand, positionInputString, 0 );

			cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!POSITION INPUT STRING: " << positionInputString << " " << this->inputVector.size() << endl <<endl;

			int positionOfState = this->stateExists( positionInputString, stateToInsert );
			if(  positionOfState < 0 ){
				/*
				 * state not already in the chart
				 */

				this->earleyParserChart[ positionInputString ].push_back( stateToInsert );
				//cout << "[DEBUG]: predictor expanded [" << nonTerminalToExpand << "]" << endl;
			}else{
				/*
				 * state already in chart
				 * pushes back the state from which it comes inside it's backtrace state vector
				 */
				this->earleyParserChart[ positionInputString ].at( positionOfState )->getBacktraceStateVector()->push_back( stateToExpand );
				delete stateToInsert;
			}
		}
	}else{
		/*
		 * do nothing
		 */
		cout << "[DEBUG]: ERROR prediction to expand [" << nonTerminalToExpand << "] in state not found in prediction hash table " << endl;
		exit(1);
	}
}

int EarleyParser::stateExists( int chartPosition, State* stateToCheck )
{
	for( vector<State*>::iterator it = this->earleyParserChart[ chartPosition ].begin(); it != this->earleyParserChart[ chartPosition ].end(); it++ )
	{
		if( (*it)->getNonTerminal() == stateToCheck->getNonTerminal() &&
			(*it)->getInputStringProgress() == stateToCheck->getInputStringProgress() &&
			(*it)->getProductionPointer() == stateToCheck->getProductionPointer() &&
			(*it)->getProductionProgress() == stateToCheck->getProductionProgress() )
		{
			cout << "[DEBUG]: found same state at position " << it - this->earleyParserChart[ chartPosition ].begin() << " of chart[" << chartPosition << "]" << endl;
			return ( it - this->earleyParserChart[ chartPosition ].begin());
		}
	}
	return -1;
}

void EarleyParser::scanner( State* stateToScan, int positionInputString )
{
	Production* productionInStateToScan = stateToScan->getProductionPointer();

	if( positionInputString >= this->inputVector.size() ){
		cout << "[DEBUG]: scanner fail, input string end reached" << endl;
		return;
	}

	if( this->inputVector.at( positionInputString ) == productionInStateToScan->getProductionContent().at( stateToScan->getProductionProgress() ) )
	{
		/*
		 * the symbol to be scanned in the production is the same of the input string
		 * a new state must be created in the next position of the chart
		 *
		 * must check for equal states in the chart and avoid insertion in case
		 */
		cout << "[DEBUG]: scan [" << this->inputVector.at( positionInputString ) << "][" << stateToScan->getProductionProgress() << "][" << stateToScan->getInputStringProgress() << "]" << endl;

		State* stateToInsert = new State( stateToScan->getNonTerminal(), stateToScan->getProductionPointer(), stateToScan, stateToScan->getInputStringProgress(), stateToScan->getProductionProgress() + 1 );
		cout << "[DEBUG]: scanner is inserting state in chart position " << positionInputString + 1 << endl;
		this->earleyParserChart[ positionInputString + 1 ].push_back( stateToInsert );
	}else{
		/*
		 * do nothing
		 * symbol to be scanned does not match with symbol in the input string
		 */

		cout << "[DEBUG]: scan mismatch - input[" << this->inputVector.at( positionInputString ) << "] production[" << productionInStateToScan->getProductionContent().at( stateToScan->getProductionProgress() ) << "]" << endl;
	}
}

void EarleyParser::completer( State* stateCompleted, int positionInputString )
{
	cout << "[DEBUG]: complete [" << stateCompleted->getNonTerminal() << "][" << stateCompleted->getProductionProgress() << "][" << stateCompleted->getInputStringProgress() << "]" << endl;
	//string nonTerminal = stateCompleted->getNonTerminal();
	//int inputStringProgress = stateCompleted->getInputStringProgress();
	//cout << "test input string progress: " << inputStringProgress << endl << endl;
	//vector<State>::iterator itTest = this->earleyParserChart[ stateCompleted->getInputStringProgress() ].begin();
	//cout << "test iterator: " << itTest->getNonTerminal() << endl << endl;

	if( stateCompleted->getInputStringProgress() >= this->earleyParserChart.size() ){
		cout << "[ERROR]: COMPLETER OVERFLOW" << endl;
		exit(1);
	}
	for( vector<State*>::iterator it = this->earleyParserChart[ stateCompleted->getInputStringProgress() ].begin();
			it != this->earleyParserChart[ stateCompleted->getInputStringProgress() ].end(); it++ )
	{
		if( (*it)->getProductionPointer()->getProductionContent().size() <= (*it)->getProductionProgress() ) continue;
		if( stateCompleted->getNonTerminal() == (*it)->getProductionPointer()->getProductionContent().at( (*it)->getProductionProgress() ) )
		{
			//cout << "before constructor in complete" << endl;
			State* stateToInsert = new State( (*it)->getNonTerminal(), (*it)->getProductionPointer(), stateCompleted, (*it)->getInputStringProgress(), (*it)->getProductionProgress() + 1 );
			this->earleyParserChart[ positionInputString ].push_back( stateToInsert );
			//cout << "after push back in complete" << endl;
		}
		//cout << "destructor called in complete" << endl;
	}

	//cout << "[DEBUG]: completed [" << stateCompleted.getNonTerminal() << "]" << endl;
}

vector<Node*>* EarleyParser::returnParsingTrees( string mainNonTerminal )
{

	for( vector<State*>::iterator it = this->earleyParserChart[ this->inputVector.size() ].begin();
			it != this->earleyParserChart[ this->inputVector.size() ].end(); it++ )
	{
		if( (*it)->getNonTerminal() == mainNonTerminal && (*it)->getProductionProgress() == (*it)->getProductionPointer()->getProductionContent().size() )
		{
			State* stateBookmark = (*it);
			Node* rootOfParsingTree;

			if( this->probabilityParser == true )
			{
				rootOfParsingTree =
						new Node(
								NULL,
								stateBookmark->getNonTerminal(),
								stateBookmark->getProductionPointer()->getProductionContent(),
								stateBookmark->getProductionPointer(),
								stateBookmark->getProductionPointer()->getProductionProbability()
							);
			}else{

				rootOfParsingTree =
						new Node(
								NULL,
								stateBookmark->getNonTerminal(),
								stateBookmark->getProductionPointer()->getProductionContent(),
								stateBookmark->getProductionPointer(),
								-1
							);
			}

			vector<State*>* test;
			State* test2;
			test2 = rootOfParsingTree->expandNode( stateBookmark, & this->earleyProductions );
			cout << endl << "finished expanding - " << test2->getNonTerminal() << endl;

			this->returnVector.push_back( rootOfParsingTree );
		}
	}

	return & this->returnVector;

}

void EarleyParser::printProductions( string mainNonTerminal )
{
	cout << "[DEBUG]: All Productions in chart[" << this->inputVector.size() << "]" << endl;
	cout << "[DEBUG]: size of chart " << this->earleyParserChart[ this->inputVector.size() ].size() << endl << endl;

	for( vector<State*>::iterator it = this->earleyParserChart[ this->inputVector.size() ].begin();
			it != this->earleyParserChart[ this->inputVector.size() ].end(); it++ )
	{
		cout << "[DEBUG]: size of production vector " << (*it)->getProductionPointer()->getProductionContent().size() << endl;
		cout << "[DEBUG]: - " << (*it)->getNonTerminal() << " -> ";
		for( unsigned int i = 0; i < (*it)->getProductionPointer()->getProductionContent().size(); i++ )
		{
			cout << (*it)->getProductionPointer()->getProductionContent().at(i) << " ";
		}
		cout << endl;
		cout << "****************************************************************** " << endl << "****************************************************************** " << endl << endl;
	}

	/*for( vector<State*>::iterator it = this->earleyParserChart[ this->inputVector.size() ].begin(); it != this->earleyParserChart[ this->inputVector.size() ].end(); it++ )
	{
		if( (*it)->getNonTerminal() == mainNonTerminal && (*it)->getProductionProgress() == (*it)->getProductionPointer()->getProductionContent().size() )
			this->printStateBackTrace( (*it) );
	}*/
}

void EarleyParser::printStateBackTrace( State* stateToPrint )
{
	//cout << "[DEBUG]: in print back trace function" << endl;

	cout << "[DEBUG]: - " << stateToPrint->getNonTerminal() << " -> ";
	for( unsigned int i = 0; i < stateToPrint->getProductionPointer()->getProductionContent().size(); i++ )
	{
		cout << stateToPrint->getProductionPointer()->getProductionContent().at(i) << " ";
	}

	vector<State*>* backTraceVectorState = stateToPrint->getBacktraceStateVector();
	cout << endl << "size of backtrace vector state: " << backTraceVectorState->size() << endl;

	//for( unsigned int i = 0; i < backTraceVectorState->at(0)->getProductionPointer()->getProductionContent().size(); i++ )
	//{
	//	cout << " " << backTraceVectorState->at(0)->getProductionPointer()->getProductionContent().at(i);
	//}

	for( unsigned int i = 0; i < backTraceVectorState->size(); i++ )
	{
		if( backTraceVectorState->at(i) == NULL ){
			cout << "[DEBUG]: pointer to next state is null" << endl;
			continue;
		}

		if( backTraceVectorState->at(i)->getPrinted() == false )
		{
			//cout << "[DEBUG]: Production expanded [ " << *(*it)->getNonTerminal() << " -> ";
			//for( unsigned int i = 0; i < *(*it)->getProductionPointer()->getProductionContent().size(); i++ ){
			//	cout << (*it)->getProductionPointer()->getProductionContent().at(i) << " ";
			//}
			//cout << "]" << endl;
		//	cout << "[DEBUG]: - check if problem is iterator ->>>>>>>>>>>>" << backTraceVectorState->at(i)->getInputStringProgress() << endl << endl;
			//cout << "[DEBUG]: - " << stateToPrint->getNonTerminal() << " -> " <<endl;
			this->printStateBackTrace( backTraceVectorState->at(i) );
		}
		//backTraceVectorState->at(i)->setPrintedTrue();
	}

	//	cout << "[DEBUG]: - check if problem is pointer ->>>>>>>>>>>>" << stateToPrint->getInputStringProgress() << endl;

	cout << endl << endl;

	stateToPrint->setPrintedTrue();
}

void EarleyParser::splitToVector
	(
		const std::string& inputString,
        std::vector<std::string>* outputVector,
        const std::string& separatorString,
        bool skipNullStrings
    )
{
    unsigned int pos = 0;
    outputVector->clear();

    while( pos < inputString.length() )
    {
        std::string next = inputString.substr( pos );
        unsigned int next_pos = next.find( separatorString );

        if( next_pos == static_cast<unsigned int> (std::string::npos) )
            next_pos = next.length();

        if( !skipNullStrings || next_pos > 0 )
        {
            std::string to_insert = next.substr( 0, next_pos );
            outputVector->push_back( to_insert );
        }

        pos += next_pos + separatorString.size();
    }
}
