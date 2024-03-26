package java4.entities;

import java.awt.*;

/**
 * Created by Hedede on 02.06.2014.
 */
public class RegularPolygon extends SizedFigure {
    private int pointsX[] = new int[24];
    private int pointsY[] = new int[24];
    private int numPoints;
    private double angle;

    public void setRadius(int r) {
        this.setBoundX(r/2);
        this.setBoundY(r/2);

        this.update();
    }

    @Override
    public void setX(int boundX) {
        super.setX(boundX);

        this.update();
    }

    @Override
    public void setY(int boundY) {
        super.setY(boundY);

        this.update();
    }

    @Override
    public void setBoundX(int boundX) {
        super.setBoundX(boundX);

        this.update();
    }

    @Override
    public void setBoundY(int boundY) {
        super.setBoundY(boundY);

        this.update();
    }

    public void setNumPoints(int numPoints) {
        if(numPoints < 3 || numPoints > 24) {
            return;
        }

        this.numPoints = numPoints;

        this.update();
    }

    public int getNumPoints() {
        return numPoints;
    }

    public void setAngle(double angle) {
        this.angle = angle;

        this.update();
    }

    public double getAngle() {
        return angle;
    }


    @Override
    public void draw(Graphics graphics) {
        super.draw(graphics);
        graphics.fillPolygon(pointsX, pointsY, numPoints);
    }

    @Override
    public void drawOutline(Graphics graphics) {
        super.drawOutline(graphics);
        graphics.drawPolygon(pointsX, pointsY, numPoints);
    }

    private void update() {
        for (int i = 0; i < numPoints; i++)
        {
            pointsX[i] = this.getX() + (int) (
                    getHalfBoundX() * Math.sin(2 * i * Math.PI / numPoints));
            pointsY[i] = this.getY() + (int)(
                    getHalfBoundY() * Math.cos(2 * i * Math.PI / numPoints));
        }
    }
}
