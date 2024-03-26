package java4;

import java4.entities.*;

/**
 * Created by Hedede on 02.06.2014.
 */
public class AnimationManager implements Runnable {
    private final DrawPanel drawPanel;
    private volatile boolean stopped = false;

    public AnimationManager(DrawPanel drawPanel) {
        this.drawPanel = drawPanel;
    }

    public void setStopped(boolean stopped) {
        this.stopped = stopped;
    }

    @Override
    public void run() {
        while (!stopped) {
            for (Figure figure : drawPanel.getFigures()) {
                figure.animate(10,
                        drawPanel.getWidth(),
                        drawPanel.getHeight());
            }
            drawPanel.repaint();
            try {
                Thread.sleep(50);
            } catch (InterruptedException e) {
                System.err.println("Failed to sleep: "
                        + e.getMessage());
            }
        }
    }
}
