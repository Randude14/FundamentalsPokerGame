# FundamentalsPokerGame
Create a dealer (server) that will simulate a 5 hand card poker game that will interface with other players (clients) that have GUI interfaces that will play the game.

	Be sure to download the GitHub Windows client: https://desktop.github.com/
	NOTE: You will have to link a shared folder with the VBox to have access to these files.

1. Choose the option on the client to clone a repository with the URL:
	https://github.com/Randude14/FundamentalsPokerGame.git
	
	This will clone the project to your local file system



2. Any files you change will show up in the GitHub Windows Client. Before adding, please compile the program to make sure it works. Enter a description before comitting and select "Commit to master". You will then see your commit and then you will click the "push to origin master". Any commits we push can be reversed on the repository so if in the event something wrong happens, we can always roll back the code changes.

3. Before working on the project, be sure to check that there aren't any file changes that need to be pulled from the repository.

4. All header files go in the "include" directory and the source (definition) files go in the "src" directory. The build directory is for compiling and running the projects.

5. To compile the program:

	make clean
	make
	
6. To run the player:
	./poker_client
	
7. To run the dealer:
	./poker_dealer