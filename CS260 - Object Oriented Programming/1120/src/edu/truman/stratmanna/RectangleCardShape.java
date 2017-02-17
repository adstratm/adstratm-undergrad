package edu.truman.stratmanna;

import java.awt.Graphics2D;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;


/**
 * A rectangular card that can flipped.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 19 November 2015
 */
public class RectangleCardShape extends AbstractCardShape
{
   /**
    * Constructs a rectangular card item.
    * @param x the left of the bounding rectangle
    * @param y the bottom of the bounding rectangle
    * @param width the width of the bounding rectangle
    */
   public RectangleCardShape(int x, int y, int width, int cardValue)
   {
      super(x, y, width, cardValue);
   }
   
   
   /**
    * Tests if a point is within the bounding rectangle of a card shape
    * @param p The point in question
    * @return The status of the point being within the card shape
    */
   public boolean contains(Point2D p)
   {
      return super.getX() <= p.getX() && p.getX() <= super.getX() + super.getWidth()
            && super.getY() <= p.getY() && p.getY() <= super.getY() + super.getWidth();
   }
   

   
   /**
    * Draws the rectangular card under
    * normal circumstances.
    * @param g2 The object to be drawn.
    */
   public void drawNormal(Graphics2D g2)
   {
            
      Point2D.Double upperLeft = 
            new Point2D.Double(super.getX(), super.getY() + super.getWidth());
      Point2D.Double upperRight = 
            new Point2D.Double(super.getX() + super.getWidth(), super.getY() + super.getWidth());
      Point2D.Double lowerLeft = 
            new Point2D.Double(super.getX(), super.getY());
      Point2D.Double lowerRight = 
            new Point2D.Double(super.getX() + super.getWidth(), super.getY());
      
      Line2D.Double top = 
            new Line2D.Double(upperLeft, upperRight);
      Line2D.Double bottom = 
            new Line2D.Double(lowerLeft, lowerRight);
      Line2D.Double leftSide = 
            new Line2D.Double(upperLeft, lowerLeft);
      Line2D.Double rightSide = 
            new Line2D.Double(upperRight, lowerRight);
      
      g2.draw(top);
      g2.draw(bottom);
      g2.draw(leftSide);
      g2.draw(rightSide);
      
      
   }
   
}