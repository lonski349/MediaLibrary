import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

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
 * Purpose: Download server used with the Java Media Client
 *
 * @author Brian Harman bsharman@asu.edu
 *
 * @version March 2016
 */
public class DownloadServer extends Thread {

    public static final String mp3 = "Server/Style.mp3";
    public static final String mp4 = "Server/Nature.mp4";
    private Socket conn;
    private int id;

    public DownloadServer(Socket sock, int id) {
        this.conn = sock;
        this.id = id;
    }

    public void run () {
        try {
            OutputStream outSock = conn.getOutputStream();
            InputStream inSock = conn.getInputStream();
            FileInputStream fileStream = null;
            BufferedInputStream buffStream = null;
            File outFile = null;
            byte clientInput[] = new byte[1024];
            int num = inSock.read(clientInput, 0, 1024);
            String clientStr = new String(clientInput, 0, num);
            System.out.println("ID: " + id);
            System.out.println("String: " + clientStr);

            if(clientStr.contains(".mp3")) {
                outFile = new File(mp3);
            } else if (clientStr.contains(".mp4")) {
                outFile = new File(mp4);
            } else {
                System.out.println("File type must be either mp3 or mp4. ");
            }

            byte[] toClient = new byte[(int)outFile.length()];
            fileStream = new FileInputStream(outFile);
            buffStream = new BufferedInputStream(fileStream);
            buffStream.read(toClient, 0, toClient.length);
            outSock.write(toClient, 0, toClient.length);
            outSock.flush();
            inSock.close();
            outSock.close();
            buffStream.close();
            conn.close();

        } catch (IOException e) {
            System.out.println("Can't get I/O for the connection. ");
        }
    }

    public static void main(String[] args) {
        int id = 0;
        try {
            if (args.length != 1) {
                System.out.println("Port number must be declared. ");
                System.exit(0);
            }
            int port = Integer.parseInt(args[0]);
            if (port <= 1024) port = 3030;
            ServerSocket server = new ServerSocket(port);
            while (true) {
                System.out.println("Server waiting for connection on port " + port);
                Socket sock = server.accept();
                System.out.println("Server connected to client " + id);
                DownloadServer serverThread = new DownloadServer(sock, id++);
                serverThread.start();
            }
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

}
