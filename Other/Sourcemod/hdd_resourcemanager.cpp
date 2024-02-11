//===== No copyrights! ======================================================//
//
// Purpose: Ebat' mozg. Oh, sorry! Used to convert certain entities to int
//
//===========================================================================//

#include "cbase.h"
#include "triggers.h"
#include "hdd_resourcemanager.hpp"

// ##################################################################################
//	>> Crate acceptor
// ##################################################################################
// Purpose: Ebat' mozg. Oh, sorry! Used to convert certain entities to int.
//			If "delay" is set, the trigger waits some time after activating before firing.
//			"wait" : Seconds between triggerings. (.2 default/minimum)
//-----------------------------------------------------------------------------
LINK_ENTITY_TO_CLASS( h_resource_acceptor, CTriggerResourceAcceptor );


BEGIN_DATADESC( CTriggerResourceAcceptor )
	DEFINE_FIELD(m_bHitZero, FIELD_BOOLEAN),
	DEFINE_FIELD(m_bHitMax, FIELD_BOOLEAN),

	// Function Pointers
	DEFINE_FUNCTION(TouchReact),
	DEFINE_FUNCTION(MultiWaitOver),

	// Keys
	DEFINE_KEYFIELD(m_ResourceCap, FIELD_INTEGER, "max"),
	DEFINE_KEYFIELD(m_PlayerCost, FIELD_INTEGER, "playerCost"),
	DEFINE_KEYFIELD(m_NPCCost, FIELD_INTEGER, "npcCost"),
	DEFINE_KEYFIELD(m_ResourceName[0], FIELD_STRING, "Resource1Name"),
	DEFINE_KEYFIELD(m_ResourceCost[0], FIELD_INTEGER, "Resource1Cost"),
	DEFINE_KEYFIELD(m_ResourceName[1], FIELD_STRING, "Resource2Name"),
	DEFINE_KEYFIELD(m_ResourceCost[1], FIELD_INTEGER, "Resource2Cost"),
	DEFINE_KEYFIELD(m_ResourceName[2], FIELD_STRING, "Resource3Name"),
	DEFINE_KEYFIELD(m_ResourceCost[2], FIELD_INTEGER, "Resource3Cost"),
	DEFINE_KEYFIELD(m_ResourceName[3], FIELD_STRING, "Resource4Name"),
	DEFINE_KEYFIELD(m_ResourceCost[3], FIELD_INTEGER, "Resource4Cost"),
	DEFINE_KEYFIELD(m_ResourceName[4], FIELD_STRING, "Resource5Name"),
	DEFINE_KEYFIELD(m_ResourceCost[4], FIELD_INTEGER, "Resource5Cost"),

	// Inputs
	DEFINE_INPUTFUNC(FIELD_VOID, "GetResources", InputGetResources),
	DEFINE_INPUTFUNC(FIELD_INTEGER, "ConsumeResources", InputConsumeResources),

	// Outputs
	DEFINE_OUTPUT(m_OnTrigger, "OnTrigger"),
	DEFINE_OUTPUT(m_OnHitZero, "OnHitZero"),
	DEFINE_OUTPUT(m_OnHitMax, "OnHitMax"),
	DEFINE_OUTPUT(m_OnNormal, "OnNormal"),
	DEFINE_OUTPUT(m_OnConsume, "OnConsume"),
	DEFINE_OUTPUT(m_Resources, "Resources"),
	DEFINE_OUTPUT(m_OnGetResources, "OnGetResources")

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Handles key values from the BSP before spawn is called.
//-----------------------------------------------------------------------------
bool CTriggerResourceAcceptor::KeyValue(const char *szKeyName, const char *szValue)
{
	//
	// Set the initial value of the counter.
	//
	if (!stricmp(szKeyName, "startvalue"))
	{
		m_Resources.Init(atoi(szValue));
		return(true);
	}

	return(BaseClass::KeyValue(szKeyName, szValue));
}

//-----------------------------------------------------------------------------
// Purpose: Called when spawning, after keyvalues have been handled.
//-----------------------------------------------------------------------------
void CTriggerResourceAcceptor::Spawn( void )
{
	BaseClass::Spawn();

	InitTrigger();

	if (m_flWait == 0)
	{
		m_flWait = 0.2;
	}

	//
	// Clamp initial value to within the valid range.
	//
	if (m_ResourceCap > 0)
	{
		int m_StartingResources = clamp(m_Resources.Get(), 0, m_ResourceCap);
		m_Resources.Init(m_StartingResources);
	}

	//ASSERTSZ(m_iHealth == 0, "trigger_multiple with health");
	SetTouch( &CTriggerResourceAcceptor::TouchReact );
}


//-----------------------------------------------------------------------------
// Purpose: Touch function. Activates the trigger.
// Input  : pOther - The thing that touched us.
//-----------------------------------------------------------------------------
void CTriggerResourceAcceptor::TouchReact(CBaseEntity *pOther)
{
	if(PassesTriggerFilters(pOther) && pOther->m_takedamage)
	{
		ProcessTrigger( pOther );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : pActivator - 
//-----------------------------------------------------------------------------
#define HDDVENDORS_TEST
void CTriggerResourceAcceptor::ProcessTrigger(CBaseEntity *pActivator)
{
	if (GetNextThink() > gpGlobals->curtime)
		return;         // still waiting for reset time

	m_hActivator = pActivator;
	
	m_OnTrigger.FireOutput(m_hActivator, this);


	if (pActivator->IsPlayer())
	{
		int NewValue = m_Resources.Get() + m_PlayerCost;
		UpdateOutValue( pActivator, NewValue );
	} 
	else if(pActivator->GetFlags() & FL_NPC)
	{
		int NewValue = m_Resources.Get() + m_NPCCost;
		UpdateOutValue( pActivator, NewValue );
	}
	else
	{	
		for(int i = 0; i < MAX_RESTYPES; i++)
		{
			if ( m_ResourceName[i] != NULL_STRING )
			{
				if(pActivator->NameMatches( STRING(m_ResourceName[i]) ))
				{
					int NewValue = m_Resources.Get() + m_ResourceCost[i];
					UpdateOutValue( pActivator, NewValue );
				}
			}
		}
	}

	// The damage position is the nearest point on the damaged entity
	// to the trigger's center. Not perfect, but better than nothing.
	Vector vecCenter = CollisionProp()->WorldSpaceCenter();
	Vector vecDamagePos;
	pActivator->CollisionProp()->CalcNearestPoint( vecCenter, &vecDamagePos );

	CTakeDamageInfo info( this, this, 9999, DMG_BLAST);
	info.SetDamagePosition( vecDamagePos );
	GuessDamageForce( &info, ( vecDamagePos - vecCenter ), vecDamagePos );
		
	pActivator->TakeDamage( info );

	if (m_flWait > 0)
	{
		SetThink( &CTriggerResourceAcceptor::MultiWaitOver );
		SetNextThink( gpGlobals->curtime + m_flWait );
	}
	else
	{
		// we can't just remove (self) here, because this is a touch function
		// called while C code is looping through area links...
		SetTouch( NULL );
		SetNextThink( gpGlobals->curtime + 0.1f );
		SetThink(  &CTriggerResourceAcceptor::SUB_Remove );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Input handler for subtracting from the current value.
// Input  : Float value to subtract.
//-----------------------------------------------------------------------------
void CTriggerResourceAcceptor::InputConsumeResources( inputdata_t &inputdata )
{
	int NewValue = m_Resources.Get() - inputdata.value.Int();
	UpdateOutValue( inputdata.pActivator, NewValue );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CTriggerResourceAcceptor::InputGetResources( inputdata_t &inputdata )
{
	int OutValue = m_Resources.Get();
	m_OnGetResources.Set( OutValue, inputdata.pActivator, inputdata.pCaller );
}

void CTriggerResourceAcceptor::UpdateOutValue(CBaseEntity *pActivator, int NewValue)
{
	if (m_ResourceCap > 0)
	{
		//
		// Fire an output any time we reach or exceed our maximum value.
		//
		if (NewValue >= m_ResourceCap)
		{
			if ( !m_bHitMax )
				m_bHitMax = true;
			
			m_OnHitMax.FireOutput( pActivator, this );
		}
		else
		{
			m_bHitMax = false;
		}

		//
		// Fire an output any time we reach or go below our minimum value.
		//
		if (NewValue <= 0)
		{
			if (!m_bHitZero)
			{
				m_bHitZero = true;
				//m_OnHitZeroOnce.FireOutput( pActivator, this );
			}
			m_OnHitZero.FireOutput( pActivator, this );
		}
		else
		{
			m_bHitZero = false;
		}

		if(!m_bHitZero && !m_bHitMax)
		{
			m_OnNormal.FireOutput( pActivator, this );
		}

		NewValue = clamp(NewValue, 0, m_ResourceCap);
	}

	m_Resources.Set(NewValue, pActivator, this);
}

//-----------------------------------------------------------------------------
// Purpose: The wait time has passed, so set back up for another activation
//-----------------------------------------------------------------------------
void CTriggerResourceAcceptor::MultiWaitOver( void )
{
	SetThink( NULL );
}
#