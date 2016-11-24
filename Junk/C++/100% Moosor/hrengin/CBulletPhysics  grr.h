if(model->primitives.size() > 1) {
	// просес компоунд шейпз
} else if(model->primitives.size() > 0) {
	SPrimitive primitive = model->primitives.back();
	btCollisionShape * shape = createPrimitiveShape(primitive);
	
	bool needTransform = 
	primitive.offset[0] != 0 || primitive.offset[1] != 0
	|| primitive.offset[2] != 0 || primitive.rotation[0] != 0
	|| primitive.rotation[1] != 0 || primitive.rotation[3] != 0;

	if(needTransform) {
		btCompoundShape* compound = new btCompoundShape();
		btTransform localTransform;

		localTransform.setIdentity();
		localTransform.setOrigin(btVector3(primitive.offset[0],primitive.offset[1],primitive.offset[2]));
		localTransform.setRotation(btQuaternion(primitive.rotation[0],primitive.rotation[1],primitive.rotation[2]));

		compound->addChildShape(localTransform,shape);
		
		collisionShapes_.push_back(compound);
	} else {
		collisionShapes_.push_back(shape);
	}
	
превращается… превращается в 
	