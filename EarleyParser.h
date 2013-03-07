/*
 * EarleyParser.h
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

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include <string.h>
#include <list>

#include "Production.h"
#include "State.h"
#include "Node.h"

using namespace std;

class EarleyParser
{

	public:

		EarleyParser();

		virtual ~EarleyParser();

		void initialize( string filePath, bool probabilityParser );
		void createDictionary( string filePath );
		void createInputStringVector( string filePath );

		int createChart( string mainNonTerminal );
		void parse();

		void printProductions( string mainNonTerminal );
		vector<Node*>* returnParsingTrees( string mainNonTerminal );

	private:

		bool probabilityParser;

		typedef unordered_map< string, list<Production> > productionHashTable;
		productionHashTable earleyProductions;

		typedef unordered_map< string, string > dictionaryHashTable;
		dictionaryHashTable earleyDictionary;

		vector<vector< State* >> earleyParserChart;

		vector< string > inputVector;

		vector<Node*> returnVector;

		list<Production>* getProduction( string nonTerminal );

		void predictor( State* stateToExpand, int positionInputString );
		void scanner( State* stateToScan, int positionInputString );
		void completer( State* stateCompleted, int positionInputString );

		int stateExists( int chartPosition, State* stateToCheck );

		void printStateBackTrace( State* stateToPrint );

		void splitToVector( const std::string& inputString, std::vector<std::string>* outputVector, const std::string& separatorString, bool skipNullStrings );

};
