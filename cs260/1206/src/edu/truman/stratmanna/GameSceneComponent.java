package edu.truman.stratmanna;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Random;
import javax.swing.JComponent;


/**
 * A component that shows a scene of cards.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 6 December 2015
 */
public class GameSceneComponent extends JComponent
{
   
   /**
    * Constructs a GameSceneComponent object.
    * This manages the cards and removes selected cards
    * @param difficulty The difficulty level of the game
    */
   private static final long serialVersionUID = 1L;
   public GameSceneComponent(int difficulty)
   {
      
      cards = new ArrayList<CircleCardShape>(NUMBER_OF_CARDS);
      cardsRemaining = NUMBER_OF_CARDS;
      possibleXValues = new int[NUMBER_OF_CARDS];
      possibleYValues = new int[NUMBER_OF_CARDS];
      // Initialize the possible values for the cards.
      for(int j = 0; j < NUMBER_OF_CARDS; j++)
      {
         Random r = new Random();
         int nextVal = r.nextInt(MAX_VALUE) + 1;
         possibleXValues[j] = nextVal;
      }
      
      for(int j = 0; j < NUMBER_OF_CARDS; j++)
      {
         Random r = new Random();
         int nextVal = r.nextInt(MAX_VALUE) + 1;
         possibleYValues[j] = nextVal;
      }
      
      
      // initialize the layout of circular cards
      for(int j = 0; j < NUMBER_OF_CARDS; j++)
      {
         cards.add(new CircleCardShape(possibleXValues[j],
               possibleYValues[j], difficulty));
      }
      
      // the action to be taken when the component is clicked
      addMouseListener(new
         MouseAdapter()
         {
            public void mouseClicked(MouseEvent event)
            {
               mousePoint = event.getPoint();
               for (SelectableMovingCard theCard : cards)
               {
                  if (theCard.contains(mousePoint))
                  {
                     theCard.setFlipped();
                     cardsRemaining--;
                  }
               }
               removeSelected();
               repaint();
                
            } 
         });
   }
      
      
   /**
    * Removes all selected cards from the scene
    */
   public void removeSelected()
   {
      for (int i = cards.size() - 1; i >= 0; i--)
      {
         SelectableMovingCard s = cards.get(i);
         if (s.isFlipped())
         {
            cards.remove(i);
         }
      }
      repaint();
   }
   
   
   /**
    * Paints the component
    * @param g The graphics component
    */
   public void paintComponent(Graphics g)
   {
      super.paintComponent(g);
      Graphics2D g2 = (Graphics2D) g;
      for (int i = cards.size() - 1; i >= 0; i--)
      {
         cards.get(i).drawCard(g2);
      }
   }
   
   
   /**
    * Determines if any cards remain in the game.
    * @return The status of cards being left in the game.
    */
   public boolean doCardsRemain()
   {
      if(cardsRemaining == 0)
      {
         return false;
      }
      else
      {
         return true;
      }
   }
   
   
   /**
    * Start the threads for each of the cards 
    * in motion.
    */
   public void startMotion()
   {
      
      //start the threads of the rectangles.
      for (int i = 0; i < cards.size(); i++)
      {
         new Thread(cards.get(i)).start();
      }
   }

   
   private ArrayList<CircleCardShape> cards;
   private Point mousePoint;
   private int[] possibleXValues;
   private int[] possibleYValues;
   private static final int NUMBER_OF_CARDS = 45;
   private static final int MAX_VALUE = 480;
   private int cardsRemaining;
   
}