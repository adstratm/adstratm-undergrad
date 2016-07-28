package edu.truman.stratmanna;

import java.awt.Graphics2D;
import java.awt.geom.Point2D;

/**
 * A circular card shape.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 19 November 2015
 */
public class CircleCardShape extends AbstractCardShape
{
   /**
    * Constructs a circle shape.
    * @param x the left of the bounding rectangle
    * @param y the top of the bounding rectangle
    * @param width the width of the bounding rectangle
    */
   public CircleCardShape(int x, int y, int width, int cardValue)
   {
      super(x, y, width, cardValue);
      centerX = x + (width / 2);
      centerY = y + (width / 2);
   }
   
   
   /**
    * Draws the circular card under normal conditions.
    * @param g2 the graphics element
    */
   public void drawNormal(Graphics2D g2)
   {
      g2.drawOval(super.getX(), super.getY(), (super.getWidth()), (super.getWidth()));

   }
   
   
   /**
    * Tests if a point lies within a circle
    * @param p The point in question
    * @return The status of the point being within the circle shape
    */
   public boolean contains(Point2D p)
   {
      double distanceToCenter = 0;
      double pointX = p.getX();
      double pointY = p.getY();
      distanceToCenter = Math.sqrt(Math.pow((pointX - centerX), 2) + Math.pow((pointY - centerY), 2));
      if(distanceToCenter < (super.getWidth() / 2))
      {
         return true;
      }
      else
      {
         return false;
      }
         
   }
   
   
   private int centerY;
   private int centerX;

}