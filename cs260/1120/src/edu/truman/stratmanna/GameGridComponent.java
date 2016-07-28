package edu.truman.stratmanna;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Random;
import java.util.concurrent.ThreadLocalRandom;
import java.util.Timer;
import java.util.TimerTask;


import javax.swing.JComponent;


/**
 * A component that shows a scene of cards.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 19 November 2015
 */
public class GameGridComponent extends JComponent
{
   
   /**
    * Constructs a GameGridComponent object.
    * This manages the cards and removes a match.
    * @param shapeIndicator A '0' indicates a rectangular game, '1' for circular
    */
   private static final long serialVersionUID = 1L;
   public GameGridComponent(int shapeIndicator)
   {
      cards = new ArrayList<FlippableCard>(NUMBER_OF_CARDS);
      // Initialize the turn counter to zero.
      turnCounter = 0;
      
      timer = new Timer();
      
      possibleValues = new int[NUMBER_OF_CARDS];
      // Initialize the possible values for the cards.
      for(int j = MIN_CARD_VAL; j <= MAX_CARD_VAL; j++)
      {
         Random r = new Random();
         int nextVal = r.nextInt(MAX_VALUE) + 1;
         possibleValues[2 * j - 2] = nextVal;
         possibleValues[2 * j - 1] = nextVal;
      }
      
      shuffleArray(possibleValues);
      
      if(shapeIndicator == 0)
      {
         // Initialize the layout of cards.
         for(int i = 0; i < ROWS_AND_COLS; i++)
         {
            for(int j = 0; j < ROWS_AND_COLS; j++)
            {
               cards.add(new RectangleCardShape(SCALING_FACTOR * i + SHIFT_FACTOR,
                     SCALING_FACTOR * j, WIDTH_FACTOR, possibleValues[i + 4 * j]));
            }
         
            scoreCard = new RectangleCardShape(SCORECARD_X, SCORECARD_Y,
                  SCORECARD_WIDTH, turnCounter);
         
         }
      }
      else
      {
         // initialize the layout of circular cards.
         for(int i = 0; i < ROWS_AND_COLS; i++)
         {
            for(int j = 0; j < ROWS_AND_COLS; j ++)
            {
               cards.add(new CircleCardShape(SCALING_FACTOR * i + SHIFT_FACTOR,
                     SCALING_FACTOR * j, WIDTH_FACTOR, possibleValues[i + 4 * j]));
            }
            
            scoreCard = new RectangleCardShape(SCORECARD_X, SCORECARD_Y,
                  SCORECARD_WIDTH, turnCounter);
         }
      }
      
      
      addMouseListener(new
         MouseAdapter()
         {
            public void mouseClicked(MouseEvent event)
            {
               mousePoint = event.getPoint();
               for (FlippableCard theCard : cards)
               {
                  if (theCard.contains(mousePoint) && !theCard.isFlipped())
                  {
                     theCard.setFlipped(!theCard.isFlipped());
                  }
               }
               
               repaint();
               
               
               // Check if another tile has already been selected
               numberSelected = 0;
               for(FlippableCard theCard : cards)
               {
                  if(theCard.isFlipped())
                  {
                     numberSelected++;
                     if(numberSelected < 2)
                     {
                        firstSelected = theCard.getCardValue();
                     }
                     else
                     {
                        secondSelected = theCard.getCardValue();
                     }
                        
                  }
               }
               

               // If two cards are selected, continue
               if(numberSelected > 1)
               {
                  
                  // increase the score by one.
                  turnCounter++;
                  scoreCard.setCardValue(turnCounter);
           
                  
                  //If the two selected cards are equal, delete them.
                  if(firstSelected == secondSelected)
                  {
                     removeSelected();
                  }
                  
                  
                  /* Deselect all of the cards, waiting 
                   * 800 ms for the user to view their values.
                   */
                  timer.schedule(new DeselectTask(), TIME_DELAY);
                  
               }   
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
         FlippableCard s = cards.get(i);
         if (s.isFlipped())
         {
            cards.remove(i);
         }
      }
      repaint();
   }
   
   
   
   /**
    * Deselects all of the cards
    */
   private void deselectAll()
   {
      for(FlippableCard theCard : cards)
      {
         theCard.setFlipped(false);
      }
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
         if (cards.get(i).isFlipped())
         {
            cards.get(i).drawFlipped(g2);
         }
         else
         {
            cards.get(i).drawNormal(g2);
         }
      }
      scoreCard.drawFlipped(g2);
   }
   
   
   /**
    * Shuffles an array of integers
    * @param array The array to be shuffled
    */
   private void shuffleArray(int[] array)
   {
     Random rnd = ThreadLocalRandom.current();
     for (int i = array.length - 1; i > 0; i--)
     {
       int index = rnd.nextInt(i + 1);
       int a = array[index];
       array[index] = array[i];
       array[i] = a;
     }
   }
   
   
   /**
    * A class to deselect the cards and repaint
    * after a given time interval.
    * @author astratmann
    *
    */
   class DeselectTask extends TimerTask
   {
      public void run()
      {
         deselectAll();
         repaint();
      }
   }

   
   
   private ArrayList<FlippableCard> cards;
   private Point mousePoint;
   private int turnCounter;
   private int numberSelected;
   private int[] possibleValues;
   private int firstSelected;
   private int secondSelected;
   private RectangleCardShape scoreCard;
   private Timer timer;
   private static final int NUMBER_OF_CARDS = 16;
   private static final int SCALING_FACTOR = 60;
   private static final int SHIFT_FACTOR = 30;
   private static final int WIDTH_FACTOR = 50;
   private static final int TIME_DELAY = 800;
   private static final int MIN_CARD_VAL = 1;
   private static final int MAX_CARD_VAL = 8;
   private static final int MAX_VALUE = 99;
   private static final int ROWS_AND_COLS = 4;
   private static final int SCORECARD_X = 121;
   private static final int SCORECARD_Y = 220;
   private static final int SCORECARD_WIDTH = 0;
   
   
}