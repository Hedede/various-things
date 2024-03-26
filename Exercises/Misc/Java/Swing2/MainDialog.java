package java4;

import java4.entities.Circle;
import java4.entities.Figure;
import java4.entities.SizedFigure;

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.io.*;
import java.util.List;

/**
 * Created by Hedede on 02.06.2014.
 */
public class MainDialog extends JDialog {
    private JPanel contentPane;
    private DrawPanel drawPanel;

    private JToolBar mainToolbar;

    private JButton buttonPlay;
    private JButton buttonStop;
    private JButton buttonStore;
    private JButton buttonLoad;
    private JButton buttonExit;

    private AnimationManager entityManager;

    public MainDialog() {
        contentPane = new JPanel();
        contentPane.setPreferredSize(new Dimension(900, 600));

        setContentPane(contentPane);

        setModal(true);

        createAnimationButtons();
        createSerializationButtons();
        createUIComponents();

        getRootPane().setDefaultButton(buttonPlay);
    }

    public static void main(String[] args) {
        // для получения варианта задания, полученное значение: 3
        // System.out.println((int)(Math.random() * 7));
        MainDialog mainDialog = new MainDialog();
        mainDialog.pack();
        mainDialog.setResizable(false);
        mainDialog.setTitle("Draw dialog");
        mainDialog.setVisible(true);
        System.exit(0);
    }

    private void createAnimationButtons() {
        buttonPlay = new JButton("Play animation");
        buttonPlay.setPreferredSize(new Dimension(120, 20));
        buttonPlay.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                entityManager.setStopped(false);
                (new Thread(entityManager)).start();
                buttonStop.setEnabled(true);
                buttonPlay.setEnabled(false);
            }
        });

        buttonStop = new JButton("Stop animation");
        buttonStop.setPreferredSize(new Dimension(120, 20));
        buttonStop.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                entityManager.setStopped(true);
                buttonStop.setEnabled(false);
                buttonPlay.setEnabled(true);
            }
        });

        buttonStop.setEnabled(false);
        this.add (buttonPlay);
        this.add (buttonStop);
    }

    private void createSerializationButtons() {
        buttonLoad = new JButton("DeSerialize");
        buttonLoad.setPreferredSize(new Dimension(120, 20));
        buttonLoad.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                try {
                    ObjectInputStream in = new ObjectInputStream(new FileInputStream("objects.bin"));
                    drawPanel.clearFigures();

                    while(true) {
                        try {
                            Object o = in.readObject();

                            if (o instanceof SizedFigure) {
                                    drawPanel.addFigure((SizedFigure) o);
                            } else {
                                System.out.println("invalid object type");
                            }
                        }
                        catch (EOFException exc) {
                            in.close();
                            break;
                        } catch (InvalidClassException ex) {
                            System.out.println(ex.getMessage());
                        }

                    }

                    System.out.println("loaded objects");
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            }
        });

        buttonStore = new JButton("Serialize");
        buttonStore.setPreferredSize(new Dimension(120, 20));
        buttonStore.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                try {
                    ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream("objects.bin"));
                    for (Figure figure : drawPanel.getFigures()) {
                        out.writeObject(figure);
                    }

                    System.out.println("saved objects");

                    out.close();
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            }
        });

        this.add (buttonLoad);
        this.add (buttonStore);
    }

    private void createUIComponents() {
        buttonExit = new JButton("Close dialog");
        buttonExit.setPreferredSize(new Dimension(120, 20));
        buttonExit.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                dispose();
            }
        });

        drawPanel = new DrawPanel();
        drawPanel.setPreferredSize(new Dimension(800, 600));
        drawPanel.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                drawPanel.addFigure(
                        FigureGenerator.generateFigure(
                                e.getX(),
                                e.getY()
                        )
                );
            }
        });

        this.entityManager = new AnimationManager(drawPanel);

        this.add(buttonExit);
        this.add(drawPanel);
    }
}
