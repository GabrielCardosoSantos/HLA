#pragma once


#include <string>
#include <RTI\NullFederateAmbassador.h>

using namespace std;
using namespace rti1516e;

class FederadoAmbassExemplo : public NullFederateAmbassador
{
	private:
		double convertTime(const LogicalTime& theTime);

	public:

		double federateTime;
		double federateLookahead;


		bool isRegulating;
		bool isConstrained;
		bool isAdvancing;
		bool isAnnounced;
		bool isReadyToRun;


		FederadoAmbassExemplo();
		~FederadoAmbassExemplo();

		virtual void synchronizationPointRegistrationSucceeded(const std::wstring& label)
			throw(FederateInternalError);

		virtual void
			synchronizationPointRegistrationFailed(const std::wstring& label,
				SynchronizationPointFailureReason reason)
			throw(FederateInternalError);

		virtual void announceSynchronizationPoint(const std::wstring& label,
			const VariableLengthData& tag)
			throw(FederateInternalError);

		virtual void federationSynchronized(const std::wstring& label,
			const FederateHandleSet& failedSet)
			throw(FederateInternalError);

		//////////////////////////
		// time related methods //
		//////////////////////////
		virtual void timeRegulationEnabled(const LogicalTime& theFederateTime)
			throw(FederateInternalError);

		virtual void timeConstrainedEnabled(const LogicalTime& theFederateTime)
			throw(FederateInternalError);

		virtual void timeAdvanceGrant(const LogicalTime& theFederateTime)
			throw(FederateInternalError);

		///////////////////////////////
		// object management methods //
		///////////////////////////////
		// object discovery
		virtual void discoverObjectInstance(ObjectInstanceHandle theObject,
			ObjectClassHandle theObjectClass,
			const std::wstring& theObjectName)
			throw(FederateInternalError);

		virtual void discoverObjectInstance(ObjectInstanceHandle theObject,
			ObjectClassHandle theObjectClass,
			const std::wstring& theObjectName,
			FederateHandle producingFederate)
			throw(FederateInternalError);


		// attribute reflection
		virtual void reflectAttributeValues(ObjectInstanceHandle theObject,
			const AttributeHandleValueMap& theAttributes,
			const VariableLengthData& tag,
			OrderType sentOrder,
			TransportationType theType,
			SupplementalReflectInfo theReflectInfo)
			throw(FederateInternalError);

		virtual void reflectAttributeValues(ObjectInstanceHandle theObject,
			const AttributeHandleValueMap& theAttributes,
			const VariableLengthData& tag,
			OrderType sentOrder,
			TransportationType theType,
			LogicalTime const & theTime,
			OrderType receivedOrder,
			SupplementalReflectInfo theReflectInfo)
			throw(FederateInternalError);

		virtual void reflectAttributeValues(ObjectInstanceHandle theObject,
			const AttributeHandleValueMap& theAttributes,
			const VariableLengthData& tag,
			OrderType sentOrder,
			TransportationType theType,
			const LogicalTime& theTime,
			OrderType receivedOrder,
			MessageRetractionHandle theHandle,
			SupplementalReflectInfo theReflectInfo)
			throw(FederateInternalError);


		// interaction received
		virtual void receiveInteraction(InteractionClassHandle theInteraction,
			const ParameterHandleValueMap& theParameters,
			const VariableLengthData& tag,
			OrderType sentOrder,
			TransportationType theType,
			SupplementalReceiveInfo theReceiveInfo)
			throw(FederateInternalError);

		virtual void receiveInteraction(InteractionClassHandle theInteraction,
			const ParameterHandleValueMap& theParameters,
			const VariableLengthData& tag,
			OrderType sentOrder,
			TransportationType theType,
			const LogicalTime& theTime,
			OrderType receivedOrder,
			SupplementalReceiveInfo theReceiveInfo)
			throw(FederateInternalError);

		virtual void receiveInteraction(InteractionClassHandle theInteraction,
			const ParameterHandleValueMap& theParameters,
			const VariableLengthData& tag,
			OrderType sentOrder,
			TransportationType theType,
			const LogicalTime& theTime,
			OrderType receivedOrder,
			MessageRetractionHandle theHandle,
			SupplementalReceiveInfo theReceiveInfo)
			throw(FederateInternalError);


		// object removed
		virtual void removeObjectInstance(ObjectInstanceHandle theObject,
			const VariableLengthData& tag,
			OrderType sentOrder,
			SupplementalRemoveInfo theRemoveInfo)
			throw(FederateInternalError);

		virtual void removeObjectInstance(ObjectInstanceHandle theObject,
			const VariableLengthData& tag,
			OrderType sentOrder,
			const LogicalTime& theTime,
			OrderType receivedOrder,
			SupplementalRemoveInfo theRemoveInfo)
			throw(FederateInternalError);

		virtual void removeObjectInstance(ObjectInstanceHandle theObject,
			const VariableLengthData& tag,
			OrderType sentOrder,
			const LogicalTime& theTime,
			OrderType receivedOrder,
			MessageRetractionHandle theHandle,
			SupplementalRemoveInfo theRemoveInfo)
			throw(FederateInternalError);
};

