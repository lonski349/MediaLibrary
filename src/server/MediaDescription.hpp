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
 * Purpose: This class stores media file information
 *
 * @author Brian Harman bsharman@asu.edu
 * @version February 2016
 */

#ifndef MediaDescription_hpp
#define MediaDescription_hpp

#include <stdio.h>
#include <iostream>
#include <json/json.h>

using namespace std;

class MediaDescription {
    int mediaType;
    string title, author, album, genre, filename;

public:
    //declaring the onstructor
    MediaDescription(int, string, string, string, string, string);

    MediaDescription(Json::Value obj);
    // ~MediaDescription();

    //declaring the accessors
    int getMediaType();
    string getTitle();
    string getAuthor();
    string getAlbum();
    string getGenre();
    string getFilename();

    //declaring the mutators
    void setMediaType(int);
    void setTitle(string);
    void setAuthor(string);
    void setAlbum(string);
    void setGenre(string);
    void setFilename(string);

    //override toString method
    string toString();
    Json::Value toJson();
};

#endif /* MediaDescription_hpp */
