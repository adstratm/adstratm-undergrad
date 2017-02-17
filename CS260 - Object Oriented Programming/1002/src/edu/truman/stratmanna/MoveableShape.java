package edu.truman.stratmanna;

import java.awt.Graphics2D;

/**
 * A shape that can be moved around.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * 1 October 2015
 */
public interface MoveableShape 
{
   /**
    * Draws the shape.
    * @param g2 the graphics context
    */
   void draw(Graphics2D g2);
   
   /**
    * Moves the shape by a given amount.
    * @param dx the amount to translate in the x-direction
    * @param dy the amount to translate in the y-direction
    */
   void translate(int dx, int dy);
	

}
