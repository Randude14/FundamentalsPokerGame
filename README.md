# FundamentalsPokerGame
Create a dealer (server) that will simulate a 5 hand card poker game that will interface with other players (clients) that have GUI interfaces that will play the game.

1. Choose the option on the client to clone a repository with the URL:
	https://github.com/Randude14/FundamentalsPokerGame.git
	
	This will clone the project to your local file system
	
	Also be sure to download the GitHub Windows client: https://desktop.github.com/
	NOTE: You will have to link a shared folder with the VBox to have access to these files.


2. Any files you change will show up in the GitHub Windows Client. Before adding, please compile the program to make sure it works. Enter a description before comitting and select "Commit to master". You will then see your commit and then you will click the "push to origin master". Any commits we push can be reversed on the repository so if in the event something wrong happens, we can always roll back the code changes.

3. Before working on the project, be sure to check that there aren't any file changes that need to be pulled from the repository.




COMPILING THE PROGRAM:

Go into the build directory
	cd build
	
Remove all files within
	rm -rf *
	
Invoke cmake to create Makefile
	cmake ..
	
Run Makefile to compile the program
	make
	
The client and server executables will be created
	./poker_client
	./poker_server