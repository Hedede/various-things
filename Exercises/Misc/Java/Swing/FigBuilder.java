/*
FYI: Fig is a soft pear-shaped fruit with sweet dark flesh and many small seeds, eaten fresh or dried.
So we'll build fruits! YAAAAY!
*/

package java3;

import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;
import java3.figures.*;

public class FigBuilder 
{
    private Random random = new Random();
    
    public Figure buildPure(int x, int y, int a, int b)
    {
        int fig = random.nextInt(6);
        switch (fig) 
        {
        default:
            System.out.println("Dot");
            return new Dot(x, y);
        case 1:
            System.out.println("Ellipse");
            return new Ellipse(x, y, a, b);
        case 2:
            System.out.println("Circle");
            // so tempted to define a macro and run this code through C preprocessor *evil*
            return new Ellipse(x, y, a, a);
        case 3:
            System.out.println("Rectangle");
            return new Rect(x, y, a, b);
        case 4:
            System.out.println("Square");
            return new Rect(x, y, a, a);
        case 5:
            System.out.println("Triangle");
            int rx1 = random.nextInt(a);
            int rx2 = random.nextInt(a);
            int ry1 = random.nextInt(b);
            int ry2 = random.nextInt(b);
            int[] dotsX = {x,x-rx1,x+rx2};
            int[] dotsY = {y+ry1,y,y-ry2};
            try {
                return new Polygon(dotsX,dotsY,3);
            } catch (Throwable ex) {
                Logger.getLogger(FigBuilder.class.getName()).log(Level.SEVERE, null, ex);
                return new Dot(x, y);
            }
        }
    }

    public Figure build(int x, int y)
    {
        int diva = -30 + random.nextInt(60);
        int divb = -30 + random.nextInt(60);
        
        // bloat b if diva and divb are too similar
        do
        {
            divb += 10;        
        } while(Math.abs(diva-divb) < 5);

        return buildPure(x, y, 50 + diva, 50 + divb);
    }
}
