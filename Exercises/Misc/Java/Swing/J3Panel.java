package java3;

// why would did they call it 'swing'?
// sounds like a thrid-party library
import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Iterator;

//whoops, bad decision to call package 'java3'
import java3.figures.Figure;

// since we invented java3, I thought it would be 
// appropriate to call this class 'J3Panel'
//public class MDrawPanel extends JPanel {
public class J3Panel extends JPanel {
    ArrayList<Figure> figures = new ArrayList<Figure>();

    public void addFigure(Figure figure) {
        figures.add(figure);
        repaint();
    }

    @Override public void paint(Graphics g) {
        super.paint(g);
        for (Figure figure : figures) {
            figure.draw(g);
        }
    }
}
