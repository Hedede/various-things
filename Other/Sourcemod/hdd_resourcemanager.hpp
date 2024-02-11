//===== No copyrights! ======================================================//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#ifndef HDDRESMGR_H
#define HDDRESMGR_H
#ifdef _WIN32
#pragma once
#endif

#include "triggers.h"

// ##################################################################################
//	>> Crate acceptor
// ##################################################################################
//-----------------------------------------------------------------------------
#define MAX_RESTYPES 5
class CTriggerResourceAcceptor : public CBaseTrigger
{
	DECLARE_CLASS( CTriggerMultiple, CBaseTrigger );
private:
	int m_ResourceCap;

	bool m_bHitZero;
	bool m_bHitMax;

	
	string_t	m_ResourceName[MAX_RESTYPES];
	int 		m_ResourceCost[MAX_RESTYPES];
	int 		m_PlayerCost;
	int 		m_NPCCost;

public:
	bool KeyValue(const char *szKeyName, const char *szValue);
	void Spawn( void );
	void TouchReact( CBaseEntity *pOther );
	void ProcessTrigger(CBaseEntity *pActivator);
	void MultiWaitOver( void ); //Needed to be deprecated

	void UpdateOutValue(CBaseEntity *pActivator, int NewValue);
	
	//Inputs
	void InputGetResources( inputdata_t &inputdata );
	void InputConsumeResources( inputdata_t &inputdata );

	// Outputs
	COutputEvent m_OnTrigger;
	COutputEvent m_OnHitZero;
	COutputEvent m_OnHitMax;
	COutputEvent m_OnNormal;
	COutputEvent m_OnConsume;
	COutputInt m_Resources;
	COutputInt m_OnGetResources;

	DECLARE_DATADESC();
};

//-----------------------------------------------------------------------------
// Purpose: Used to control resource flow and create entities.
//-----------------------------------------------------------------------------
#endif //HDDRESMGR
