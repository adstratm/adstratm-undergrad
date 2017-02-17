package edu.truman.stratmanna;

import java.awt.Graphics2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;


/**
 * A car that can be moved around.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 8 October 2015
 */
public class CarShape extends AbstractSceneShape
{
   /**
    * Constructs a car item.
    * @param x the left of the bounding rectangle
    * @param y the top of the bounding rectangle
    * @param width the width of the bounding rectangle
    */
   public CarShape(int x, int y, int width)
   {
      super(x, y, width);
      ORIGINAL_WIDTH = width;
   }
   
   
   /**
    * Tests if a point is within the bounding rectangle of a car shape
    * @param p The point in question
    * @return The status of the point being within the car shape
    */
   public boolean contains(Point2D p)
   {
      return super.getX() <= p.getX() && p.getX() <= super.getX() + super.getWidth()
            && super.getY() <= p.getY() && p.getY() <= super.getY() + super.getWidth() / 2;
   }
   

   
   /**
    * Draw a 2d graphics object.
    * @param g2 The object to be drawn.
    */
   public void drawNormal(Graphics2D g2)
   {
      Rectangle2D.Double body
         = new Rectangle2D.Double(super.getX(), super.getY() + super.getWidth() / 6,
               super.getWidth() - 1, super.getWidth() / 6);
      Ellipse2D.Double frontTire
         = new Ellipse2D.Double(super.getX() + super.getWidth() / 6, super.getY() + super.getWidth() / 3,
               super.getWidth() / 6, super.getWidth() / 6);
      Ellipse2D.Double rearTire
         = new Ellipse2D.Double(super.getX() + super.getWidth() * 2 / 3, super.getY() + super.getWidth() / 3,
               super.getWidth() / 6, super.getWidth() / 6);
      
      // The bottom of the front windshield
      Point2D.Double r1
         = new Point2D.Double(super.getX() + super.getWidth() / 6, super.getY() + super.getWidth() / 6);
      // The front of the roof
      Point2D.Double r2
         = new Point2D.Double(super.getX() + super.getWidth() / 3, super.getY());
      // The rear of the roof
      Point2D.Double r3
         = new Point2D.Double(super.getX() + super.getWidth() * 2 / 3, super.getY());
      // The bottom of the rear windshield
      Point2D.Double r4
         = new Point2D.Double(super.getX() + super.getWidth() * 5 / 6, super.getY() + super.getWidth() / 6);
      Line2D.Double frontWindshield
         = new Line2D.Double(r1, r2);
      Line2D.Double roofTop
         = new Line2D.Double(r2, r3);
      Line2D.Double rearWindshield
         = new Line2D.Double(r3, r4);
      
      
      g2.draw(body);
      g2.draw(frontTire);
      g2.draw(rearTire);
      g2.draw(frontWindshield);
      g2.draw(roofTop);
      g2.draw(rearWindshield);
      
   }
   
   
   /**
    * Change the size of the car by way of a scaling factor.
    * @param factor The scaling factor
    */
   public void setSize(int factor)
   {
      super.setWidth((ORIGINAL_WIDTH * factor) / SCALING_FACTOR_NO_CHANGE);
   }
   
   
   private final int ORIGINAL_WIDTH;
   private static final int SCALING_FACTOR_NO_CHANGE = 50;
   
   
}