//============================================================================
// Name        : NLP_Earley_Parser_main.cpp
// Author      : 
// Version     :
// Copyright   : Author Matteo Ruggero Ronchi, All Rights Reserved.
//				(For any kind of information on the license contact matrronchi.license@gmail.com)
// Description : Earley Parser Implementation
//============================================================================

/*
 * NOTE: 	controls on the input file are not performed
 * 			in case of bad input program might misbehave
 */

/*
 * NLP_Early_Parser_main.cpp
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
#include "State.h"

using namespace std;

int main( int argc, char** argv )
{
	if( argc < 6 ){

		cout << endl << "Usage: ./NLP_Early_Parser 'Context Free Input Grammar File' 'Input Dictionary File' 'Input string file path' 'main production' 'probability parse?(y/n)'" << endl << endl;
		exit(1);
	}

	EarleyParser myEarleyParser;
	bool probabilityParser;

	int checkOperation;

	if( string( argv[5] ).compare("y") == 0 )
		probabilityParser = true;
	else
		probabilityParser = false;

	myEarleyParser.initialize( string( argv[1] ), probabilityParser );
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	myEarleyParser.createDictionary( string( argv[2] ) );
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	myEarleyParser.createInputStringVector( string( argv[3] ) );
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	string mainNonTerminal = string( argv[4] );
	checkOperation = myEarleyParser.createChart( mainNonTerminal );
	if( checkOperation < 0 ) exit(1);
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	myEarleyParser.parse();
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	myEarleyParser.printProductions( mainNonTerminal );
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	vector<Node*>* vectorOfParsingTreeRoots;

	vectorOfParsingTreeRoots = myEarleyParser.returnParsingTrees( mainNonTerminal );
	cout << "*****************************************************" << endl << "*****************************************************" << endl;

	for( vector<Node*>::iterator it = vectorOfParsingTreeRoots->begin(); it != vectorOfParsingTreeRoots->end(); it++ )
	{
		float prob;
		prob = (*it)->print( probabilityParser );
		cout << "Probaility of the printed tree is: [" << prob << "]" << endl;
		cout << "*****************************************************" << endl << "*****************************************************" << endl << endl;
	}

	return 0;

}
