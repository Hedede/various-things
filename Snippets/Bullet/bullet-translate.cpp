    float dx, dy, distance;
    if (_firstMouseEvent.get() == NULL || _lastMouseEvent.get() == NULL) {
        dx = dy = distance = 0;
    } else {
        dx = _firstMouseEvent->getX() - _lastMouseEvent->getX();
        dy = _firstMouseEvent->getY() - _lastMouseEvent->getY();
        distance = sqrt(dx * dx + dy * dy);
    }

    osgbBullet::MotionState* motion = _pickedRod->getRigidRodMotionState();
    btTransform btt;

    btt.setIdentity();
    motion->getWorldTransform(btt);
    btQuaternion rotation = btt.getRotation();
    btVector3 movement(0.,distance, 0.);
    if(_direction.x() > 0) {
        if(dx >= 0)
            btt.setOrigin(btt.getOrigin() - movement);
        else
            btt.setOrigin(btt.getOrigin() + movement);
    } else if(_direction.x() < 0) {
        if(dx >= 0)
            btt.setOrigin(btt.getOrigin() + movement);
        else
            btt.setOrigin(btt.getOrigin() - movement);
    } else {
        if(dy > 0)
            btt.setOrigin(btt.getOrigin() + btVector3(0, dy*dy*_direction.y(), 0));
        else
            btt.setOrigin(btt.getOrigin() - btVector3(0, dy*dy*_direction.y(), 0));
    }

    btt.setRotation(rotation);
    motion->setWorldTransform(btt);
	
    osgbBullet::MotionState* motion = _pickedRod->getRigidRodMotionState();
    btTransform btt;
    motion->getWorldTransform(btt);

    btQuaternion rotation = btt.getRotation();
    btVector3 offset = btt.getOrigin();
    rotation.setRotation( rotation.getAxis(), rotation.getAngle() + rotationAngle );
    btt.setOrigin(btVector3(0., 0., 0.));
    btt.setRotation(rotation);
    btt.setOrigin(offset);

    motion->setWorldTransform(btt);