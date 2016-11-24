

Vector3d advance (f32 timestep)
{
	switch(state)
	{
	default:
	case idle:
		return;
	case moving:
		Vector3d v = tryMove()
		if(obstructed) {
			if(obstructor->who() == friend) {
				obstructor->pleaseFuckOff()
				state = waitForPath;
			} elseif(cancrush) {
				state = moving;
			} elseif(unit->agressive() && obstructor->who() == enemy) {
				unit->attack(obstructor);
				state = waitForEnemyCorpse;
			} else {
				state = avoid;
			}
		}
	case avoid:
		Vector3d v = tryAvoid()
		if(!obstructed) {
			state = vernucco
		}
	case vernuccp:
		v2 = tryMoveBack()
		if(!obstructed) {
			v = v2
		} else {
			v = continueAvoid()
		}
}