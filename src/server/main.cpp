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
#include "MediaDescription.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
   int port = 8080;
   string file = "media.json";

   HttpServer httpserver(port);
   MediaLibrary ml(httpserver, port, file);
   ml.StartListening();
   int c = getchar();
   ml.StopListening();
   return 0;
}
