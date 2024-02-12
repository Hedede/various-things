void perform_damage( stack& from, stack& to )
{
	// don't remember if there's any
	for ( auto func : to.pre_damage_defensive_abilities )
		func( to, from );

	for ( auto func : from.pre_damage_abilities )
		func( from, to );

	calc_and_apply_damage( from, to );

	// like "fire shield" which damages the attacker, think thorns
	for ( auto func : to.post_damage_defensive_abilities )
		func( to, from );

	// like "blind" which temporarily disables enemy
	for ( auto func : from.post_damage_abilities )
		func( from, to );
}

void perform_attack( stack& from, stack& to )
{
	// like "stun", makes enemy not retaliate
	for ( auto func : from.pre_attack_abilities )
		func( from, to );

	perform_damage( from, to );

	// if attacker dies from fire shield || was blinded, for example
	if (from.dead || from.disabled)
		return;

	// like "fear", makes enemy run away
	for ( auto func : from.post_attack_abilities )
		func( from, to );
}

void perform_retaliation( stack& from, stack& to )
{
	// most creatures retaliate only once per turn,
	// some have nlimited, griffin in h3 had 2 retaliations
	if (from.num_retaliations_available == 0)
		return;

	// err, what was it called, double retaliation damage temporarily, for example
	for ( auto func : from.pre_retaliation_abilities )
		func( from, to );

	perform_damage( from, to );

	if (from.dead || from.disabled)
		return;

	for ( auto func : from.post_retaliation_abilities )
		func( from, to );

	--from.num_retaliations_available;
}


void attack( stack& from, stack& to )
{
	perform_attacK(from, to);
	if (!to.dead && !to.disabled && !to.stunned) {
		perform_retaliation( to, from );
	}
	for(int i = 1, i < from.num_attacks; ++i) {
		peform_attack(from,to);
		if (zis_is_homm5_game_mechanics)
			if (!to.dead && !to.disabled && !to.stunned)
				perform_retaliation( to, from );
	}
	to.stunned = false;
}
