package java4.entities;

import java.awt.*;

/**
 * Created by Hedede on 02.06.2014.
 */
public class Rectangle extends SizedFigure {
    @Override
    public void draw(Graphics graphics) {
        super.draw(graphics);
        graphics.fillRect(
                getX()-getHalfBoundX(),
                getY()-getHalfBoundY(),
                getBoundX(),
                getBoundY()
        );
    }

    @Override
    public void drawOutline(Graphics graphics) {
        super.drawOutline(graphics);
        graphics.drawRect(
                getX()-getHalfBoundX(),
                getY()-getHalfBoundY(),
                getBoundX(),
                getBoundY()
        );
    }
}
