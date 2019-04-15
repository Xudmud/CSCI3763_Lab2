# CSCI3763_Lab2  
-------------------------------------------------------------------------------  
 Name:       Gregory Wicklund  
 Student ID: 810906774  
 Class:      CSCI3761 Intro to Computer Networks, Spring 2019  
 Lab 2:      Socket Programming  
 Due Date:   8 April 2019  
-------------------------------------------------------------------------------  

Instructions:  
Run make all from the root directory  
$ make all

To run the server:  
$ ./wic6774sockserver

To run a client and connect to the server:  
$ ./wic6774sockclient \<server ip\>


This project is a work-in-progress, it will be updated as I complete more of it.  
The folder structure may change without warning, based on progress and requirements.  

--Update 11 March 2019--  
Client and Server will each need their own program, and are designed to run on  
separate machines. Split them into their own folders to keep code separate.

--Update 9 April 2019--  
Restructured makefiles to only use a single makefile, for ease of compilation.  The bin directories in server and client are probably not needed anymore...

--Update 13 April 2019--  
Socket functionality is working, with placeholder text in place for the supported functions. Working on getting the functions implemented.