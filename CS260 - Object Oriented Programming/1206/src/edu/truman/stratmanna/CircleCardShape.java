package edu.truman.stratmanna;

import java.awt.Graphics2D;
import java.awt.geom.Point2D;

/**
 * A shape that manages its selection state and moves
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 6 December 2015
 *
 */
public class CircleCardShape implements SelectableMovingCard
{

   /**
    * Constructs a new CircleCardShape object.
    * This constructor sets the values of x, y, and delay
    * @param x The left of the bounding rectangle
    * @param y The bottom of the bounding rectangle
    * @param delay The delay between translations
    */
   public CircleCardShape(int x, int y, int delay)
   {
      this.x = x;
      this.y = y;
      this.delay = delay;
      
      centerX = x + (WIDTH / 2);
      centerY = y + (WIDTH / 2);
      movingRight = true;
   }
   
   
   /**
    * Draws the card under normal conditions.
    * @param g2 The graphics component.
    */
   public void drawCard(Graphics2D g2)
   {
      g2.drawOval(x, y, WIDTH, WIDTH);
   }
   
   
   /**
    * Translates the card to a new location.
    * @param dx The change in the x direction.
    * @param dy The change in the y direction.
    */
   public void translate(int dx, int dy)
   {
      x = x + dx;
      y = y + dy;
      centerX = x + (WIDTH / 2);
      centerY = y + (WIDTH / 2);
   }
   
   
   /**
    * Tests if a point is contained within a card.
    * @param p The point in question.
    * @return The status of the point being in the card.
    */
   public boolean contains(Point2D p)
   {
      double distanceToCenter = 0;
      double pointX = p.getX();
      double pointY = p.getY();
      distanceToCenter = Math.sqrt(Math.pow((pointX - 
            centerX), 2) + Math.pow((pointY - centerY), 2));
      if(distanceToCenter < (WIDTH / 2))
      {
         return true;
      }
      else
      {
         return false;
      }
   }
   
   
   /**
    * Checks if a card is flipped.
    * @return The flipped status of the card.
    */
   public boolean isFlipped()
   {
      return flipped;
   }
   
   
   /**
    * Flips the shape
    */
   public void setFlipped()
   {
      flipped = true;
   }

   
   /**
    * The actions to be taken when the card's
    * thread runs.
    */
   public void run()
   {
      try
      {
         while(true)
         {
            if(x + WIDTH >= FRAME_SIZE)
            {
               x = (FRAME_SIZE - WIDTH);
               movingRight = false;
            }
            else if(x <= 0)
            {
               x = 0;
               movingRight = true;
            }
            
            if(movingRight == true)
            {
               translate(1, 0);
            }
            else
            {
               translate(-1, 0);
            }
            Thread.sleep(delay);
         }
      }
      catch(InterruptedException exception)
      {
         //nothing
      }

   }
   
   
   private boolean flipped;
   // The top of the bounding rectangle
   private int x;
   // The left of the bounding rectangle
   private int y;
   private int delay;
   private int centerX;
   private int centerY;
   private static final int WIDTH = 50;
   private boolean movingRight;
   private static final int FRAME_SIZE = 600;

}