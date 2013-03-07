/*
 * Production.cpp
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

using namespace std;

Production::Production( float productionProbability, int productionLength, vector< string > productionContent )
{
	this->productionProbability = productionProbability;
	this->productionLength = productionLength;

	this->productionContent = productionContent;

}

Production::~Production()
{

}

vector< string > Production::getProductionContent()
{
	return this->productionContent;
}

int Production::getProductionLength()
{
	return this->productionLength;
}

float Production::getProductionProbability()
{
	return this->productionProbability;
}
