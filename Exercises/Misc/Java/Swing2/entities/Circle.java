package java4.entities;

/**
 * Created by Hedede on 02.06.2014.
 */
public class Circle extends Ellipsis {
    private int[] pointsY;
    private int[] pointsX;
    private int pointsCount;

    public void setRadius(int r) {
        this.setBoundX(r / 2);
        this.setBoundY(r / 2);
    }
}
