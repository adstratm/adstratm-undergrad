package edu.truman.stratmanna;

import java.awt.Graphics2D;
import java.awt.geom.Point2D;

/**
 * An interface for a card that can be flipped.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 19 November 2015
 */
public interface FlippableCard
{
   /**
    * Sets the flipped state of the card
    * @param b The desired flipped state
    */
   void setFlipped(boolean b);
   
   
   /**
    * Tests to see if the card is flipped
    * @return The flipped status of the card
    */
   boolean isFlipped();
   
   
   /**
    * Draws the card under normal conditions
    * @param g2 The graphics component
    */
   void drawNormal(Graphics2D g2);
   
   
   /**
    * Draws the card when it is flipped
    * @param g2 The graphics component
    */
   void drawFlipped(Graphics2D g2);
   
   
   /**
    * Returns the value of the card
    * @return the value of the card
    */
   int getCardValue();
   
   
   /**
    * Tests to see if a point lies within a card
    * @param aPoint The point in question
    * @return The status of a point within a card
    */
   boolean contains(Point2D aPoint);
   
}