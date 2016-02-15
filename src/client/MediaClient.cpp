#include "MediaClientGui.cpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Tree.H>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <array>
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <json/json.h>

#include "mediastub.h"

using namespace jsonrpc;
using namespace std;
/**
 * Copyright (c) 2015 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This class has been modified by Steven Carneado for use with
 * the Assign 5 MediaServer.
 * <p>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ FLTK client sample.
 * This class extends the Gui component class MediaClientGui.
 * Sample control functions that respond to button clicks and tree item
 * selections.
 * This software is meant to run on Debian Wheezy Linux
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @file    MediaClient.cpp
 * @date    August, 2015
 **/

// string variable and run method for use with threading. A separate thread
// is created to run the vlc application, which plays the music or video media file.
std::string cmd;
void run(){
   std::system(cmd.c_str());
}

class MediaClient : public MediaClientGui {

public:

   // Don't make the client stub for the json-rpc-cpp an instance variable of
   // this object. In FLTK, callbacks may be handled on different threads, and
   // because of the way libjson-rpc-cpp is implmented, you must recreate
   // the stub in each callback method in which its used.
   std::string appAuthor;
   std::thread * playThread;
   string host;


   /**
    * Constructor for the MediaClient class.
    * This constructor creates the callback for clicking the X (exit) in the window,
    * creates callback for all menu item selections, and creates a callback
    * for tree selection events. Event callbacks must be static methods, so the
    * object passed to the callback is the MediaClient object. The static callback
    * methods then call an instance method on the instance. For example,
    * a tree item selection calls the TreeCallbackS static method, passing it
    * the MediaClient instance (this) as an argument. The static TreeCallbackS uses
    * the instance to call the instance method which has access to the MediaClient,
    * and MediaClientGui instance variables (Fl_Tree, FL_Input (title, artist, genre,
    * album), Fl_Choice (mediaType)) necessary to implement the control functionality.
    **/
   MediaClient(std::string name, string newhost) : MediaClientGui(name.c_str()) {
      host = newhost;
      callback(ClickedX, (void*)this);
      playThread = NULL;
      menubar->callback(Menu_ClickedS, (void*)this);
      appAuthor = name;
      buildTree();
      tree->callback(TreeCallbackS, (void*)this);
   }

   /**
    * ClickedX is one of the callbacks for GUI controls.
    * Callbacks in FLTK need to be static functions. But, static functions
    * cannot directly access instance data. This program uses "userdata"
    * to get around that by passing the instance to the callback
    * function. The callback then accesses whatever GUI control object
    * that it needs for implementing its functionality.
    */
   static void ClickedX(Fl_Widget * w, void * userdata) {
      std::cout << "You clicked Exit" << std::endl;
      MediaClient *o = (MediaClient*)userdata;
      if(o->playThread != NULL && (o->playThread)->joinable()){
         (o->playThread)->join();
      }
      exit(1);
   }

   // Static tree callback method
   static void TreeCallbackS(Fl_Widget*w, void*data) {
      MediaClient *o = (MediaClient*)data;
      o->TreeCallback(); //call the instance callback method
   }

   /**
    * TreeCallback is a callback for tree selections, deselections, expand or
    * collapse.
    */
   void TreeCallback() {
      //Connect client to server
      HttpClient httpclient(host);
      mediastub ms(httpclient);

      // Find item that was clicked
      Fl_Tree_Item *item = (Fl_Tree_Item*)tree->item_clicked();
      std::cout << "Tree callback. Current selection is: ";
      if ( item ) {
         std::cout << item->label();
      } else {
         std::cout << "none";
      }
      std::cout << endl;
      std::string aStr("unknown");
      std::string aTitle(item->label());
      switch ( tree->callback_reason() ) {  // reason callback was invoked
        case       FL_TREE_REASON_NONE: {aStr = "none"; break;}
        case     FL_TREE_REASON_OPENED: {aStr = "opened";break;}
        case     FL_TREE_REASON_CLOSED: {aStr = "closed"; break;}
        case   FL_TREE_REASON_SELECTED: {
           aStr = "selection";
           Json::Value temp = ms.get(aTitle);
           mediaType->value(temp[0].asInt());
           titleInput->value(aTitle.c_str());
           authorInput->value(temp[2].asCString());
           genreInput->value(temp[3].asCString());
           albumInput->value(temp[4].asCString());
           fileInput->value(temp[5].asCString());

           break;
        }
        case FL_TREE_REASON_DESELECTED: {aStr = "deselected"; break;}
      default: {break;}
      }
   std::cout << "Callback reason: " << aStr.c_str() << endl;
   }

   // Static menu callback method
   static void Menu_ClickedS(Fl_Widget*w, void*data) {
      MediaClient *o = (MediaClient*)data;
      o->Menu_Clicked(); //call the instance callback method
   }

   // Menu selection instance method that has ccess to instance vars.
   void Menu_Clicked() {
      //Connect client to server
      HttpClient httpclient(host);
      mediastub ms(httpclient);

      char picked[80];
      menubar->item_pathname(picked, sizeof(picked)-1);
      string selectPath(picked);
      cout << "Selected Menu Path: " << selectPath << endl;
      int select = mediaType->value();
      cout << "Selected media type: " << ((select==0)?"Music":"Video") << endl;
      // get a stub to the library server
      // Handle menu selections
      if(selectPath.compare("File/Save")==0){
         cout << "Menu item Save selected." << endl;
      }else if(selectPath.compare("File/Restore")==0){
         cout << "Menu item Restore selected." << endl;
      }else if(selectPath.compare("File/Tree Refresh")==0){
         cout << "Menu item Tree Refresh selected." << endl;
         buildTree();
      }else if(selectPath.compare("File/Exit")==0){
         cout << "Menu item Exit selected." << endl;
         if(playThread != NULL && playThread->joinable()){
            playThread->join();
         }
         exit(0);
      }else if(selectPath.compare("Media/Add")==0){
         cout << "Menu item Add selected." << endl;
         cout << "Adding media with title: " << titleInput->value()
              << " type " << mediaType->value()
              << " " << endl;
         //Add media to server
         bool addRes = ms.add(mediaType->value(), titleInput->value(), authorInput->value(),
              genreInput->value(), albumInput->value(), fileInput->value());
         //Rebuild tree if add successful
         if(addRes){
            buildTree();
         }
         cout << "Add " << ((addRes)?"successful":"unsuccessful") << endl;
       }else if(selectPath.compare("Media/Remove")==0){
         cout << "Menu item Removing selected." << endl;
         cout << "Removing media with title: " << titleInput->value()
              << " type " << mediaType->value()
              << " " << endl;
         //Remove media from server
         bool remRes = ms.remove(titleInput->value());
         //Rebuild tree if remove successful
         if(remRes){
            buildTree();
         }
         cout << "Remove " << ((remRes)?"successful":"unsuccessful") << endl;
      }else if(selectPath.compare("Media/Play")==0){
         // uname returns OS Name. This program defined to work with
         // Linux and Darwin (Darwin is Mac OSX)
         std::string unameres = exec("uname");
         // pwd is print working directory. Used to build an absolute path to the
         // media file to be played.
         std::string pwdPath = exec("pwd");
         pwdPath = pwdPath.substr(0,pwdPath.length()-1);
         std::cout << "OS type is: " << unameres << " curr.dir is: "
                   << pwdPath << std::endl;
         // This path is only valid on linux so we will have to check ostype
         std::stringstream streamLinux;
         streamLinux << "/usr/bin/vlc "
                     << pwdPath << "/"
                     << ((mediaType->value()==0)?"PaperNavySwanSong.mp3":
                         "MachuPicchuTimelapseVimeo.mp4");
         std::string aStr("Linux");
         std::stringstream streamMac;
         streamMac << "/Applications/VLC.app/Contents/MacOS/VLC "
                     << pwdPath << "/"
                     << ((mediaType->value()==0)?"PaperNavySwanSong.mp3":
                         "MachuPicchuTimelapseVimeo.mp4");
         cout << "mac command: " << streamMac.str() << endl;
         cout << "linux command: " << streamLinux.str() << endl;
         // start vlc to play the media file.
         // limit the comparison to the length of Linux to remove new line char.
         // Create a new thread to play the media using vlc on the appropriate system.
         // An attempt to exit the GUI/Application will block to join
         // (syncronize) with the thread/vlc
         if(unameres.compare(0,aStr.length(),aStr)==0){
            string argLinux(streamLinux.str());
            cmd = argLinux;
            playThread = new std::thread(run);
         }else{
            string arg(streamMac.str());
            cmd = arg;
            playThread = new std::thread(run);
         }
      }
   }

   // local method to execute a system command (uname or pwd). Note, the
   // command is executed on the main thread.
   std::string exec(const char* cmd) {
      FILE* pipe = popen(cmd, "r");
      if (!pipe) return "ERROR";
      char buffer[128];
      std::string result = "";
      while(!feof(pipe)) {
         if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
      }
      pclose(pipe);
      return result;
   }

   // local method to build the tree in the GUI left panel.
   void buildTree(){
      //Connect client
      HttpClient httpclient(host);
      mediastub ms(httpclient);

      //Get current media server titles
      Json::Value mtitles = ms.getMusicTitles();
      Json::Value vtitles = ms.getVideoTitles();

      vector<string> musicList;
      vector<string> musicAlbum;
      vector<string> videoList;
      vector<string> videoGenre;

      for(int i = 0; i < mtitles.size(); i++){
          Json::Value temp = ms.get(mtitles[i].asString());
          musicList.push_back(temp[1].asString());
          musicAlbum.push_back(temp[4].asString());
      }

      for(int i = 0; i < vtitles.size(); i++){
          Json::Value temp = ms.get(vtitles[i].asString());
          videoList.push_back(temp[1].asString());
          videoGenre.push_back(temp[3].asString());
      }


      cout << "Adding tree nodes for music titles: ";
      tree->clear();
      for(int i=0; i<musicList.size(); i++){
         cout << musicList[i] << ", ";
         string title = musicList[i];
         string album = musicAlbum[i];
         std::stringstream stream;
         stream << "Music"
                << "/"
                << album
                << "/" << title;
         tree->add(stream.str().c_str());
      }
      cout << endl << "Adding tree nodes for video titles: ";
      for(int i=0; i<videoList.size(); i++){
         cout << " " << videoList[i] << ", ";
         string title = videoList[i];
         string genre = videoGenre[i];
         std::stringstream stream;
         stream << "Video"
                << "/"
                << genre
                << "/" << title;
         tree->add(stream.str().c_str());
      }
      cout << endl;
      tree->root_label(appAuthor.c_str());
      tree->redraw();
   }

};

// main method for this program.
int main(int argc, char * argv[]) {
   std::string nameStr = "Harman Library";
   MediaClient mc(nameStr, "192.168.1.15");
   return (Fl::run());
}
