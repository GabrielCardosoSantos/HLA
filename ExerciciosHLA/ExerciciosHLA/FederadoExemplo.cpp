#include "stdafx.h"
#include "FederadoExemplo.h"
#include "FederadoAmbassExemplo.h"

#include <string>

#include "RTI/time/HLAfloat64Interval.h"
#include "RTI/time/HLAfloat64Time.h"
using namespace std;

FederadoExemplo::FederadoExemplo()
{

}

void FederadoExemplo::runFederado(wstring nome)
{
	RTIambassadorFactory factory = RTIambassadorFactory();
	rti = factory.createRTIambassador().release();

	this->fedamb = new FederadoAmbassExemplo();

	try {
		rti->connect(*this->fedamb, HLA_EVOKED);
	}
	catch (ConnectionFailed& connectionFailed)
	{
		wcout << L"Connection failed: " << connectionFailed.what() << endl;
	}
	catch (InvalidLocalSettingsDesignator& settings)
	{
		wcout << L"Connection failed, InvalidLocalSettingsDesignator: " << settings.what() << endl;
	}
	catch (UnsupportedCallbackModel& callbackModel)
	{
		wcout << L"Connection failed, UnsupportedCallbackModel: " << callbackModel.what() << endl;
	}
	catch (AlreadyConnected& connected)
	{
		wcout << L"Connection failed, AlreadyConnected: " << connected.what() << endl;
	}
	catch (RTIinternalError& error)
	{
		wcout << L"Connection failed, Generic Error: " << error.what() << endl;
	}

	try {
		vector<wstring> foms;
		foms.push_back(L"restaurant/RestaurantFood.xml");
		foms.push_back(L"restaurant/RestaurantDrinks.xml");
		foms.push_back(L"restaurant/RestaurantProcesses.xml");

		rti->createFederationExecution(L"ExampleFederation", foms);
	}
	catch (FederationExecutionAlreadyExists& exists)
	{
		wcout << L"Federação já existe." << endl;
	}
	catch (Exception& e)
	{
		wcout << L"Erro na criação: " << e.what() << endl;
	}

	rti->joinFederationExecution(nome, L"Example Federate", L"ExampleFederation");
	wcout << L"Joined Federation as " << nome << endl;

	inicializaHandles();

	VariableLengthData tag((void*)"", 1);
	rti->registerFederationSynchronizationPoint(READY_TO_RUN, tag);
	while (fedamb->isAnnounced == false)
	{
		rti->evokeMultipleCallbacks(0.1, 1.0);
	}

	esperaUsuarios();

	rti->synchronizationPointAchieved(READY_TO_RUN);
	wcout << L"Achieved sync point: " << READY_TO_RUN << L", waiting for federation..." << endl;
	while (fedamb->isReadyToRun == false)
	{
		rti->evokeMultipleCallbacks(0.1, 1.0);
	}

	habilitaPolitica();
	wcout << L"Politica habilitada" << endl;

	publicaAssina();

	wcout << L"Publica e Assina" << endl;

	ObjectInstanceHandle objectHandle = registraObjeto();

	wcout << L"Registro objeto, handle: "<< objectHandle << endl;

	float time = 1.0;

	for (int i = 0; i < 20; i++) {
		atualizaValorAtributos(objectHandle);
		enviaInteracao();
		avancaTempo(time);
		wcout << L"Tempo avancado em " << time <<" seg: " << fedamb->federateTime << endl;
	}

	apagaObjeto(objectHandle);

	wcout << L"Objeto apagado: " << objectHandle << endl;

	rti->resignFederationExecution(NO_ACTION);

	wcout << L"Resignado pela federacao" << endl;

	try {
		rti->destroyFederationExecution(L"ExampleFederation");
		wcout << L"Federacao excluida." << endl;
	}
	catch (FederationExecutionDoesNotExist& dne) {
		wcout << L"Erro ao destruir federacao" << endl;
	}
	catch (FederatesCurrentlyJoined& fcj) {
		wcout << L"Nao foi possivel destruir federacao" << endl;
	}

	this->rti->disconnect();

	delete this->rti;

}

void FederadoExemplo::inicializaHandles()
{
	this->refriHandle = rti->getObjectClassHandle(L"HLAobjectRoot.Food.Drink.Soda");
	this->numeroCoposHandle = rti->getAttributeHandle(refriHandle, L"NumberCups");
	this->saborHandle = rti->getAttributeHandle(refriHandle, L"Flavor");

	this->adminServirHandle = rti->getInteractionClassHandle(L"HLAinteractionRoot.CustomerTransactions.FoodServed.RootBeerServed");
	this->adminCheckHandle = rti->getParameterHandle(adminServirHandle, L"SodaType");
}

void FederadoExemplo::esperaUsuarios()
{
	wcout << endl << L">>>>>> Pressione Enter para continuar <<<<<<<<<<<<" << endl;
	string linha;
	getline(cin, linha);
}

void FederadoExemplo::habilitaPolitica()
{
	double lookahead = fedamb->federateLookahead;
	auto_ptr<HLAfloat64Interval> interval(new HLAfloat64Interval(lookahead));
	rti->enableTimeRegulation(*interval);

	while (fedamb->isRegulating == false) {
		rti->evokeMultipleCallbacks(0.1, 1.0);
	}

	rti->enableTimeConstrained();

	while (fedamb->isConstrained == false) {
		rti->evokeMultipleCallbacks(0.1, 1.0);
	}

}

void FederadoExemplo::publicaAssina()
{
	AttributeHandleSet att;
	att.insert(this->numeroCoposHandle);
	att.insert(this->saborHandle);

	rti->publishObjectClassAttributes(this->refriHandle, att);

	rti->subscribeObjectClassAttributes(this->refriHandle, att);
	rti->publishInteractionClass(this->adminServirHandle);
	rti->subscribeInteractionClass(this->adminServirHandle);
}

ObjectInstanceHandle FederadoExemplo::registraObjeto()
{
	return rti->registerObjectInstance(rti->getObjectClassHandle(L"HLAobjectRoot.Food.Drink.Soda"));
}

void FederadoExemplo::atualizaValorAtributos(ObjectInstanceHandle obj)
{
	AttributeHandleValueMap att;
	char qtdCopos[16], sabores[16];
	sprintf_s(qtdCopos, "Copos: %d", 5);
	sprintf_s(sabores, "Sabor: %d", 102);

	VariableLengthData qtdCoposData((void*)qtdCopos, strlen(qtdCopos)+1);
	VariableLengthData saborData((void*)sabores, strlen(sabores) + 1);

	att[numeroCoposHandle] = qtdCoposData;
	att[saborHandle] = saborData;

	VariableLengthData tag((void*) "Hi!", 4);
	rti->updateAttributeValues(obj, att, tag);

	auto_ptr<HLAfloat64Time> time(new HLAfloat64Time(fedamb->federateTime + fedamb->federateLookahead));

	rti->updateAttributeValues(obj, att, tag, *time);
}

void FederadoExemplo::enviaInteracao()
{

	ParameterHandleValueMap parametros;

	char saborValor[16];

	sprintf_s(saborValor, "Sabor: %d", 103);

	VariableLengthData saborData((void*)saborValor, strlen(saborValor) + 1);
	parametros[adminCheckHandle] = saborData;

	VariableLengthData tag((void*) "Hi!", 4);
	rti->sendInteraction(this->adminServirHandle, parametros, tag);

	auto_ptr<HLAfloat64Time> time(new HLAfloat64Time(fedamb->federateTime + fedamb->federateLookahead));

	rti->sendInteraction(this->adminServirHandle, parametros, tag, *time);

}

void FederadoExemplo::avancaTempo(double tempo)
{
	fedamb->isAdvancing = true;
	auto_ptr<HLAfloat64Time> novoTempo(new HLAfloat64Time(fedamb->federateTime + tempo));
	rti->timeAdvanceRequest(*novoTempo);

	while (fedamb->isAdvancing) {
		rti->evokeMultipleCallbacks(0.1, 1.0);
	}
}

void FederadoExemplo::apagaObjeto(ObjectInstanceHandle obj)
{
	VariableLengthData tag((void*) "Hi!", 4);
	rti->deleteObjectInstance(obj, tag);
}

double FederadoExemplo::getLbts()
{
	return (fedamb->federateTime+ fedamb->federateLookahead);
}

