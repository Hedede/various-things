
Vector3d<f32> CNavigatorGround::calculateAvoidanceVector(Vector3d<f32> const& direction)
{
	CExceptHimselfCallback callback(unit_->getPhysicsShape());
	callback.collFilter = physics::CollisionFilter(COL_UNIT,COL_UNIT);

	Vector3d<f32> position = unit_->getPosition();
	Vector3d<f32> velocity = locomotor_->getVelocity();

	Line3d<f32> ray(position, position + 1.5*velocity);
	
	world_->castRay(ray.start, ray.end, &callback);
	rundurur()->drawLine(ray.start+Vector3d<f32>(0,0.5,0),
		ray.end+Vector3d<f32>(0,0.5,0),
		Vector3d<f32>(0,255,0));

	if(!callback.hasHit()) {
		return direction;
	}

	callback.collObject = 0;

	f32 const facing = math::DegToRad(90.0f + unit_->getFacing());
	Vector3d<f32> heading(sin(facing),0,cos(facing));

	ray.end = position + 1.5*velocity + 3*heading;
	world_->castRay(ray.start, ray.end, &callback);
	rundurur()->drawLine(ray.start+Vector3d<f32>(0,0.5,0),
		ray.end+Vector3d<f32>(0,0.5,0),
		Vector3d<f32>(0,0,255));

	if(!callback.hasHit()) {
		// move in this direction
		return ray.vector();
	}
		
	ray.end = position + 1.5*velocity - 3*heading;
	world_->castRay(ray.start, ray.end, &callback);
	rundurur()->drawLine(ray.start+Vector3d<f32>(0,0.5,0),
		ray.end+Vector3d<f32>(0,0.5,0),
		Vector3d<f32>(255,0,0));
		
	if(!callback.hasHit()) {
		// move in this direction
		return ray.vector();
	} else {
		// panic
		return Vector3d<f32>::zero;
	}
	
	return direction;
}