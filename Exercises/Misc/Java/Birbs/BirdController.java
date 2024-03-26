package java2;

import java.util.ArrayList;
import java.util.Iterator;

public class BirdController {
    public static final BirdController Instance = new BirdController();
    private ArrayList<Bird> BirdList = new ArrayList<>();;
    
    public void reLocate(float X, float Y)
    {
        Iterator<Bird> iterator = BirdList.iterator();
        System.out.println("Starting bird migration");
        while(iterator.hasNext())
        {
            Bird nextbird = iterator.next();
            float time = nextbird.move(X,Y); 
            System.out.println(nextbird.getClass().getName() + " arrived at location (" + X + ", " + Y + "), it took " + time + "h to get there.");
        }
        System.out.println("Birds reloated");
    }    
    
    public void Register(Bird bird)
    {
        BirdList.add(bird);
    }
}
