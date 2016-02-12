#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Tree_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

/**
 * \class MediaClientGui
 * Copyright (c) 2015 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p>
 * This class has been modified by Steven Carneado for use with
 * the Assign 5 MediaServer.
 * <p>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p>
 * Purpose: Sample C++ FLTK view class. MediaClientGui constructs the view
 * for media app. This class is extended by the client controller which is
 * the MediaClient class. MediaClient defines the call-backs for UI controls.
 * It contains sample control functions that respond to button clicks and tree
 * selects.
 * This software is meant to run on Debian Wheezy Linux
 * <p>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * \author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * \file    MediaClientGui.cpp
 * \date    August, 2015
 **/
class MediaClientGui : public Fl_Window {
protected:

   /**
    * tree is the Fl_Tree object that occupies the left side of the window.
    * This tree control provides the ability to select nodes. The app uses
    * this selection functionality to request
    * actions based on the selection, such as remove a media item, or play a media
    * file.
    */
   Fl_Tree * tree;

   /**
    * titleInput is the Fl_Input object labelled Title.
    * Its for the user to enter the media tile.
    * The controller places the title of the tree's currently selected media
    * in this field.
    */
   Fl_Input * titleInput;

   /**
    * albumInput is the Fl_Input object labelled Album.
    * Its for the user to enter the media album name. For videos, genre is
    * used to organize video. For music, album is used as the organization term.
    * The controller places the album of the tree's currently selected media
    * in this field.
    */
   Fl_Input * albumInput;

   /**
    * authorInput is the Fl_Input object labelled Artist.
    * Its for the user to enter the artist for music, or for entering the
    * primary actor for videos.
    * The controller places the artist of the tree's currently selected media
    * in this field.
    */
   Fl_Input * authorInput;

   /**
    * genreInput is the Fl_Input object labelled Genre.
    * Its for the user to enter the genre for music or video.
    * The controller places the genre of the tree's currently selected media
    * in this field.
    */
   Fl_Input * genreInput;

  /**
    * fileInput is the Fl_Input object labelled Filename.
    * Its for the user to enter the Filename for music or video.
    * The controller places the genre of the tree's currently selected media
    * in this field.
    */
   Fl_Input * fileInput;

   /**
    * menubar is the Fl_Menu_Bar object containing all of the menuitems for the Gui.
    */
   Fl_Menu_Bar *menubar;

   /**
    * mediaType is the Fl_Choice object labelled Media Type.
    * It provides a drop-down for the user to select Music or Video. It is also
    * used by the controller to indicate whether a music or video title is being
    * displayed.
    */
   Fl_Choice *mediaType;

public:

   /** A Constructor for MediaClientGui
    * Constructor taking a c-string argument, which is used as a window label
    * for the client application.
    * @param name the c-string to be used as window title and root of tree
    **/
   MediaClientGui(const char * name = "Ser321") : Fl_Window(635,350,name) {
      begin();

      menubar = new Fl_Menu_Bar(0, 0, this->w(), 25);
      menubar->add("File/Save");
      menubar->add("File/Restore");
      menubar->add("File/Tree Refresh");
      menubar->add("File/Exit");
      menubar->add("Media/Add");
      menubar->add("Media/Remove");
      menubar->add("Media/Play");

      // create a tree control at position x=10, y=10. Its 150 pixels wide
      // and window height less 45 pixels high. Add some sample tree nodes.
      tree = new Fl_Tree(10, 35, 225, this->h()-45);
      tree->add("Flintstones/Fred");
      tree->add("Flintstones/Wilma");
      tree->close("/Flintstones");
      tree->root_label(name);

      /*
       * add a text input control at x=250, y=35 of width 200 pixels and
       * height of 25 pixels. Initialize it contents to media title.
       */
      //Fl_Label * titleLab = new Fl_Label(175, 35, 75, 25);
      //titleLab->value("Title");
      titleInput = new Fl_Input(285, 35, 325, 25);
      titleInput->label("Title");
      titleInput->value("media title");

      /*
       * add a text input control at x=250, y=35 of width 200 pixels and
       * height of 25 pixels. Initialize it contents to media title.
       */
      albumInput = new Fl_Input(285, 105, 325, 25);
      albumInput->label("Album");
      albumInput->value("album title");

      genreInput = new Fl_Input(285, 175, 175, 25);
      genreInput->label("Genre");
      genreInput->value("genre description");

      // create the media type drop-down (input_choice)
      mediaType = new Fl_Choice(515, 175, 100, 25, "Type");
      mediaType->add("Music");
      mediaType->add("Video");
      mediaType->value(0); // set the control initially to Music

      authorInput = new Fl_Input(285, 245, 325, 25);
      authorInput->label("Artist");
      authorInput->value("artist name");

      fileInput = new Fl_Input(285, 315, 325,25);
      fileInput->label("File");
      fileInput->value("file name");

      end();
      show();
   }
};
