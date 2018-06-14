#pragma once

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

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

#include "FederadoAmbassExemplo.h"
using namespace std;
using namespace rti1516e;

#define READY_TO_RUN L"ReadyToRun"

class FederadoExemplo {
	public:
		RTIambassador* rti;
		FederadoAmbassExemplo* fedamb;

		ObjectClassHandle refriHandle;
		AttributeHandle numeroCoposHandle;
		AttributeHandle saborHandle;

		InteractionClassHandle adminServirHandle;
		ParameterHandle adminCheckHandle;

		FederadoExemplo();
		void runFederado(wstring nome);
	private:
		void inicializaHandles();
		void esperaUsuarios();
		void habilitaPolitica();
		void publicaAssina();
		ObjectInstanceHandle registraObjeto();

		void atualizaValorAtributos(ObjectInstanceHandle obj);
		void enviaInteracao();
		void avancaTempo(double tempo);
		void apagaObjeto(ObjectInstanceHandle obj);
		double getLbts();

};

