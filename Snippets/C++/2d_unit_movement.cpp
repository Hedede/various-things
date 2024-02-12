std::vector<sf::Vector2f> path; //this is just a simple vector of tile (x,y) locations


void Imp::followPath(float tick){
    float dx,dy;
    bool reachingDestination=false;
    //calculate the current path tile the imp is occupying
    pathCurrent.x = floor(position.x / myLevel->tileset.twidth)*myLevel->tileset.twidth;
    pathCurrent.y = floor(position.y / myLevel->tileset.theight)*myLevel->tileset.twidth;

    typeof(path.begin()) it = find(all(path), pathCurrent); //find the current path tile the imp is occupying
    //pathnext is the location of the next tile in the path
    if (++it != path.end()){//path.size()>1){
        //++it;
        pathNext = *it;
    }
    else{
        //this must mean we're arriving at desitnation... TOOT TOOT
        pathNext = pathCurrent;
        reachingDestination = true;
    }
    dx = position.x - pathNext.x; //when "next" is to the right (larger) gives negative dx
    dy = position.y - pathNext.y; //when next is below (larger) gives negative dy
    if (abs(dx) > speed*tick || abs(dy) > speed*tick){
        if (abs(dx) > abs(dy)){
            if (position.y - pathCurrent.y > speed*tick)
                setDirection(UP);
            else{
                if (dx < 0)
                    setDirection(RIGHT);
                else
                    setDirection(LEFT);
            }
        }
        else{
            if (position.x - pathCurrent.x > speed*tick)
                setDirection(LEFT);
            else{
                if (dy < 0)
                    setDirection(DOWN);
                else
                    setDirection(UP);
            }
        }
        move(tick);
    }
    else{
        if (reachingDestination){
            //destination reached!
            if (hasCoffee){
                requestExit();
            }
            else{
                requestGather();
            }
        }
        else {
            //this happens if the imp gets a litt bit "stuck" on its journey
            move(tick);
        }
    }
}