while (!done) {
    if(user_indicates_they_want_to_quit) {
        done = true;
    }

    // Render a frame
    if(false == video->renderOneFrame()) {
        done = true;
        // Game quits if render failed
    }

    // Physics handling part of the loop
    /* This, like the rendering, ticks every time around.
    Bullet does the interpolation for us. */

    time_physics_curr = getMilliseconds();

    // Tick the bullet world. Keep in mind that bullet takes seconds, and we measure time in milliseconds.
    mWorld->stepSimulation(((float)(time_physics_curr - time_physics_prev))/1000.0, 10);

    time_physics_prev = time_physics_curr;


    // Game Clock part of the loop
    /*  This ticks once every TickMs milliseconds on average */
    long long dt = getMilliseconds() - time_gameclock;

    while(dt >= TickMs) {
        dt -= TickMs;
        time_gameclock += TickMs;

        // Pulse the input every TickMs milliseconds
        input->do_all_your_input_processing();

        // Everything on your system that needs to happen once every game tick should be processed here
        TWTickable::tickAll(TickMs);
    }
}