// ExerciciosHLA.cpp : define o ponto de entrada para o aplicativo do console.
//

#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <exception>
#include <map>
#include <string.h>
#include "RTI\NullFederateAmbassador.h"
#include "RTI\RTIambassador.h"
#include "RTI\RTIambassadorFactory.h"
#include "RTI\Exception.h"
#include "RTI\Enums.h"
#include <RTI/time/HLAinteger64TimeFactory.h>
#include <RTI/time/HLAinteger64Time.h>
#include <RTI/time/HLAinteger64Interval.h>
#include <RTI/encoding/BasicDataElements.h>
#include <RTI/RTI1516.h>
#include <stdio.h>

#include "FederadoExemplo.h"

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

using namespace std;
using namespace rti1516e;


int main()
{
	wstring nome = L"exampleFederate";
	wcout << rtiVersion() << endl;
	FederadoExemplo * federado;
	federado = new FederadoExemplo();
	federado->runFederado(nome);
	
	system("PAUSE");
	delete federado;
    return 0;
}

