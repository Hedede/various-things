package java3;

import java.awt.Dimension;
import javax.swing.*;
import java.awt.event.*;

public class J3Dialog1 extends JDialog {
    private JPanel contentPane;
    private JButton buttonOK;
    private JButton buttonCancel;
    private J3Panel drawPanel;
    
            
    //this.add(buttonOK);
                
    public J3Dialog1() {
        contentPane = new JPanel();
        contentPane.setPreferredSize(new Dimension(800, 700));
        setContentPane(contentPane);
        setModal(true);
        setResizable(false);
                        
        drawPanel = new J3Panel();
        drawPanel.setPreferredSize(new Dimension(800, 600));
        this.add (drawPanel);
        
        buttonOK = new JButton("OK");
        buttonOK.setPreferredSize(new Dimension(80, 30));
        this.add (buttonOK);
        
        buttonCancel = new JButton("Cancel");
        buttonCancel.setPreferredSize(new Dimension(80, 30));
        this.add (buttonCancel);
                
        getRootPane().setDefaultButton(buttonOK);

        buttonOK.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onOK();
            }
        });

        buttonCancel.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onCancel();
            }
        });

        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                onCancel();
            }
        });
    
        contentPane.registerKeyboardAction(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onCancel();
            }
        }, KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);
        
        drawPanel.addMouseListener(new MouseAdapter() 
        {
            @Override 
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);
                drawPanel.addFigure(new FigBuilder().build(e.getX(), e.getY()));
            }
        });
        
    }
    
    private void onOK() {
        dispose();
    }

    private void onCancel() {
        dispose();
    }

    public static void main(String[] args) {
        J3Dialog1 dialog = new J3Dialog1();
        dialog.pack();
        dialog.setVisible(true);
        System.exit(0);
    }

    private void createUIComponents() {
        drawPanel = new J3Panel();
    }
}
