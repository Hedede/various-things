m_diagInvInertiaStore = m_body->getInvInertiaDiagLocal();
m_body->setInvInertiaDiagLocal(btVector3(0,0,0));
m_body->updateInertiaTensor();
m_rotationEnabled = false;

m_body->setInvInertiaDiagLocal( m_diagInvInertiaStore );
m_body->updateInertiaTensor();
m_rotationEnabled = true;