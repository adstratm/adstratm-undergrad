package edu.truman.stratmanna;

import java.awt.Graphics2D;
import java.awt.geom.Point2D;

/**
 * An interface for a card that can be selected
 * and moves.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 6 December 2015
 */
public interface SelectableMovingCard extends Runnable
{
   /**
    * Flips the card
    */
   void setFlipped();
   
   
   /**
    * Checks to see if a card is flipped
    * @return The flipped status of the card.
    */
   boolean isFlipped();
   
   
   /**
    * Draws the card.
    * @param g2 The graphics component
    */
   void drawCard(Graphics2D g2);
   
   
   /**
    * Translates the card to a new location.
    * @param dx The change in the x direction.
    * @param dy The change in the y direction.
    */
   void translate(int dx, int dy);
   
   
   /**
    * Tests to see if a point lies within a card
    * @param aPoint The point in question
    * @return The status of a point within a card
    */
   boolean contains(Point2D aPoint);
   
}