package java4.entities;

import java.awt.*;

/**
 * Created by Hedede on 02.06.2014.
 */
public class Ellipsis extends SizedFigure {
    @Override
    public void draw(Graphics graphics) {
        super.draw(graphics);
        graphics.fillOval(
                getX()-getHalfBoundX(),
                getY()-getHalfBoundY(),
                getBoundX(),getBoundY());
    }

    @Override
    public void drawOutline(Graphics graphics) {
        super.drawOutline(graphics);
        graphics.drawOval(
                getX()-getHalfBoundX(),
                getY()-getHalfBoundY(),
                getBoundX(),getBoundY());
    }
}
