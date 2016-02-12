/**
 * Copyright 2016 Brian Harman,
 *
 * ASU has permission to use this code to copy, execute, and distribute as
 * necessary for evaluation in the Ser321 course, and as otherwise required
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
 * Purpose: The main class demonstrates the functionality of the
 * MediaLibrary and MediaDescription class.
 *
 * @author Brian Harman bsharman@asu.edu
 *
 * @version Januaray 2016
 */

#include <iostream>
#include "MediaLibrary.hpp"

int main() {

    // //Add music to library
    // if(library.Add(MediaDescription("Music", "Slow Show", "The National", "Matt Berninger", "Indie", "Slow_Show.mp3")))
    //     cout << "\n";
    // if(library.Add(MediaDescription("Music", "Can't Do Without You", "Caribou", "Dan Snaith", "Electronic", "Can't_Do_Without_You.mp3")))
    //     cout << "\n";
    // if(library.Add(MediaDescription("Music", "Wanderlust", "Wild Beasts", "Hayden Thorpe", "Indie", "Wanderlust.mp3")))
    //     cout << "\n";

    // //Add videos to library
    // if(library.Add(MediaDescription("Video", "Toy Story", "Pixar", "", "Animation", "Toy_Story.mp4")))
    //     cout << "\n";
    // if(library.Add(MediaDescription("Video", "Inside Out", "Pixar", "", "Animation", "Inside_Out.mp4")))
    //     cout << "\n";
    // if(library.Add(MediaDescription("Video", "Wreck It Ralph", "Pixar", "", "Animation", "Wreck_It_Ralph.mp4")))
    //     cout << "\n";

    // //Display music titles in library
    // cout << "\nMusic titles in library: \n";
    // vector<string> musLib = library.getMusicTitles();
    // for(int i = 0; i < musLib.size(); i++) {
    //     cout << musLib[i] << "\n";
    // }

    // //Display video titles in library
    // cout << "\nVideo titles in library: \n";
    // vector<string> vidLib = library.getVideoTitles();
    // for(int i = 0; i < vidLib.size(); i++) {
    //     cout << vidLib[i] << "\n";
    // }

    // //Display music information
    // cout << "\nRetrieving track information for Slow Show... \n";
    // cout << library.get("Slow Show").toString();
    // cout << endl << endl;


    // //Display video information
    // cout << "Retrieving video information for Toy Story... \n";
    // cout << library.get("Toy Story").toString();
    // cout << endl << endl;

    // //Remove media from library
    // if(library.Remove("Wreck It Ralph"))
    // cout << "Deleting Wreck It Ralph from the library... \n";
    //      << "Wreck It Ralph has been removed from the library. \n";

    //Instantiate Json MediaLibrary
    MediaLibrary jLibrary = MediaLibrary::MediaLibrary("media.json");

    //Displays the updated list of items in the library
    cout << "==================================================\n";
    cout << "Importing Library\n";
    cout << "==================================================\n";
    //jLibrary.Add(*new MediaDescription("Music", "Slow Show", "The National", "Matt Berninger", "Indie", "Slow_Show.mp3"));
    //jLibrary.getTitles();vector<string> medLib;
    Json::Value medLib;
    medLib = jLibrary.getTitles();
    for (int i = 0; i < medLib.size(); i++) {
        cout << medLib[i] << " added\n";
    }
    cout << "==================================================\n";
    cout << "Library Exported Successfully\n";
    cout << "==================================================\n";
    jLibrary.toJsonFile("media.json");
    return 0;
}
