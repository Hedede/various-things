package java3.figures;

import java.awt.Graphics;

//public class Polygon extends Dot {
public class Polygon implements Figure{
    private int[] Xs; // read 'exes'
    private int[] Ys; // read 'whys'
    private int mPoints; // read 'whys'    
    
    public Polygon(int[] pointsX, int[] pointsY, int points) throws Throwable {
        //int CenterX = 0;
        //int CenterY = 0;
        
        if(pointsX.length != points || pointsY.length != points)
        {
            //too lazy to implement this propely
            throw new Throwable("blah-blah mismatch");
        }
        
        Xs = pointsX;
        Ys = pointsY;
        mPoints = points;
             
       /* for(int i=0; i < points; i++)
        {
            CenterX += pointsX[i];
            CenterY += pointsY[i];
        }
        
        CenterX /= points;
        CenterY /= points;
               
        for(int i=0; i < points; i++)
        {
            Xs[i] -= CenterX;
            Ys[i] -= CenterY;
        }
        
        super(centerX, centerY);*/
        
    }

    public int getPointX(int id) {
        return Xs[id];
    }
    
    public int getPointY(int id) {
        return Ys[id];
    }
    
    public int getPointsAmount() {
        return mPoints;
    }

    @Override
    public void draw(Graphics g) {
        g.drawPolygon(Xs, Ys, mPoints); 
        // oh, I love this IDE! it makes me even more lazy        
    }
}
