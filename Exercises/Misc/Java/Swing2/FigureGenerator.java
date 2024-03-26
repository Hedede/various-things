package java4;

import java.awt.*;
import java.util.Random;

import java4.entities.*;

/**
 * Created by Hedede on 02.06.2014.
 */
public class FigureGenerator {
    private static Random random = new Random();

    public static Figure generateFigure(int x, int y) {
        int index = random.nextInt(6);
        SizedFigure figure;
        switch (index) {
            case 0:
                figure = new Circle();
                ((Circle) figure).setRadius(100 + random.nextInt(100));
                break;
            case 1:
                figure = new Ellipsis();
                ((Ellipsis) figure).setBoundX(100 + random.nextInt(100));
                ((Ellipsis) figure).setBoundY(100 + random.nextInt(100));
                break;
            case 2:
                figure = new java4.entities.Rectangle();
                ((java4.entities.Rectangle) figure).setBoundX(100 + random.nextInt(100));
                ((java4.entities.Rectangle) figure).setBoundY(100 + random.nextInt(100));
                break;
            default:
                figure = new RegularPolygon();
                ((RegularPolygon) figure).setBoundX(100 + random.nextInt(100));
                ((RegularPolygon) figure).setBoundY(100 + random.nextInt(100));
                ((RegularPolygon) figure).setAngle(random.nextDouble());
                ((RegularPolygon) figure).setNumPoints(3 + random.nextInt(5));
                break;
        }
        figure.setColor(randomColor());
        figure.setX(x);
        figure.setY(y);
        return figure;
    }

    public static Color randomColor() {
        int index = random.nextInt(8);
        switch (index) {
            case 0:
                return Color.red;
            case 1:
                return Color.green;
            case 2:
                return Color.blue;
            case 3:
                return Color.cyan;
            case 4:
                return Color.magenta;
            case 5:
                return Color.yellow;
            case 6:
                return Color.orange;
            case 7:
                return Color.pink;
            default:
                return Color.gray;
        }
    }
}
