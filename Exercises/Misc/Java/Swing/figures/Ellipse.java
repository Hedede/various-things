/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package java3.figures;

import java.awt.Graphics;

/**
 *
 * @author Hedede
 */
public class Ellipse extends Rect {
    public Ellipse(int centerX, int centerY, int width, int height) {
        super(centerX, centerY, width, height);
    }

    @Override
    public void draw(Graphics g) {
        g.drawOval(getCenterX(), getCenterY(), getWidth(), getHeight());
    }
}
