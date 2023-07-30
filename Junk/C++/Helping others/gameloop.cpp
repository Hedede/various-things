int main()
{
	double t = 0.0;
	const double dt = 0.01;

	double currentTime = hires_time_in_seconds();
	double accumulator = 0.0;

	while ( !quit )
	{
		double newTime = hires_time_in_seconds();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		while ( accumulator >= dt )
		{
			integrate( state, t, dt );
			accumulator -= dt;
			t += dt;
		}

		render( state );
	}
}
