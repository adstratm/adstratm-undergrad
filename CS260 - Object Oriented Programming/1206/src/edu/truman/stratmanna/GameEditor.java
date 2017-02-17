package edu.truman.stratmanna;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.text.SimpleDateFormat;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.Timer;


/**
 * A program that manages a moving card game
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 6 December 2015
 */
public class GameEditor
{
   public static void main(String[] args)
   {
      new GameEditor();     
   }
   
   /**
    * Construct a new GameEditor object.  This sets up the 
    * graphical components needed to correctly display the game
    * as well as provide options to the user for playing the game.
    */
   public GameEditor()
   {
      final JFrame frame = new JFrame();
      frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      final JPanel topBorder = new JPanel();
      JLabel welcomeMessage = new JLabel(
            "Press a number 1 through 5 to start a game");
      topBorder.add(welcomeMessage);
      frame.add(topBorder, BorderLayout.NORTH);
      frame.setSize(FRAME_SIZE, FRAME_SIZE);
      frame.setVisible(true);
      frame.setFocusable(true);
      
      //Listen for a key press to make the game start.
      frame.addKeyListener(new KeyAdapter()
            {
               public void keyPressed(KeyEvent e)
               {
                  char theCharacter = e.getKeyChar();
                  if(theCharacter == '1' ||
                        theCharacter == '2' ||
                        theCharacter == '3' ||
                        theCharacter == '4' ||
                        theCharacter == '5')
                  {
                     timeLeft = TIME_PER_GAME;
                     frame.getContentPane().removeAll();
                     frame.add(topBorder, BorderLayout.NORTH);
                     scene = new GameSceneComponent( DIFFICULTY_OFFSET
                           - Character.getNumericValue(theCharacter));
                     frame.add(scene, BorderLayout.CENTER);
                     final JLabel countdownLabel = new JLabel("");
                     final JPanel bottomBorder = new JPanel();
                     bottomBorder.add(countdownLabel);
                     frame.add(bottomBorder, BorderLayout.SOUTH);
                     frame.setVisible(true);
                     
                     // Create timer listener to repaint the scene.
                     // The timer listener can be an anonymous class
                     ActionListener listener = new ActionListener() 
                     {
                        public void actionPerformed(ActionEvent e)
                        {
                           scene.repaint();
                        }
                     };
                     timer = new Timer(1, listener);
                     
                     
                     //start the timer and threads
                     timer.start();
                     scene.startMotion();
                     
                     
                     //timer to handle the countdown
                     ActionListener countDown = new ActionListener()
                     {
                         public void actionPerformed(ActionEvent e)
                         {
                             timeLeft -= COUNTDOWN_STEP;
                             if(scene.doCardsRemain() == false)
                             {
                                JOptionPane.showMessageDialog(frame,
                                      "YOU WIN!");
                                countdownTimer.stop();
                             }
                             SimpleDateFormat dateFormat = 
                                   new SimpleDateFormat("ss");
                             countdownLabel.setText(dateFormat.format(
                                   timeLeft));
                             if(timeLeft<=0)
                             {
                                 if(scene.doCardsRemain())
                                 {
                                    System.out.print(scene.doCardsRemain());
                                    frame.getContentPane().removeAll();
                                    frame.add(topBorder, BorderLayout.NORTH);
                                    JOptionPane.showMessageDialog(frame,
                                          "YOU LOSE - TRY AGAIN");
                                    frame.repaint();
                                 }
                                 countdownTimer.stop();
                             }
                         }
                     };
                     //
                     countdownTimer = new Timer(COUNTDOWN_STEP,
                           countDown);
                     
                     //start the countdown timer.

                     countdownTimer.start();
                  } 
               }
            });
   }
   
   
   private Timer timer;
   private GameSceneComponent scene;
   private Timer countdownTimer;
   private int timeLeft;
   private static final int TIME_PER_GAME = 46000;
   private static final int FRAME_SIZE = 600;
   private static final int DIFFICULTY_OFFSET = 6;
   private static final int COUNTDOWN_STEP = 100;

}