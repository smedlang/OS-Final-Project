This program manipulates images -- the image can be set at the top of the program
Must have opencv and pkg-config

To compile: run this command

	g++ $(pkg-config --cflags --libs opencv) imageManipulator.cpp -o im -lpthread

Then to run:
	./im <choiceNumber>

choiceNumbers:

	1:inversion
	2:switch RBG values
	3:grayscale
	4:remove green
	5:remove blue
	6:remove red
	7:changes based on threadID



