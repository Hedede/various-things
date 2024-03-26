package java4.entities;

import java.awt.*;
import java.io.Serializable;

/**
 * Created by Hedede on 02.06.2014.
 */
public interface Entity extends Serializable {
    void draw(Graphics graphics);
    void drawOutline(Graphics graphics);
    void animate(int step, int frameX, int frameY);
}
