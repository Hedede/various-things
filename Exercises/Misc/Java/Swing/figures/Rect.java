package java3.figures;

import java.awt.Graphics;

public class Rect extends Dot {
    private int width;
    private int height;

    public Rect(int centerX, int centerY, int width, int height) {
        super(centerX, centerY);
        this.width = width;
        this.height = height;
    }

    public int getWidth() {
        return width;
    }
    public int getHeight() {
        return height;
    }

    @Override
    public void draw(Graphics g) {
        g.drawRect(getCenterX(), getCenterY(), getWidth(), getHeight());
    }
}
