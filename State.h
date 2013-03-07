/*
 * State.h
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

#ifndef STATE_H_
#define STATE_H_

#include "Production.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

using namespace std;

class State
{
	public:

		State( string nonTerminal, Production* productionPointer, State* backtraceState, int inputStringProgress, int productionProgress );

		//State();

		virtual ~State();

		vector<State*>* getBacktraceStateVector();
		const string getNonTerminal();

		Production* getProductionPointer() const;
		int getProductionProgress() const;
		int getInputStringProgress();

		bool isComplete();
		string nextSymbolToProcess();

		bool getPrinted();
		void setPrintedTrue();

	private:

		explicit State() : productionPointer( NULL ) {}

		string nonTerminal;
		Production* productionPointer;

		vector<State*> backtraceStateVector;

		int productionProgress;
		int inputStringProgress;

		bool printed;

};


#endif /* STATE_H_ */
