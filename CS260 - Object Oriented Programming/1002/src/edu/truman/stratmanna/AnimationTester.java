package edu.truman.stratmanna;

import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.Timer;

/**
 * This program implements an animation that moves a car shape.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * 1 October 2015
 */
public class AnimationTester
{
   public static void main(String[] args)
   {
      new AnimationTester();   
   }
   
   /**
    * Create a new AnimationTester.
    * This code will display the graphics of interest.
    */
   public AnimationTester()
   {
      JFrame frame = new JFrame();
      final MoveableShape shape = new CarShape(NO_DISTANCE, NO_DISTANCE, CAR_WIDTH);
      
      ShapeIcon icon = new ShapeIcon(shape, ICON_WIDTH, ICON_HEIGHT);
      
      final JLabel label = new JLabel(icon);
      frame.setLayout(new FlowLayout());
      frame.add(label);
      
      frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      frame.pack();
      frame.setVisible(true);
      
      final int DELAY = TIMER_DELAY;
      // milliseconds between timer ticks
      Timer t = new Timer(DELAY, new
            ActionListener()
            {
               public void actionPerformed(ActionEvent event)
               {
                  shape.translate(FORWARD, NO_DISTANCE);
                  label.repaint();
               }
            });
      t.start();
   }
   
   private static final int ICON_WIDTH = 400;
   private static final int ICON_HEIGHT = 100;
   private static final int CAR_WIDTH = 100;
   private static final int TIMER_DELAY = 100;
   private static final int FORWARD = 1;
   private static final int NO_DISTANCE = 0;
}
