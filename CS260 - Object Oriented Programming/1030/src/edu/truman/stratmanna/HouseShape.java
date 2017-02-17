package edu.truman.stratmanna;

import java.awt.Graphics2D;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;

/**
 * A house shape
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 29 October 2015
 *
 */
public class HouseShape extends AbstractSceneShape
{
   /**
    * Constructs a house shape.
    * @param x the left of the bounding rectangle
    * @param y the top of the bounding rectangle
    * @param width the width of the bounding rectangle
    */
   public HouseShape(int x, int y, int width)
   {
      super(x, y, width);
   }
   
   
   /**
    * Draws the house under normal conditions.
    * @param g2 the graphics element
    */
   public void drawNormal(Graphics2D g2)
   {
      Rectangle2D.Double base = new Rectangle2D.Double(super.getX(),
            super.getY() + super.getWidth(),
            super.getWidth(), super.getWidth());
      
      // The left bottom of the roof
      Point2D.Double r1 = new Point2D.Double(super.getX(),
            super.getY() + super.getWidth());
      
      // The top of the roof
      Point2D.Double r2 = new Point2D.Double(super.getX() + super.getWidth() / 2,
            super.getY());
      
      //The bottom right of the roof
      Point2D.Double r3 = new Point2D.Double(super.getX() + super.getWidth(),
            super.getY() + super.getWidth());
      
      Line2D.Double roofLeft = new Line2D.Double(r1, r2);
      Line2D.Double roofRight = new Line2D.Double(r2, r3);
      
      
      g2.draw(base);
      g2.draw(roofLeft);
      g2.draw(roofRight);
   }
   
   
   /**
    * Tests if a point lies within the bounding rectangle of a house
    * @param p The point in question
    * @return The status of the point being within the house shape
    */
   public boolean contains(Point2D p)
   {
      return super.getX() <= p.getX() 
            && p.getX() <= super.getX() + super.getWidth()
            && super.getY() <= p.getY()
            && p.getY() <= super.getY() + 2 * super.getWidth();
   }
   
   

}
