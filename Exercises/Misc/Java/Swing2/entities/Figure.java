package java4.entities;

import java.awt.*;

/**
 * Created by Hedede on 02.06.2014.
 */
public class Figure implements Entity {
    private int x;
    private int y;

    private Color color;

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
    }

    public void setColor(Color color) {
        this.color = color;
    }

    public Color getColor() {
        return color;
    }

    /*
    Добавить циклическое смещение фигур вправо по координатной оси x с
     определённым шагом. При выходе фигуры за границы формы, она должна
     возвращаться с обратной стороны.
     */

    @Override
    public void draw(Graphics graphics) {
        graphics.setColor(getColor());
    }

    public void drawOutline(Graphics graphics) {
        graphics.setColor(Color.black);
    }

    @Override
    public void animate(int step, int frameX, int frameY) {
        setX(getX() + step);
        setY(getY() + step);
        if (getX() > frameX) {
            setX(0);
        }
        if (getY() > frameY) {
            setY(0);
        }
    }
}
