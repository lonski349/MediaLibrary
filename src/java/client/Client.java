
import ser321.media.*;
import java.net.*;
import javax.swing.*;
import java.io.*;
import javax.sound.sampled.*;
import java.beans.*;
import java.net.*;
import javax.swing.tree.*;
import javax.swing.event.*;
import javax.swing.text.html.*;
import javax.swing.filechooser.*;
import java.awt.event.*;
import java.awt.*;
import java.util.*;
import java.lang.Runtime;
import org.json.JSONObject;
import org.json.JSONArray;

/**
 * Copyright 2015 Steven Carneado,
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
 * Purpose: GUI Media client for MediaServer
 *
 * @author Steven Carneado scarnead@asu.edu
 *
 * @version October 2015
 */

public class Client extends MediaLibraryGui implements
			TreeWillExpandListener,ActionListener, TreeSelectionListener {

   private static final boolean debugOn = true;
   private static final boolean bootstrapOn = false;
   private static final int downloadSize = 10000000;
   private boolean stopPlaying;         //shared with playing thread.

   private Socket sock;
   private String host;
   private int portNum;
   private String serviceURL;
   private JsonRpcRequestViaHttp server;
   public static int id = 0;

   public Client(String author, String serviceURL, String host, int portNum ) {
      super(author);
      this.serviceURL = serviceURL;
      this.host = host;
      this.portNum = portNum;

      //Attempt to connect to Server
      try{
    	  this.server = new JsonRpcRequestViaHttp(new URL(serviceURL));
      }catch (Exception ex){
    	  System.out.println("Malformed URL " + ex.getMessage());
      }

      stopPlaying = false;
      if(bootstrapOn){
         System.out.println("bootstraping a single media description ...");
      }

      for(int i=0; i<userMenuItems.length; i++){
         for(int j=0; j<userMenuItems[i].length; j++){
            userMenuItems[i][j].addActionListener(this);
         }
      }
      //tree.addTreeWillExpandListener(this);
      try{
         tree.addTreeSelectionListener(this);
         rebuildTree();
      }catch (Exception ex){
         JOptionPane.showMessageDialog(this,"Handling "+
                                " constructor exception: " + ex.getMessage());
      }
      setVisible(true);
   }

   private void debug(String message) {
      if (debugOn)
         System.out.println("debug: "+message);
   }

   /**
    * create and initialize nodes in the JTree of the left pane.
    * buildInitialTree is called by MediaLibraryGui to initialize the JTree.
    * Classes that extend MediaLibraryGui should override this method to
    * perform initialization actions specific to the extended class.
    * The default functionality is to set base as the label of root.
    * In your solution, you will probably want to initialize by deserializing
    * your library and displaying the categories and subcategories in the
    * tree.
    * @param root Is the root node of the tree to be initialized.
    * @param base Is the string that is the root node of the tree.
    */
   public void buildInitialTree(DefaultMutableTreeNode root, String base){
      //set up the context and base name
      try{
         root.setUserObject(base);
      }catch (Exception ex){
         JOptionPane.showMessageDialog(this,"exception initial tree:"+ex);
         ex.printStackTrace();
      }
   }

   public void rebuildTree(){
	  String[] musicList = this.getMusicTitles();
	  String[] videoList = this.getVideoTitles();
      String[] musicAlbum = new String[musicList.length];
      String[] videoGenre = new String[videoList.length];

      for(int i = 0; i < musicList.length; i++){
    	  JSONObject temp = get(musicList[i]);
    	  musicAlbum[i] = getAlbum(temp);
      }

      for(int i = 0; i < videoList.length; i++){
    	  JSONObject temp = get(videoList[i]);
    	  videoGenre[i] = getGenre(temp);
      }

      tree.removeTreeSelectionListener(this);
      DefaultTreeModel model = (DefaultTreeModel)tree.getModel();
      DefaultMutableTreeNode root = (DefaultMutableTreeNode)model.getRoot();
      clearTree(root, model);
      DefaultMutableTreeNode musicNode = new DefaultMutableTreeNode("Music");
      model.insertNodeInto(musicNode, root, model.getChildCount(root));
      DefaultMutableTreeNode videoNode = new DefaultMutableTreeNode("Video");
      model.insertNodeInto(videoNode, root, model.getChildCount(root));
      // put nodes in the tree for all  registered with the library
      for (int i = 0; i<musicList.length; i++){
         String aMTitle = musicList[i];
         String anAlbum = musicAlbum[i];
         DefaultMutableTreeNode toAdd = new DefaultMutableTreeNode(aMTitle);
         DefaultMutableTreeNode subNode = getSubLabelled(musicNode,anAlbum);
         if(subNode!=null){ // if album subnode already exists
            debug("album exists: "+anAlbum);
            model.insertNodeInto(toAdd, subNode,
                                 model.getChildCount(subNode));
         }else{ // album node does not exist
            DefaultMutableTreeNode anAlbumNode =
               new DefaultMutableTreeNode(anAlbum);
            model.insertNodeInto(anAlbumNode, musicNode,
                                 model.getChildCount(musicNode));
            DefaultMutableTreeNode aSubCatNode =
               new DefaultMutableTreeNode("aSubCat");
            debug("adding subcat labelled: "+"aSubCat");
            model.insertNodeInto(toAdd,anAlbumNode,
                                 model.getChildCount(anAlbumNode));
         }
      }
      // put nodes in the tree for all video registered with the library
      for (int i = 0; i<videoList.length; i++){
         String aTitle = videoList[i];
         String aGenre = videoGenre[i];
         DefaultMutableTreeNode toAdd = new DefaultMutableTreeNode(aTitle);
         DefaultMutableTreeNode subNode = getSubLabelled(videoNode,aGenre);
         if(subNode!=null){ // if album subnode already exists
            model.insertNodeInto(toAdd, subNode, model.getChildCount(subNode));
         }else{ // album node does not exist
            DefaultMutableTreeNode anAlbumNode =
               new DefaultMutableTreeNode(aGenre);
            model.insertNodeInto(anAlbumNode, videoNode,
                                 model.getChildCount(videoNode));
            DefaultMutableTreeNode aSubCatNode =
               new DefaultMutableTreeNode("aSubCat");
            debug("adding subcat labelled: "+"aSubCat");
            model.insertNodeInto(toAdd,anAlbumNode,
                                 model.getChildCount(anAlbumNode));
         }
      }
      // expand all the nodes in the JTree
      for(int r =0; r < tree.getRowCount(); r++){
         tree.expandRow(r);
      }
      tree.addTreeSelectionListener(this);
   }

   private void clearTree(DefaultMutableTreeNode root, DefaultTreeModel model){
      try{
         DefaultMutableTreeNode next = null;
         int subs = model.getChildCount(root);
         for(int k=subs-1; k>=0; k--){
            next = (DefaultMutableTreeNode)model.getChild(root,k);
            debug("removing node labelled:"+(String)next.getUserObject());
            model.removeNodeFromParent(next);
         }
      }catch (Exception ex) {
         System.out.println("Exception while trying to clear tree:");
         ex.printStackTrace();
      }
   }

   private DefaultMutableTreeNode getSubLabelled(DefaultMutableTreeNode root,
                                                 String label){
      DefaultMutableTreeNode ret = null;
      DefaultMutableTreeNode next = null;
      boolean found = false;
      for(Enumeration e = root.children(); e.hasMoreElements();){
         next = (DefaultMutableTreeNode)e.nextElement();
         debug("sub with label: "+(String)next.getUserObject());
         if (((String)next.getUserObject()).equals(label)){
            debug("found sub with label: "+label);
            found = true;
            break;
         }
      }
      if(found)
         ret = next;
      else
         ret = null;
      return ret;
   }

   public void treeWillCollapse(TreeExpansionEvent tee) {
      debug("In treeWillCollapse with path: "+tee.getPath());
      tree.setSelectionPath(tee.getPath());
   }

   public void treeWillExpand(TreeExpansionEvent tee) {
      debug("In treeWillExpand with path: "+tee.getPath());
      //DefaultMutableTreeNode dmtn =
      //    (DefaultMutableTreeNode)tee.getPath().getLastPathComponent();
      //System.out.println("will expand node: "+dmtn.getUserObject()+
      //		   " whose path is: "+tee.getPath());
   }

   public void valueChanged(TreeSelectionEvent e) {
      try{
         tree.removeTreeSelectionListener(this);
         DefaultMutableTreeNode node = (DefaultMutableTreeNode)
            tree.getLastSelectedPathComponent();
         if(node!=null){
            String nodeLabel = (String)node.getUserObject();
            debug("In valueChanged. Selected node labelled: "+nodeLabel);
            // is this a terminal node?
            if(node.getChildCount()==0 &&
               (node != (DefaultMutableTreeNode)tree.getModel().getRoot())){
               JSONObject media = this.get(nodeLabel);
               JSONArray mediaArr = media.getJSONArray("result");

               typeJCB.setSelectedIndex(mediaArr.getInt(0));
               titleJTF.setText(mediaArr.getString(1));
               authorJTF.setText(mediaArr.getString(2));
               genreJTF.setText(mediaArr.getString(3));
               albumJTF.setText(mediaArr.getString(4));
               fileNameJTF.setText(mediaArr.getString(5));

            }
         }
      }catch (Exception ex){
         ex.printStackTrace();
      }
      tree.addTreeSelectionListener(this);
   }

   public void actionPerformed(ActionEvent e) {
      tree.removeTreeSelectionListener(this);
      if(e.getActionCommand().equals("Exit")) {
         System.exit(0);
      }else if(e.getActionCommand().equals("Restore")) {
         rebuildTree();
         System.out.println("Restore "+((true)?"successful":"unsuccessful"));
      }else if(e.getActionCommand().equals("Tree Refresh")) {
         rebuildTree();
      }else if(e.getActionCommand().equals("Remove")){
    	  DefaultMutableTreeNode node = (DefaultMutableTreeNode)
    	            tree.getLastSelectedPathComponent();

    	  	   String nodeLabel = (String)node.getUserObject();
    	  	   if(node.getChildCount()==0 &&
                  (node != (DefaultMutableTreeNode)tree.getModel().getRoot())){
                  boolean success = this.remove(nodeLabel);

                  if(success){
                	  System.out.println(nodeLabel + " removed.");
                	  rebuildTree();
                  }
               }
      }else if(e.getActionCommand().equals("Add")){
    	        boolean success = this.add(typeJCB.getSelectedIndex(), titleJTF.getText(),
                authorJTF.getText(), genreJTF.getText(), albumJTF.getText(),
                fileNameJTF.getText());

                if(success){
              	  System.out.println(titleJTF.getText() + " added.");
              	  rebuildTree();
                }
      }else if(e.getActionCommand().equals("Play")){
         try{
            String nodeFileName = fileNameJTF.getText();
            String mediaFileName = (typeJCB.getSelectedIndex()==0)?"DataClient/mediafile.mp3":
                "DataClient/mediafile.mp4";
            String aURIPath = "file://"+System.getProperty("user.dir")+"/"+
               mediaFileName;

            sock = new Socket(host, portNum);
            byte[] bytesToSend = nodeFileName.getBytes();
            byte[] bytesToReceive = new byte[downloadSize];

            OutputStream os = sock.getOutputStream();
            InputStream is = sock.getInputStream();
            FileOutputStream fileOutStream = new FileOutputStream(mediaFileName);
            BufferedOutputStream bufOutStream = new BufferedOutputStream(fileOutStream);
            os.write(bytesToSend, 0, bytesToSend.length);

            int count;

            while((count = is.read(bytesToReceive, 0, bytesToReceive.length)) > 0){
            	bufOutStream.write(bytesToReceive, 0, count);
            }

            bufOutStream.flush();

            playMedia(aURIPath, nodeFileName);
         }catch(Exception ex){
            System.out.println("Execption trying to play media:");
            ex.printStackTrace();
         }
      }
      tree.addTreeSelectionListener(this);
   }

   public boolean sezToStop(){
      return stopPlaying;
   }

   //Adds constant JSON info
   private String packageMediaCall(String oper){
	   JSONObject jsonObj = new JSONObject();
	   jsonObj.put("jsonrpc","2.0");
	   jsonObj.put("method",oper);
	   jsonObj.put("id",++id);
	   return jsonObj.toString();
   }

   //The Add method attempts to add a media file to the server
   public boolean add(int mtype, String title, String author, String genre,
		   String album, String filename){
	   boolean result = false;
	   try{
		   String jsonString = packageMediaCall("Add");
		   String insert = ",\"params\":[" + mtype + ",\"" + title + "\",\""
				   + author + "\",\"" + genre + "\",\"" + album + "\",\"" + filename + "\"]";
		   String begin = jsonString.substring(0,jsonString.length()-1);
		   String end = jsonString.substring(jsonString.length()-1);
		   jsonString = begin + insert + end;
		   System.out.println(jsonString);
		   String resString = server.call(jsonString);
		   JSONObject res = new JSONObject(resString);
		   result = res.optBoolean("result");

	   }catch(Exception ex){
		   System.out.println("Exception in rpc call to add " + ex.getMessage());
	   }

	   return result;
   }

   //The remove method attempts to remove a media description from the server
   public boolean remove(String title){
	   boolean result = false;
	   try{
		   String jsonString = packageMediaCall("Remove");
		   String insert = ",\"params\":[\"" + title + "\"]";
		   String begin = jsonString.substring(0,jsonString.length()-1);
		   String end = jsonString.substring(jsonString.length()-1);
		   jsonString = begin + insert + end;
		   System.out.println(jsonString);
		   String resString = server.call(jsonString);
		   JSONObject res = new JSONObject(resString);
		   result = res.optBoolean("result");

	   }catch(Exception ex){
		   System.out.println("Exception in rpc call to Remove " + ex.getMessage());
	   }

	   return result;
   }

   //The get method attempts to get a MediaDescription from the server
   public JSONObject get(String title){
	   JSONObject res = null;
	   try{
		   String jsonString = packageMediaCall("Get");
		   String insert = ",\"params\":[\"" + title + "\"]";
		   String begin = jsonString.substring(0,jsonString.length()-1);
		   String end = jsonString.substring(jsonString.length()-1);
		   jsonString = begin + insert + end;
		   System.out.println(jsonString);
		   String resString = server.call(jsonString);
		   res = new JSONObject(resString);

	   }catch(Exception ex){
		   System.out.println("Exception in rpc call to get " + ex.getMessage());
	   }

	   return res;
   }

   //The getTitles method attempts to get the titles of all MediaDescriptions
   //on the server.
   public String[] getTitles(){
	   ArrayList<String> titles = new ArrayList<String>();
	   try{
		   String jsonString = packageMediaCall("GetTitles");
		   String insert = ",\"params\":[]";
		   String begin = jsonString.substring(0,jsonString.length()-1);
		   String end = jsonString.substring(jsonString.length()-1);
		   jsonString = begin + insert + end;
		   System.out.println(jsonString);
		   String resString = server.call(jsonString);
		   JSONObject res = new JSONObject(resString);
		   JSONArray arr = res.optJSONArray("result");

		   for(int i = 0; i < arr.length(); i++){
			   titles.add(arr.optString(i));
		   }

	   }catch(Exception ex){
		   System.out.println("Exception in rpc call to getTitles " + ex.getMessage());
	   }

	   return titles.toArray(new String[titles.size()]);
   }

   public String[] getMusicTitles(){
	   ArrayList<String> titles = new ArrayList<String>();
	   try{
		   String jsonString = packageMediaCall("GetMusicTitles");
		   String insert = ",\"params\":[]";
		   String begin = jsonString.substring(0,jsonString.length()-1);
		   String end = jsonString.substring(jsonString.length()-1);
		   jsonString = begin + insert + end;
		   System.out.println(jsonString);
		   String resString = server.call(jsonString);
		   JSONObject res = new JSONObject(resString);
		   JSONArray arr = res.optJSONArray("result");

		   for(int i = 0; i < arr.length(); i++){
			   titles.add(arr.optString(i));
		   }

	   }catch(Exception ex){
		   System.out.println("Exception in rpc call to getMusicTitles " + ex.getMessage());
	   }

	   return titles.toArray(new String[titles.size()]);
   }

   public String[] getVideoTitles(){
	   ArrayList<String> titles = new ArrayList<String>();
	   try{
		   String jsonString = packageMediaCall("GetVideoTitles");
		   String insert = ",\"params\":[]";
		   String begin = jsonString.substring(0,jsonString.length()-1);
		   String end = jsonString.substring(jsonString.length()-1);
		   jsonString = begin + insert + end;
		   System.out.println(jsonString);
		   String resString = server.call(jsonString);
		   JSONObject res = new JSONObject(resString);
		   JSONArray arr = res.optJSONArray("result");

		   for(int i = 0; i < arr.length(); i++){
			   titles.add(arr.optString(i));
		   }

	   }catch(Exception ex){
		   System.out.println("Exception in rpc call to getVideoTitles" + ex.getMessage());
	   }

	   return titles.toArray(new String[titles.size()]);
   }

   public String serviceInfo(){
	   String msg = null;
	   try{
		   String jsonString = packageMediaCall("serviceInfo");
		   String insert = ",\"params\":[]";
		   String begin = jsonString.substring(0,jsonString.length()-1);
		   String end = jsonString.substring(jsonString.length()-1);
		   jsonString = begin + insert + end;
		   System.out.println(jsonString);
		   String resString = server.call(jsonString);
		   JSONObject res = new JSONObject(resString);
		   msg = res.optString("result");

	   }catch(Exception ex){
		   System.out.println("Exception in rpc call to serviceInfo " + ex.getMessage());
	   }

	   return msg;
   }

   private String getAlbum(JSONObject obj){
	   JSONArray temp = obj.optJSONArray("result");
	   ArrayList<String> values = new ArrayList<String>();
	   String result = "";

	   for(int i = 1; i < 4; i++){
		   values.add(temp.getString(i));
	   }

	   result = values.get(2);
	   return result;
   }

   private String getGenre(JSONObject obj){
	   JSONArray temp = obj.optJSONArray("result");
	   ArrayList<String> values = new ArrayList<String>();
	   String result = "";

	   for(int i = 1; i < 5; i++){
		   values.add(temp.getString(i));
	   }

	   result = values.get(3);
	   return result;
   }


   public static void main(String args[]) {
      try{
         String authorName = "Steven Carneado Library";
         String url = "http://127.0.0.1:8080/";
         String host = "127.0.0.1";
         int port = 3030;

         if(args.length >=1){
            url = args[0];
            host = args[1];
            port = Integer.parseInt(args[2]);
         }
         System.out.println("calling constructor name "+authorName);
         Client mla = new Client(authorName, url, host, port);
      }catch (Exception ex){
         System.out.println("Exception in main: "+ex.getMessage());
         ex.printStackTrace();
      }
   }
}
