package java2;

import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.Constructor;
import java.util.ArrayList;

import java.util.logging.Level;
import java.util.logging.Logger;

public class Java2 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        /* Singular because bird is a mineral */
        ArrayList<Bird> bird = new ArrayList<>();
        bird.add(new Goose());
        bird.add(new Penguin());
        bird.add(new Penguin(4.f,5.f));
        bird.add(new Goose(5.f,4.f));
        bird.add(new Goose(40.f,50.f));
        bird.add(new Penguin(40.f,50.f));
        
        BirdController.Instance.reLocate(10.f, 10.f);
        BirdController.Instance.reLocate(0.f, 0.f);        
        
        System.out.println("Press any key to exit.");
        try {
            System.in.read();
        } catch (IOException ex) {
            Logger.getLogger(Java2.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
}
