package java3.figures;

import java.awt.*;

public class Dot implements Figure {
    private int centerX;
    private int centerY;
	
    public Dot(int pcenterX, int pcenterY) {
        centerX = pcenterX;
        centerY = pcenterY;
    }

    public int getCenterX() {
        return centerX;
    }

    public int getCenterY() {
        return centerY;
    }
    
    @Override public void draw(Graphics g) {
        g.fillOval(getCenterX(), getCenterY(), 3, 3);
    }    
}
