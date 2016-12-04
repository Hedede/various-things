
Vector3d<f32> CNavigatorGround::calculateAvoidanceVector(Vector3d<f32> const& direction)
{
	CExceptHimselfCallback callback(unit_->getPhysicsShape());

	Vector3d<f32> position = unit_->getPosition();
	Vector3d<f32> velocity = locomotor_->getVelocity();

	Line3d<f32> ray(position, position + 0.5*velocity);
	
	world_->castRay(ray.start, ray.end, &callback);

	if(callback.hasHit) {
		callback.collObject = 0;

		f32 const facing = math::DegToRad(90.0f + unit_->getFacing());
		Vector3d<f32> heading(sin(facing),0,cos(facing));

		ray.end = position + 0.5*velocity + 3*heading;
		world_->castRay(ray.start, ray.end, &callback);

		if(!callback.hasHit) {
			// move in this direction
			return;
		}
		
		ray.end = position + 0.5*velocity - 3*heading;
		world_->castRay(ray.start, ray.end, &callback);
		
		if(!callback.hasHit) {
			// move in this direction
			return;
		} else {
			// panic
			return;
		}
	}
	
	return direction;
}