package java4;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

import java4.entities.*;

/**
 * Created by Hedede on 02.06.2014.
 */
public class DrawPanel extends JPanel {
    List<Figure> figures = new ArrayList<Figure>();

    public void addFigure(Figure figure) {
        figures.add(figure);
        repaint();
    }

    public void clearFigures() {
        figures.clear();
    }

    public List<Figure> getFigures() {
        return figures;
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);
        g.drawString(
                "DrawPanel",
                getWidth() / 2,
                getHeight() / 2
        );

        for (Figure figure : figures) {
            figure.draw(g);
            figure.drawOutline(g);
        }
    }
}