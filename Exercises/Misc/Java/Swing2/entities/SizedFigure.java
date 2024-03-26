package java4.entities;

/**
 * Created by Hedede on 02.06.2014.
 */
public class SizedFigure extends Figure {
    private int boundX;
    private int boundY;

    public int getBoundX() {
        return boundX;
    }

    public int getHalfBoundX() {
        return boundX/2;
    }

    public void setBoundX(int boundX) {
        this.boundX = boundX;
    }

    public int getBoundY() {
        return boundY;
    }

    public int getHalfBoundY() {
        return boundY/2;
    }

    public void setBoundY(int boundY) {
        this.boundY = boundY;
    }

    @Override
    public void animate(int step, int frameX, int frameY) {
        setX(getX() + step);
        setY(getY() + step);
        if (getX()+getHalfBoundX() > frameX) {
            setX(getHalfBoundX());
        }
        if (getY()+getHalfBoundY() > frameY) {
            setY(getHalfBoundY());
        }
    }
}
