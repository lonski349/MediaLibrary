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

#ifndef MediaLibrary_hpp
#define MediaLibrary_hpp

#include <stdio.h>
#include "MediaDescription.hpp"
#include <json/json.h>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdlib.h>
#include "mediaserverstub.h"

using namespace jsonrpc;

class MediaLibrary : public mediaserverstub {
public:
    MediaLibrary(AbstractServerConnecter(&connector, int port, string mediafile)) : mediaserverstub(connector);
    void notifyServer();
    string serviceInfo();
    MediaLibrary();
    MediaLibrary(string jsonFile);
    ~MediaLibrary();
    bool Add(MediaDescription aClip);
    bool Remove(string title);
    Json::Value get(string title);
    Json::Value getTitles();
    Json::Value getMusicTitles();
    Json::Value getVideoTitles();

    void toJsonFile(string jsonfile);

private:
    vector<MediaDescription> library;
    int findMedia(string title);
    int portNumber;
};

#endif /* MediaLibrary_hpp */
