/*
 * Production.h
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

#ifndef PRODUCTION_H_
#define PRODUCTION_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

using namespace std;

class Production
{
	public:

		Production( float productionProbability, int productionLength, vector< string > productionContent );

		//Production( const Production& copy_me );

		virtual ~Production();

		float getProductionProbability();
		int getProductionLength();
		vector< string > getProductionContent();

	private:

		explicit Production() {}

		float productionProbability;
		int productionLength;
		vector< string > productionContent;

};


#endif /* PRODUCTION_H_ */
