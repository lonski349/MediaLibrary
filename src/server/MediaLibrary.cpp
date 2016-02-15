/**
 * Copyright 2016 Brian Harman
 *
 * ASU has permission to use this code to copy, execute, and distribute as
 * necessary for evaluation in the SER 321 course, and as otherwise required
 * for SE program evaluation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Purpose: This class provides a method to store and retrieve information
 * from MediaDescription objects.
 *
 * @author Brian Harman bsharman@asu.edu
 * @version January 2016
 */

#include "MediaLibrary.hpp"
#include <json/json.h>
#include <iostream>
#include <fstream>

MediaLibrary::MediaLibrary(AbstractServerConnecter &connector, int port, string mediafile) : mediaserverstub(connector) {
  Json::Reader reader;
  Json::Value root;

  ifstream infile(jsonfile.c_str(), ifstream::binary);
  bool parse = reader.parse(infile,root,false);
  if(parse){
      Json::Value::Members values = root.getMemberNames();
      for(int i = 0; i < values.size(); i++){
          Json::Value media = root[values[i]];
          library.push_back(MediaDescription(media));
      }
  }

  portNumber = port;
  cout << "Server running on port " << port << endl;
}

void MediaLibrary::notifyServer() {
  cout << "Media server notified" << endl;
}

string MediaLibrary::serviceInfo() {
  string message = "Permissible commands include: Add, Remove, Get, GetTitles, GetMusicTitles, GetVideoTitles on ";
  return message.append(portNumber);
}

//Add MediaDescription
bool MediaLibrary::Add(int mediaType, const string &title, const string &author, const string &album, const string &genre, const string &filename) {
    library.push_back(MediaDescription(mediaType, title, author, album, genre, filename));
    cout << title << " has been added to the Media Library ";
    return true;
}

//Remove Media
bool MediaLibrary::Remove(const string &title) {
    int found = findMedia(title);
    bool removed = false;
    if(found != -1) {
        cout << "Deleting " << title << " from the library... \n";
        library.erase(library.begin() + found);
        cout << title << " has been removed from the Media Library \n";
        removed = true;
    }
    return removed;
}

//Returns the MediaDescription
Json::Value MediaLibrary::get(const string &title) {
   Json::Value media;
   int found = findMedia(title);
   media.append(library.at(found).getMediaType());
   media.append(library.at(found).getTitle());
   media.append(library.at(found).getAuthor());
   media.append(library.at(found).getGenre());
   media.append(library.at(found).getAlbum());
   media.append(library.at(found).getFilename());
   return media;
}

//Returns a vector of all titles.
Json::Value MediaLibrary::getTitles() {
    Json::Value titles;
    for(int i = 0; i < library.size(); i++) {
        titles.append(library[i].getTitle());
    }
    return titles;
}

//Returns a vector of music titles
Json::Value MediaLibrary::getMusicTitles() {
    Json::Value titles;
    for(int i = 0; i < library.size(); i++) {
        if(library[i].getMediaType() == 0) {
            titles.append(library[i].getTitle());
        }
    }
    return titles;
}

//Returns a vector of video titles
Json::Value MediaLibrary::getVideoTitles() {
    Json::Value titles;
    for(int i = 0; i < library.size(); i++) {
        if(library[i].getMediaType() == 1) {
            titles.append(library[i].getTitle());
        }
    }
    return titles;
}

//Returns the index of a specified title
int MediaLibrary::findMedia(const string &title) {
    int found = -1;
    for(int i = 0; i < library.size(); i++) {
        if(title.compare(library[i].getTitle()) == 0) {
            found = i;
        }
    }
    return found;
}

void MediaLibrary::toJsonFile(string jsonfile){
    Json::Value json_media;

    for(int i = 0; i < library.size(); i++){
        string key = library[i].getTitle();

        Json::Value media = library[i].toJson();

        json_media[key] = media;
    }

    Json::StyledStreamWriter writer("  ");
    ofstream json_out(jsonfile.c_str(), ofstream::binary);
    writer.write(json_out, json_media);

}
