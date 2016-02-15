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
 * @version January 2016
 */

#include "MediaDescription.hpp"

//defining the constructor
MediaDescription::MediaDescription(int mediaType, string title, string author, string album, string genre, string filename) {
    this->mediaType = mediaType;
    this->title = title;
    this->author = author;
    this->album = album;
    this->genre = genre;
    this->filename = filename;
}

// MediaDescription::~MediaDescription() {
//     this->mediaType = "";
//     this->title = "";
//     this->author = "";
//     this->album = "";
//     this->genre = "";
//     this->filename = "";
// }

//JSON file constructor
MediaDescription::MediaDescription(Json::Value obj) {
    this->mediaType = obj["mediaType"].asInt();
    this->title = obj["title"].asString();
    this->author = obj["author"].asString();
    this->genre = obj["genre"].asString();
    this->album = obj["album"].asString();
    this->filename = obj["filename"].asString();
}

//defining the accessors
int MediaDescription::getMediaType() {
    return this->mediaType;
}
string MediaDescription::getTitle() {
    return this->title;
}
string MediaDescription::getAuthor() {
    return this->author;
}
string MediaDescription::getAlbum() {
    return this->album;
}
string MediaDescription::getGenre() {
    return this->genre;
}
string MediaDescription::getFilename() {
    return this->filename;
}

//defining the mutators
void MediaDescription::setMediaType(int mediaType) {
    this->mediaType = mediaType;
}
void MediaDescription::setTitle(string title) {
    this->title = title;
}
void MediaDescription::setAuthor(string author) {
    this->author = author;
}
void MediaDescription::setAlbum(string album) {
    this->album = album;
}
void MediaDescription::setGenre(string genre) {
    this->genre = genre;
}
void MediaDescription::setFilename(string filename) {
    this->filename = filename;
}

//Converts MediaDescription to Json Value
Json::Value MediaDescription::toJson() {
    Json::Value media;
    media["mediaType"] = this->mediaType;
    media["title"] = this->title;
    media["author"] = this->author;
    media["album"] = this->album;
    media["genre"] = this->genre;
    media["filename"] = this->filename;
    return media;
}

//defining the toString method
string MediaDescription::toString() {
    return "MediaDescription{ mediaType = " + mediaType +
           ", title = '" + title + '\'' +
           ", author = '" + author + '\'' +
           ", album = '" + album + '\'' +
           ", genre = " + genre +
           ", filename = " + filename +
           '}';
}
