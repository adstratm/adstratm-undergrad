package edu.truman.stratmanna;

import java.awt.Graphics2D;

/**
 * A shape that manages its selection state.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 29 October 2015
 *
 */
public abstract class AbstractSceneShape implements SceneShape
{

   
   
   /**
    * Constructs a new AbstractSceneShape object.
    * This constructor sets the values of x, y, and width
    * @param x The left of the bounding rectangle
    * @param y The top of the bounding rectangle
    * @param width The width of the bounding rectangle
    */
   public AbstractSceneShape(int x, int y, int width)
   {
      this.x = x;
      this.y = y;
      this.width = width;
   }
   
   
   /**
    * Sets the selection state of the shape
    * @param b the desired selection state
    */
   public void setSelected(boolean b)
   {
      selected = b;
   }

   
   /**
    * Tests if the shape is currently selected
    * @return The current selection state of the shape
    */
   public boolean isSelected()
   {
      return selected;
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
    * Returns the top of the bounding rectangle
    * @return The top of the bounding rectangle
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
    * Translates the shape to a new location
    * @param dx The change in the x-direction
    * @param dy The change in the y-direction
    */
   public void translate(int dx, int dy)
   {
      x += dx;
      y += dy;
   }
  
  
   /**
    * Draws the shape in the selected state
    * @param g2 The graphics component
    */
   public void drawSelected(Graphics2D g2)
   {
      translate(1, 1);
      drawNormal(g2);
      translate(1, 1);
      drawNormal(g2);
      translate(-2, -2);

   }
   
   
   private boolean selected;
   // The top of the bounding rectangle
   private int x;
   // The left of the bounding rectangle
   private int y;
   // The width of the bounding rectangle
   private int width;


}
