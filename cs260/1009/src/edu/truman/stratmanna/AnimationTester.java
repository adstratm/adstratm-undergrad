package edu.truman.stratmanna;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.Timer;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

/**
 * This program implements an animation that moves a car shape.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 8 October 2015
 */
public class AnimationTester
{
   public static void main(String[] args)
   {
      new AnimationTester();  
   }
   
   /**
    * Create a new AnimationTester.
    * This code will display the graphics of interest,
    * including a button to start and stop the motion
    * as well as a slider to control size.
    */
   public AnimationTester()
   {
      JFrame frame = new JFrame();
      final JPanel controls = new JPanel();
      
      shape = new CarShape(NO_DISTANCE, NO_DISTANCE, CAR_WIDTH);
      
      icon = new ShapeIcon(shape, ICON_WIDTH, ICON_HEIGHT);
      
      
      final JButton stopStarter = new JButton("Start");
      stopStarter.addActionListener(new
            ActionListener()
            {
               public void actionPerformed(ActionEvent event)
               {
                  if (stopStarter.getText().equals("Start"))
                  {
                     run();
                     stopStarter.setText("Pause");
                  }
                  else if (stopStarter.getText().equals("Pause"))
                  {
                     stop();
                     stopStarter.setText("Resume");
                  }
                  else if (stopStarter.getText().equals("Resume"))
                  {
                     run();
                     stopStarter.setText("Pause");
                  }
               }
            });
      
      
      
      final JSlider sizeSlider = new JSlider(SLIDER_MIN, SLIDER_MAX);
      sizeSlider.addChangeListener(new
            ChangeListener()
             {
                 public void stateChanged(ChangeEvent e) 
                 {
                     JSlider source = (JSlider)e.getSource();
                     int size = (int)source.getValue();
                     resize(size);
                     label.setIcon(icon);
                     label.repaint();
                 }
             });
      
      
      
      controls.setLayout(new BoxLayout(controls, 3));
      controls.add(stopStarter);
      JLabel sizeLabel = new JLabel("Size");
      controls.add(sizeLabel);
      controls.add(sizeSlider);
      stopStarter.setAlignmentX(Component.CENTER_ALIGNMENT);
      sizeLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
      
      
      label = new JLabel(icon);
      frame.setLayout(new BorderLayout());
      frame.add(label, BorderLayout.CENTER);
      frame.add(controls, BorderLayout.SOUTH);
      
      frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      frame.pack();
      frame.setVisible(true);
      
      final int DELAY = TIMER_DELAY;
      // milliseconds between timer ticks
      t = new Timer(DELAY, new
            ActionListener()
            {
               public void actionPerformed(ActionEvent event)
               {
                  shape.translate(VELOCITY_X, VELOCITY_Y);
                  label.repaint();
               }
            });
   }
   
   
   /**
    * Starts the car in motion.
    */
   public void run()
   {
      t.start();
   }
   
   /**
    * Halts the motion of the car.
    */
   public void stop()
   {
      t.stop();
   }
   
   
   /**
    * Resizes the car given a scaling factor
    * @param j The scaling factor, with 50
    * corresponding to no change in size.
    */
   public void resize(int j)
   {
      shape.setSize(j);
      icon = new ShapeIcon(shape, ICON_WIDTH, ICON_HEIGHT);
   }
   
   
   
   
   private JLabel label;
   private ShapeIcon icon;
   private final MoveableShape shape;
   private final Timer t;
   private static final int ICON_WIDTH = 400;
   private static final int ICON_HEIGHT = 100;
   private static final int CAR_WIDTH = 100;
   private static final int TIMER_DELAY = 100;
   private static final int VELOCITY_X = 1;
   private static final int VELOCITY_Y = 0;
   private static final int NO_DISTANCE = 0;
   private static final int SLIDER_MIN = 25;
   private static final int SLIDER_MAX = 75;
}