package edu.truman.stratmanna;

import java.awt.Graphics2D;

/**
 * A shape that manages its flipped state.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 19 November 2015
 *
 */
public abstract class AbstractCardShape implements FlippableCard
{

   
   /**
    * Constructs a new AbstractCardShape object.
    * This constructor sets the values of x, y, and width
    * @param x The left of the bounding rectangle
    * @param y The top of the bounding rectangle
    * @param width The width of the bounding rectangle
    */
   public AbstractCardShape(int x, int y, int width, int cardValue)
   {
      this.x = x;
      this.y = y;
      this.width = width;
      this.cardValue = cardValue;
   }
   
   
   /**
    * Sets the flipped state of the shape
    * @param b the desired flipped state
    */
   public void setFlipped(boolean b)
   {
      flipped = b;
   }

   
   /**
    * Tests if the shape is currently flipped
    * @return The flipped selection state of the shape
    */
   public boolean isFlipped()
   {
      return flipped;
   }
   
   
   /**
    * Sets the width of the bounding rectangle
    * @param width The width of the bounding rectangle
    */
   public void setWidth(int width)
   {
      this.width = width;
   }
   
   
   /**
    * Returns the left of the bounding rectangle
    * @return The left of the bounding rectangle
    */
   public int getX()
   {
      return x;
   }

   
   /**
    * Returns the bottom of the bounding rectangle
    * @return The bottom of the bounding rectangle
    */
   public int getY()
   {
      return y;
   }
   
   
   /**
    * Returns the width of the bounding rectangle
    * @return The width of the bounding rectangle
    */
   public int getWidth()
   {
      return width;
   }
   
   
   /**
    * Returns the value of the card.
    * @return The value of the card.
    */
   public int getCardValue()
   {
      return cardValue;
   }
   
   
   /**
    * Sets the value of the card.
    * @param value The value to be stored in the card.
    */
   public void setCardValue(int value)
   {
      cardValue = value;
   }
  
  
   /**
    * Draws the shape in the flipped state
    * @param g2 The graphics component
    */
   public void drawFlipped(Graphics2D g2)
   {
      drawNormal(g2);
      g2.drawString(Integer.toString(cardValue), x + OFFSET_X,
            y + OFFSET_Y);
   }
   
   
   private boolean flipped;
   // The top of the bounding rectangle
   private int x;
   // The left of the bounding rectangle
   private int y;
   // The width of the bounding rectangle
   private int width;
   
   // The value of the card
   private int cardValue;
   /*
    * The x offset required for the value
    * to display in the center of the card.
    */
   private static final int OFFSET_X = 20;
   
   /*
    * The y offset required for the value to 
    * display in the center of the card.
    */
   private static final int OFFSET_Y = 30;


}