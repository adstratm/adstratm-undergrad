package edu.truman.stratmanna;

import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;
import javax.swing.Icon;

/**
 * An icon that contains a moveable shape.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * 1 October 2015
 */
public class ShapeIcon implements Icon
{
   /**
    * Create a Shape Icon object.
    * @param shape The shape to be contained in the icon.
    * @param width The width of the icon.
    * @param height The height of the icon.
    */
   public ShapeIcon(MoveableShape shape, int width, int height)
   {
      this.shape = shape;
      this.width = width;
      this.height = height;
   }
   
   /**
    *  Retrieve the width of the icon.
    *  @return The width of the icon. 
    */
   public int getIconWidth()
   {
      return width;
   }
   
   /**
    * Retrieve the height of the icon.
    * @return The height of the icon.
    */
   public int getIconHeight()
   {
      return height;
   }

   /**
    * Paints the icon (required for the interface)
    * @param C useful information needed before painting the icon
    * @param g the base graphics class needed to produce output
    * @param x the x coordinate of the location where the icon will be painted
    * @param y the y coordinate of the location where the icon will be painted
    */
   public void paintIcon(Component C, Graphics g, int x, int y)
   {
      Graphics2D g2 = (Graphics2D) g;
      shape.draw(g2);
   }

   private int width;
   private int height;
   private MoveableShape shape;

}
