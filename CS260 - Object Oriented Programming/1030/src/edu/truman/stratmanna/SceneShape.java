package edu.truman.stratmanna;

import java.awt.Graphics2D;
import java.awt.geom.Point2D;



/**
 * An interface for a shape that can be selected.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 29 October 2015
 *
 */
public interface SceneShape
{
   /**
    * Sets the selection state of the shape
    * @param b The desired selection state
    */
   void setSelected(boolean b);
   
   
   /**
    * Tests to see if the shape is selected
    * @return The selection status of the shape
    */
   boolean isSelected();
   
   
   /**
    * Draws the shape under normal conditions
    * @param g2 The graphics component
    */
   void drawNormal(Graphics2D g2);
   
   
   /**
    * Draws the shape when it is selected 
    * @param g2 The graphics component
    */
   void drawSelected(Graphics2D g2);
   
   
   /**
    * Translates the shape
    * @param dx The change in the x-direction
    * @param dy The change in the y-direction
    */
   void translate(int dx, int dy);
   
   
   /**
    * Tests to see if a point lies within a shape
    * @param aPoint The point in question
    * @return The status of a point within a shape
    */
   boolean contains(Point2D aPoint);
   
   

}
