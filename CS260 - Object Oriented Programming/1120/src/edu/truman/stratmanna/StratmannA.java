package edu.truman.stratmanna;

import java.awt.BorderLayout;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;


/**
 * A program that manages a memory card game
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 19 November 2015
 */
public class StratmannA
{
   public static void main(String[] args)
   {
      new StratmannA();     
   }
   
   /**
    * Construct a new GameEditor object.  This sets up the 
    * graphical components needed to correctly display the game
    * as well as provide options to the user for playing the game.
    */
   public StratmannA()
   {
      final JFrame frame = new JFrame();
      frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      final JPanel topBorder = new JPanel();
      JButton welcomeMessage = new JButton("Press 'c' or 'r' to start a game");
      welcomeMessage.setEnabled(false);
      topBorder.add(welcomeMessage);
      frame.add(topBorder, BorderLayout.NORTH);
      frame.setSize(300, 300);
      frame.setVisible(true);
      
      
      frame.setFocusable(true);
      
      //Listen for a key press to make the game start.
      frame.addKeyListener(new KeyAdapter()
            {
               public void keyPressed(KeyEvent e)
               {
                  char theKey = e.getKeyChar();
                  if(theKey == 'c')
                  {
                     frame.getContentPane().removeAll();
                     frame.add(topBorder, BorderLayout.NORTH);
                     GameGridComponent scene = new GameGridComponent(CIRCLE_GAME);
                     frame.add(scene, BorderLayout.CENTER);
                     frame.setVisible(true);
                     
                  }
                  else if(theKey == 'r')
                  {
                     frame.getContentPane().removeAll();
                     frame.add(topBorder, BorderLayout.NORTH);
                     GameGridComponent scene = new GameGridComponent(RECTANGLE_GAME);
                     frame.add(scene, BorderLayout.CENTER);
                     frame.setVisible(true);
                  }
               }
         
            });
      
      
   }
   
   private static final int CIRCLE_GAME = 1;
   private static final int RECTANGLE_GAME = 0;
}