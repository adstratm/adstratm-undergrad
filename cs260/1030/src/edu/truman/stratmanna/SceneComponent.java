package edu.truman.stratmanna;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.util.ArrayList;

import javax.swing.JComponent;


/**
 * A component that shows a scene composed of shapes.
 * @author Andrew Stratmann
 * ads5743@truman.edu
 * @version 29 October 2015
 *
 */
public class SceneComponent extends JComponent
{
   
   /**
    * Constructs a new SceneComponent object
    * This includes setting up the correct buttons for each
    * of the desired capabilities, as well as configuring them
    * to carry out their respective tasks.
    * 
    */
   private static final long serialVersionUID = 1L;
   public SceneComponent()
   {
      shapes = new ArrayList<SceneShape>();
      
      addMouseListener(new
         MouseAdapter()
         {
            public void mouseClicked(MouseEvent event)
            {
               mousePoint = event.getPoint();
               for (SceneShape s : shapes)
               {
                  if (s.contains(mousePoint))
                  {
                     s.setSelected(!s.isSelected());
                  }
               }
               repaint();
            }
         });
      
      
      
      addMouseListener(new
            MouseAdapter()
            {
               public void mousePressed(MouseEvent event)
               {
                  mousePoint = event.getPoint();
               }
            });
      
      
      addMouseMotionListener(new
         MouseMotionAdapter()
         {
            public void mouseDragged(MouseEvent event)
            {
               Point lastMousePoint = mousePoint;
               mousePoint = event.getPoint();
               for(SceneShape s : shapes)
               {
                  if (s.isSelected())
                  {
                     double dx = mousePoint.getX() - lastMousePoint.getX();
                     double dy = mousePoint.getY() - lastMousePoint.getY();
                     s.translate((int) dx, (int) dy);
                  }
               }
               repaint();
            }
         });
   }
   
   
   /** 
    * Adds a shape to the scene.
    * @param s the shape to add
    */
   public void add(SceneShape s)
   {
      shapes.add(s);
      repaint();
   }
   
   
   /**
    * Removes all selected shapes from the scene
    */
   public void removeSelected()
   {
      for (int i = shapes.size() - 1; i >= 0; i--)
      {
         SceneShape s = shapes.get(i);
         if (s.isSelected())
         {
            shapes.remove(i);
         }
         repaint();
      }
   }
   
   
   /**
    * Paints the component
    * @param g The graphics component
    */
   public void paintComponent(Graphics g)
   {
      super.paintComponent(g);
      Graphics2D g2 = (Graphics2D) g;
      for (SceneShape s : shapes)
      {
         s.drawNormal(g2);
         if (s.isSelected())
         {
            s.drawSelected(g2);
         }
      }
   }
   
   
   private ArrayList<SceneShape> shapes;
   private Point mousePoint;
   

   
}
