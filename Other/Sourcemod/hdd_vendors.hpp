
#ifndef HDDVENDORS_H
#define HDDVENDORS_H
#ifdef _WIN32
#pragma once
#endif

#include "TemplateEntities.h"
#include "point_template.h"

struct supply_template_t
{
	int			iTemplateIndex;
	int			iSupplyCost;
	VMatrix		matEntityToTemplate;

	DECLARE_SIMPLE_DATADESC();
};

// ##################################################################################
//	>> Supply vendor
// ##################################################################################
// Purpose: Convert resources back to entities.
//-----------------------------------------------------------------------------
#define MAX_HVENDOR_TEMPLATES		16

class CPointVendor : public CPointTemplate
{
	DECLARE_CLASS( CPointVendor, CPointTemplate );
public:
	DECLARE_DATADESC();
	
	virtual void	Spawn( void );
	virtual void	Precache();

	// Template initialization
	void			StartBuildingTemplates( void );
	void			FinishBuildingTemplates( void );

	// Template Entity accessors
	int				GetNumTemplateEntities( void );
	CBaseEntity		*GetTemplateEntity( int iTemplateNumber );
	void			AddTemplate( CBaseEntity *pEntity, const char *pszMapData, int nLen );

	// Templates accessors
	int				GetNumTemplates( void );
	int				GetTemplateIndexForTemplate( int iTemplate );

	// Template instancing
	bool			CreateInstance(char *TemplateId, const Vector &vecOrigin, const QAngle &vecAngles, CUtlVector<CBaseEntity*> *pEntities );

	// Inputs
	void			InputSupplySpawn( inputdata_t &inputdata );

	//virtual void	PerformPrecache();
private:
	string_t	m_iszTemplateEntityNames[MAX_HVENDOR_TEMPLATES];
	int			m_iSupplyCost[MAX_HVENDOR_TEMPLATES];

	CUtlVector< CBaseEntity * >		m_hTemplateEntities;
	CUtlVector< template_t >		m_hTemplates;
	
	// Output
	COutputEvent					m_pOutputOnSpawned;
};


// ##################################################################################
//	>> Upgrade vendor
// #################################################################################

#endif // HDDVENDORS_H
