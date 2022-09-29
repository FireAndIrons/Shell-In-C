Members:
	Samantha Fisher
	Kylie Facison
	Brian Granados Cuellar
	Eduardo Garcia (No Show/MIA)

Organization of the Project:
	Samantha Fisher:
		- Change Dir
		- Pipelining 
	Kylie Facison:
		- Path
		- Redirection
	Brian Granados Cuellar
		- Exit
		- Signal Handling
	Eduardo Garcia (No Show/MIA):
		- My History
		- Alias

Design Overview:
	- Checks arg count to determine interactive mode or batch mode
		1) If 1 arg, run interactive loop, else read in batch file and process
	- Continuous loop that 
		1) Reads the line
		2) Checks line length
		3) Parses line
		4) Runs commands, whether built-in or executed with exec()

Complete Specification:
	- Handles lines >= 1 length with an error message and returns to loop
	- Handles lines with just semicolons and no commands by checking if string is NULL and string length and returns to loop

Known Bugs or Problems:
	- Commands not implemented due to having only 3 actve members in group:
		- myhistory
		- alias
		- pipelining
		- redirection
	- Additionally, because pipelining was not implemented, certain commands have been manually removed from the program (i.e. grep, sort, etc.)